#include <GL/glut.h>
#include <openvrml/browser.h>
#include <openvrml/util/vrmlreader.h>

using namespace openvrml;
using namespace std;

Node* scene; 

void init() {

  VRMLreader reader;
  scene = reader.parse("yellowcone.wrl");

}

void display() {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  scene->render();

  glutSwapBuffers();

}

int main(int argc, char** argv) {

  glutInit(&argc, argv);

  init();

  glutDisplayFunc(display);

  glutMainLoop();

  return 0;

}
