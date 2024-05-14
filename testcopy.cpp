#include <GL/glut.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <iostream>
using namespace std;

float cameraAngleY = 0.0f;
float angleIncrement = 1.0f;

// Class representing the information of a 3D object
class Object3D {
public:
    float bottomRadius;
    float height;
    Object3D(float br, float h) : bottomRadius(br), height(h) {}
};
vector<Object3D> objects;
// Read node
void readNode(stringstream& ss) {
    string token;
    while (ss >> token) {
        if (token == "Cone") {
            float bottomRadius, height;
            bool radiusFound = false, heightFound = false;
            // Skip '{' character following "Cone"
            if (ss.peek() == '{') {
                ss.ignore();
            }
            while (ss >> token) {
                if (token == "bottomRadius") {
                    ss >> bottomRadius;
                    radiusFound = true;
                } else if (token == "height") {
                    ss >> height;
                    heightFound = true;
                } else if (token == "}") {
                    // Cone node ends with '}'
                    break;
                }
            }
            if (radiusFound && heightFound) {
                objects.push_back(Object3D(bottomRadius, height));
            }
        } else if (token == "Sphere") {
            float radius;
            bool radiusFound = false;
            // Skip '{' character following "Sphere"
            if (ss.peek() == '{') {
                ss.ignore();
            }
            while (ss >> token) {
                if (token == "radius") {
                    ss >> radius;
                    radiusFound = true;
                } else if (token == "}") {
                    // Sphere node ends with '}'
                    break;
                }
            }
            if (radiusFound) {
                // Assuming your Object3D class supports a sphere
                objects.push_back(Object3D(radius, radius * 2.0));
            }
        } else if (token == "Cylinder") {
            float radius, height;
            bool radiusFound = false, heightFound = false;
            // Skip '{' character following "Cylinder"
            if (ss.peek() == '{') {
                ss.ignore();
            }
            while (ss >> token) {
                if (token == "radius") {
                    ss >> radius;
                    radiusFound = true;
                } else if (token == "height") {
                    ss >> height;
                    heightFound = true;
                } else if (token == "}") {
                    // Cylinder node ends with '}'
                    break;
                }
            }
            if (radiusFound && heightFound) {
                objects.push_back(Object3D(radius, height));
            }
        }
    }
}

// Read VRML file
void readVRML(const string& filePath) {
    ifstream file(filePath.c_str());
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filePath << endl;
        return;
    }
    stringstream ss;
    ss << file.rdbuf();
    file.close();
    readNode(ss);
}
void setupView(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)width / (double)height, 0.1, 100.0); // Thay d?i góc nhìn
    glMatrixMode(GL_MODELVIEW);
}

//void drawGrid(int size, int step) {
//    glColor3f(0.5, 0.5, 0.5);
//    for (int i = -size; i <= size; i += step) {
//        glBegin(GL_LINES);
//        glVertex3f(i, 0, -size);
//        glVertex3f(i, 0, size);
//        glEnd();
//
//        glBegin(GL_LINES);
//        glVertex3f(-size, 0, i);
//        glVertex3f(size, 0, i);
//        glEnd();
//    }
//}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(0.0, 0.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glRotatef(cameraAngleY, 0.0, 1.0, 0.0); // Xoay camera quanh tr?c y

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glClearColor(1.0, 1.0, 1.0, 1.0);  // Màu n?n tr?ng

    //drawGrid(10, 1); // V? lu?i v?i kích thu?c 10 và bu?c 1

    // Draw objects using OpenGL
    for (size_t i = 0; i < objects.size(); ++i) {
        const Object3D& obj = objects[i];
        glColor3f(1.0, 1.0, 0.0); // Set the color to yellow
        glutSolidCone(obj.bottomRadius, obj.height, 20, 20); // Changed to solid cone for better visibility
    }

    glutSwapBuffers();
}



void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'a':
            cameraAngleY -= angleIncrement;
            break;
        case 'd':
            cameraAngleY += angleIncrement;
            break;
        case 27: // Phím Esc d?ng
            exit(0);
            break;
    }

    glutPostRedisplay(); // G?i hàm display d? v? l?i c?nh
}


// Reshape callback
void reshape(int w, int h) {
    setupView(w, h);
}
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL VRML Viewer");
    glEnable(GL_DEPTH_TEST);
    setupView(800, 600);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    string filePath = "D:\\A BT CODE\\Code Dev C++\\C_OpenGl\\VRML\\yellowcone.wrl";
    readVRML(filePath);
    if (objects.empty()) {
        cerr << "No objects to display. Exiting." << endl;
        return 1;
    }
    glutMainLoop();
    return 0;
}
