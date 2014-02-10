#include <iostream>
#include <GL\glut.h>
#include "Point.h"
#include "CatmullClark.h"
#define PI 3.1415926535898
using namespace std;

static int width = 600;
static int height = 600;
float camera_theta = 3*PI/2.0;
float camera_phi = 0.0;
float camera_r = 30.0;
CatmullClark * ccMesh;
CatmullClark * ccPrint;

void RenderScene (void)
{
  glClearColor(1.0, 1.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
//  glFrustum(-2.0, 2.0, -2.0, 2.0, 1.0, 50.0);
//  gluOrtho2D(0, width-2, height-2, 0);
  gluPerspective(90, width/height, 1, 80);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(camera_r*sin(camera_theta)*cos(camera_phi), camera_r*cos(camera_theta), camera_r*sin(camera_theta)*sin(camera_phi),
         0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
  glPushMatrix();
  glColor3f(0.0, 1.0, 1.0); 
  glLineWidth(1.0f);
//  glutSolidCube(1);
  ccPrint->display();
  glPopMatrix();
  glutSwapBuffers();
}

void ChangeSize(int w, int h){
	glViewport (0, 0, w, h);
	width = w;
	h = height;
}

void KeyboardKeys(unsigned char key, int x, int y)
{
  switch (key) {
  case 'z':
	  if(camera_r - 1.0/18.0 > 0)
         camera_r -= 1.0;
      break;
  case 'x':
      camera_r += 1.0;
      break;
  case 'l':
	  if(ccPrint->ccnext != NULL){
		ccPrint = ccPrint->ccnext;
	  } else {
	    ccPrint = ccPrint->subdivision();
	  }
	  break;
  case 'k':
	  if(ccPrint->ccfront != NULL){
		ccPrint = ccPrint->ccfront;
	  }
	  break;
  }
  cout << "camera_r: " << camera_r << endl;
  glutPostRedisplay();
}

void SpecialKeys(int key, int x, int y)
{
        switch(key) {
        case GLUT_KEY_LEFT:
                camera_theta -= PI/18.0;
                break;
        case GLUT_KEY_RIGHT:
                camera_theta += PI/18.0;
                break;
        case GLUT_KEY_UP:
                camera_phi += PI/18;
                break;
        case GLUT_KEY_DOWN:
                camera_phi -= PI/18;
                break;
        }
        glutPostRedisplay();
}

int main(int argc, char **argv){
	if (argc > 1){
		ccMesh =  readFile(argv[1]);
		ccPrint = ccMesh;
	} else {
		cout << "NO input file" << endl;
		exit(1);
	}
//	ccMesh->print();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Display and simplify Mesh");
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    glutSpecialFunc(SpecialKeys);
	glutKeyboardFunc(KeyboardKeys);
//    glutMouseFunc(MouseCallback);
//    glutMotionFunc(motion);
    glutMainLoop();
}
