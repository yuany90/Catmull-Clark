#include <vector>
#include <fstream>
#include <GL\glut.h>
#include "Point.h"
#include "CatmullClark.h"
#define LINE_LENGTH 200
using namespace std;

Edge::Edge(int id, int vertexID1, int vertexID2, int faceID1, int faceID2){
	this->id = id;
	this->vertexID1 = vertexID1;
	this->vertexID2 = vertexID2;
	this->faceID1 = faceID1;
	this->faceID2 = faceID2;
}

void Edge::displayEdge(){
	Point v1 = cc->allvertices[vertexID1].coord;
	Point v2 = cc->allvertices[vertexID2].coord;
	glBegin(GL_LINE_STRIP);
	glVertex3f(v1.x, v1.y, v1.z);
	glVertex3f(v2.x, v2.y, v2.z);
	glEnd();
}

void Edge::calEdgePoint(){
	edgePoint.toZero();
	edgePoint =edgePoint + cc->allvertices[vertexID1].coord;
	edgePoint =edgePoint + cc->allvertices[vertexID2].coord;
	edgePoint =edgePoint + cc->allfaces[faceID1].facePoint;
	edgePoint =edgePoint + cc->allfaces[faceID2].facePoint;
	edgePoint.divide(4);
}

Edge::~Edge(){
}

Face::Face(int id){
	this->id = id;
}

Face::Face(int id, vector<int> vertices, vector<int> edges){
	this->id = id;
	this->vertices = vertices;
	this->edges = edges;
}

void Face::displayFace(){
	glBegin(GL_POLYGON);
	for(int i=0; i<vertices.size(); i++){
		int index = vertices[i];
		Point v = cc->allvertices[index].coord;
		glVertex3f(v.x, v.y, v.z);
	}
	glEnd();
}

void Face::calFacePoint(){
	int n = vertices.size();
	facePoint.toZero();
	for(int i = 0; i < n; i++){
		int index = vertices[i];
		facePoint = facePoint + (cc)->allvertices[index].coord;
	}
	facePoint.divide(n);
}

Face::~Face(){
}

Vertex::Vertex(Point coord, int index, CatmullClark * cc){
	this->coord = coord;
	id = index;
	this->cc = cc;
}

Vertex::Vertex(vector<int> faces, vector<int> edges){
	this->faces = faces;
	this->edges = edges;
}

void Vertex::displayVertex(){
	Point v = coord;
	glBegin(GL_POINT);
	glVertex3f(v.x, v.y, v.z);
	glEnd();
}

void Vertex::calVertexPoint(){
	Point f(0,0,0), r(0,0,0);
	int n = faces.size();
	for(int i= 0; i < n; i++){
		int index = faces[i];
		f = f + cc->allfaces[index].facePoint;
	}
	f.divide(n);
	int m = edges.size();
	for(int i =0; i < m; i++){
		int index = edges[i];
		int indexp1 = cc->alledges[index].vertexID1;
		int indexp2 = cc->alledges[index].vertexID2;
		r = r + (cc->allvertices[indexp1].coord + cc->allvertices[indexp2].coord)/2;
	}
	r.divide(m);
	vertexPoint = (f + r*2 + coord*(n-3))/n;
}

Vertex::~Vertex(){
}

CatmullClark::CatmullClark(){
	ccnext = NULL;
	ccfront = NULL;
}

CatmullClark * CatmullClark::subdivision(){
	vector<Face> allfacesNew;
	vector<Vertex> allverticesNew;
	CatmullClark * newCC = new CatmullClark();
	int id = 0;
	for(int i=0; i < allfaces.size(); i++){
		allfaces[i].calFacePoint();
		allverticesNew.push_back(Vertex(allfaces[i].facePoint, id, newCC));
		allfaces[i].PointID = id;
		id++;
	}
	for(int i=0; i < allvertices.size(); i++){
		allvertices[i].calVertexPoint();
		allverticesNew.push_back(Vertex(allvertices[i].vertexPoint, id, newCC));
		allvertices[i].PointID = id;
		id++;
	}
	for(int i=0; i < alledges.size(); i++){
		alledges[i].calEdgePoint();
		allverticesNew.push_back(Vertex(alledges[i].edgePoint, id, newCC));
		alledges[i].PointID = id;
		id++;
	}
	for(int i=0; i < allfaces.size(); i++){
		Face face = allfaces[i];
		for(int j=0; j < face.vertices.size(); j++){
			int vertexID = face.vertices[j];
			Edge foundEdges[2];
			int index = 0;
			for(int k=0; k < face.edges.size(); k++){
				Edge edge = alledges[face.edges[k]];
				if(edge.vertexID1 == vertexID || edge.vertexID2 == vertexID){
					foundEdges[index] = edge;
					index++;
				}
			}
			vector<int> vectorList;
			vectorList.push_back(face.PointID);
			vectorList.push_back(foundEdges[0].PointID);
			vectorList.push_back(allvertices[vertexID].PointID);
			vectorList.push_back(foundEdges[1].PointID);
			Face newface(allfacesNew.size());
			newface.cc = newCC;
			newface.vertices = vectorList;
			allfacesNew.push_back(newface);
		}
	}
	newCC->allfaces = allfacesNew;
	newCC->allvertices = allverticesNew;
	newCC->buildEdges();
	newCC->ccfront = this;
	ccnext = newCC;
	return newCC;
}

void CatmullClark::buildEdges(){
	for(int i=0; i < allfaces.size(); i++){
		int numFaceVertices = allfaces[i].vertices.size();
		for(int j = 0; j < numFaceVertices; j++){
			int edgeVertexIndex1 = allfaces[i].vertices[j];
			int edgeVertexIndex2 = allfaces[i].vertices[(j+1)%numFaceVertices];
			bool edgeExists = false;
			Vertex edgeVertex1 = allvertices[edgeVertexIndex1];
			Vertex edgeVertex2 = allvertices[edgeVertexIndex2];
			for(int k = 0; k < edgeVertex1.neighborVertices.size(); k++){
				if(edgeVertexIndex2 == edgeVertex1.neighborVertices[k]){
					edgeExists = true;
				}
			}
			if(edgeExists){
				for(int k = 0; k < alledges.size(); k++){
					if((alledges[k].vertexID1 == edgeVertexIndex1 && alledges[k].vertexID2 == edgeVertexIndex2)
						||(alledges[k].vertexID2 == edgeVertexIndex1 && alledges[k].vertexID1 == edgeVertexIndex2)){
							alledges[k].faceID2 = i;
							allfaces[i].edges.push_back(k);
							allvertices[edgeVertexIndex1].faces.push_back(i);

					}
				}
			} else {
				int edgeID = alledges.size();
				Edge newEdge(edgeID, edgeVertexIndex1, edgeVertexIndex2, i);
				newEdge.cc = this;
				allvertices[edgeVertexIndex1].neighborVertices.push_back(edgeVertexIndex2);
				allvertices[edgeVertexIndex2].neighborVertices.push_back(edgeVertexIndex1);
				allvertices[edgeVertexIndex1].edges.push_back(edgeID);
				allvertices[edgeVertexIndex2].edges.push_back(edgeID);
				allvertices[edgeVertexIndex1].faces.push_back(i);
				allfaces[i].edges.push_back(edgeID);
				alledges.push_back(newEdge);
			}
		}
	}
}
void CatmullClark::display(){

	for(int i=0; i < allvertices.size(); i++){
		allvertices[i].displayVertex();
	}
	for(int i=0; i < alledges.size(); i++){
		alledges[i].displayEdge();
	}
	/*
//	cout << allvertices[0].coord << endl;
	glColor3f(0.0, 0.0, 1.0);
	for(int i=0; i < allfaces.size(); i++){
		allfaces[i].displayFace();
	}
	*/
}

void CatmullClark::print(){
	for(int i=0; i < allvertices.size(); i++){
		cout << allvertices[i].coord << endl;
	}
	for(int i=0; i < alledges.size(); i++){
		cout << "edge" << i << endl;
		cout << allvertices[alledges[i].vertexID1].coord << endl;
		cout << allvertices[alledges[i].vertexID2].coord << endl;
	}
}

CatmullClark * readFile(char * fileName){
	ifstream input;
	input.open(fileName, ios::in);
	char s[LINE_LENGTH];
	string line;
	bool faceInput = false;
	vector<Face> allfaces;
	vector<Vertex> allvertices;
	vector<Edge> alledges;
	CatmullClark *cc = new CatmullClark();
	int allverticesIndex = 0;
	while(input.getline(s, LINE_LENGTH)){
		if(s[0] == '#') continue;
		if(s[0] == 'f'){
			faceInput = true;
		} else {
			if(!faceInput){ //input vertices
				Point *point = new Point();
				sscanf(s, "%f %f %f", &point->x, &point->y, &point->z);
				allvertices.push_back(Vertex(*point, allverticesIndex, cc));
				allverticesIndex++;
			} else { //input face
				int vertex_size = allvertices.size();
				vector<int> vertices;
				int bits = 0;
				bool store = false;
				for(int i=0; i < LINE_LENGTH; i++){
					if( s[i]<= '9' && s[i]>='0'){
						bits = bits*10 + s[i] - 48;
						store = true;
					} else if( bits >=0 && store ){
						vertices.push_back(bits);
						bits = 0;
						store = false;
						if( s[i] == '\0' ){
							break;
						}
					}
				}
				Face face(allfaces.size());
				face.vertices = vertices;
				face.cc = cc;
				allfaces.push_back(face);
			}
		}
	}
	cc->allvertices = allvertices;
	cc->allfaces = allfaces;
	cc->buildEdges();
	return cc;
}
