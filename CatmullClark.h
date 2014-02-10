#include <vector>
#include "Point.h"
using namespace std;

class CatmullClark;

class Edge
{
public:
	int id;
	int vertexID1;
	int vertexID2;
	int faceID1;
	int faceID2;
	CatmullClark * cc;
	Point edgePoint;
	int PointID;
	Edge(int id = -1, int vertexID1 = -1, int vertexID2 = -1, int faceID1 = -1, int faceID2 = -1);
	void displayEdge();
	void calEdgePoint();
	~Edge();
};

class Face{
public:
	int id;
	vector<int> vertices;
	vector<int> edges;
	CatmullClark * cc;
	Point facePoint;
	int PointID;
	Face(int id);
	Face(int id, vector<int> vertices, vector<int> edges);
	void displayFace();
	void calFacePoint();
	~Face();
};

class Vertex{
public:
	int id;
	Point coord;
	vector<int> faces;
	vector<int> edges;
	vector<int> neighborVertices;
	CatmullClark * cc;
	Point vertexPoint;
	int PointID;
	Vertex(Point coord, int index, CatmullClark * cc);
	Vertex(vector<int> faces, vector<int> edges);
	void displayVertex();
	void calVertexPoint();
	~Vertex();
};

class CatmullClark{
public:
	vector<Face> allfaces;
	vector<Vertex> allvertices;
	vector<Edge> alledges;
	CatmullClark * ccnext;
	CatmullClark * ccfront;
	CatmullClark * subdivision();
	CatmullClark();
	void buildEdges();
	void display();
	void print();
};

CatmullClark * readFile(char * filename);
