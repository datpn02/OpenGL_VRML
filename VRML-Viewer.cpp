#include <GL/glut.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <iostream>
using namespace std;

float cameraAngleX = 0.0f;
float cameraAngleY = 0.0f;
float angleIncrement = 1.0f;

// Class representing the information of a 3D object
class Object3D {
public:
    virtual void draw() const = 0; // Make it a base class for polymorphism
    virtual ~Object3D() {} // Virtual destructor for polymorphism
};

class Box : public Object3D {
public:
    float width, height, depth;
    Box(float w, float h, float d) : width(w), height(h), depth(d) {}

    void draw() const override {
        glPushMatrix();
        glScalef(width, height, depth); // Scale cube to desired dimensions
        glutSolidCube(1.0); // GLUT provides a simple cube primitive at unit size
        glPopMatrix();
    }
};

class Cone : public Object3D {
public:
    float bottomRadius;
    float height;
    Cone(float br, float h) : bottomRadius(br), height(h) {}

    void draw() const override {
        glColor3f(1.0, 1.0, 1.0); // Set the color to yellow
        glutSolidCone(bottomRadius, height, 20, 20);
    }
};

class Sphere : public Object3D {
public:
    float radius;
    Sphere(float r) : radius(r) {}

    void draw() const override {
        glColor3f(1.0, 1.0, 1.0); // Set the color to yellow
        glutSolidSphere(radius, 20, 20);
    }
};

class Cylinder : public Object3D {
public:
    float radius;
    float height;
    Cylinder(float r, float h) : radius(r), height(h) {}

    void draw() const override {
        glColor3f(1.0, 1.0, 1.0); // Set the color to yellow
        glutSolidCone(radius, height, 20, 20);
        glTranslatef(0.0, 0.0, height); // Move to the top of the cone
        glutSolidSphere(radius, 20, 20); // Draw a sphere to close the top
    }
};

vector<Object3D*> objects;

// Read node
void readNode(stringstream& ss) {
    string token;
    while (ss >> token) {
        if (token == "Box") {
            float w = 1, h = 1, d = 1; // Gi? s? giá tr? m?c d?nh
            string subToken;
            while (ss >> subToken) {
                if (subToken == "size") {
                    ss >> w >> h >> d;
                    break;
                }
            }
            objects.push_back(new Box(w, h, d));
        } else if (token == "Cone") {
            float bottomRadius, height;
            bool radiusFound = false, heightFound = false;
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
                    break;
                }
            }
            if (radiusFound && heightFound) {
                objects.push_back(new Cone(bottomRadius, height));
            }
        } else if (token == "Sphere") {
            float radius;
            bool radiusFound = false;
            if (ss.peek() == '{') {
                ss.ignore();
            }
            while (ss >> token) {
                if (token == "radius") {
                    ss >> radius;
                    radiusFound = true;
                } else if (token == "}") {
                    break;
                }
            }
            if (radiusFound) {
                objects.push_back(new Sphere(radius));
            }
        } else if (token == "Cylinder") {
            float radius, height;
            bool radiusFound = false, heightFound = false;
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
                    break;
                }
            }
            if (radiusFound && heightFound) {
                objects.push_back(new Cylinder(radius, height));
            }
        }
    }
}

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
    gluPerspective(60.0, (double)width / (double)height, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void drawGrid(int size, int step) {
    glColor3f(1.0, 1.0, 1.0);  // Set the color to white
    for (int i = -size; i <= size; i += step) {
        glBegin(GL_LINES);
        glVertex3f(i, 0, -size);
        glVertex3f(i, 0, size);
        glEnd();

        glBegin(GL_LINES);
        glVertex3f(-size, 0, i);
        glVertex3f(size, 0, i);
        glEnd();
    }
}

float zoomFactor = 1.0f; // T? l? zoom m?c d?nh

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(0.0, 0.0, 20.0 / zoomFactor, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glRotatef(cameraAngleX, 1.0, 0.0, 0.0);
    glRotatef(cameraAngleY, 0.0, 1.0, 0.0);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    drawGrid(10, 1);

    for (size_t i = 0; i < objects.size(); ++i) {
        const Object3D* obj = objects[i];

        glPushMatrix();
        glScalef(zoomFactor, zoomFactor, zoomFactor);
        obj->draw();  // Use polymorphism to call the correct draw method
        glPopMatrix();
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
        case 'w':  // Quay lên
            cameraAngleX += angleIncrement;
            break;
        case 's':  // Quay xu?ng
            cameraAngleX -= angleIncrement;
            break;
        case '+': // Phím '+' d? zoom in
            zoomFactor += 0.1f;
            break;
        case '-': // Phím '-' d? zoom out
            zoomFactor -= 0.1f;
            if (zoomFactor < 0.1f) {
                zoomFactor = 0.1f;
            }
            break;
        case 27:  // ESC key
            exit(0);
            break;
    }
    glutPostRedisplay();
}

void reshape(int w, int h) {
    setupView(w, h);
}

void cleanup() {
    for (size_t i = 0; i < objects.size(); ++i) {
        delete objects[i];
    }
    objects.clear();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL VRML Viewer");
    glEnable(GL_DEPTH_TEST);
    setupView(800, 600);
    glClearColor(1.0, 1.0, 1.0, 1.0);  // R, G, B, A
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    atexit(cleanup);

    string filePath = "shape.wrl";
    readVRML(filePath);
    if (objects.empty()) {
        cerr << "No objects to display. Exiting." << endl;
        return 1;
    }

    glutMainLoop();
    return 0;
}

