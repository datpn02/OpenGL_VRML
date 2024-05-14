#include <GL/glut.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

using namespace std;

// Class representing the information of a 3D object
class Object3D {
public:
    virtual void draw() const = 0; // Make it a base class for polymorphism
    virtual ~Object3D() {} // Virtual destructor for polymorphism
};

class IndexedFaceSet : public Object3D {
public:
    vector<float> points;
    vector<int> coordIndex;

    void draw() const override {
        glBegin(GL_QUADS);
        for (size_t i = 0; i < coordIndex.size(); ++i) {
            if (coordIndex[i] == -1) {
                glEnd();
                glBegin(GL_QUADS);
            } else {
                int index = coordIndex[i] * 3;
                glVertex3f(points[index], points[index + 1], points[index + 2]);
            }
        }
        glEnd();
    }
};

vector<Object3D*> objects;

void readVRML(const string& filePath) {
    ifstream file(filePath.c_str());
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filePath << endl;
        return;
    }

    string line;
    stringstream ss;
    while (getline(file, line)) {
        ss << line << endl;
    }
    file.close();

    // Process VRML data
    string token;
    while (ss >> token) {
        if (token == "Transform") {
            // Read and process Transform node
            string transformToken;
            while (ss >> transformToken) {
                if (transformToken == "translation") {
                    float x, y, z;
                    ss >> x >> y >> z;
                    // Modify the translation values as needed
                    glTranslatef(x, y, z);
                } else if (transformToken == "IndexedFaceSet") {
                    // Read and process IndexedFaceSet node
                    IndexedFaceSet* faceSet = new IndexedFaceSet();
                    string coordToken;
                    while (ss >> coordToken) {
                        if (coordToken == "point") {
                            // Read and process Coordinate node
                            string pointToken;
                            while (ss >> pointToken) {
                                if (pointToken == "[") {
                                    float value;
                                    while (ss >> value) {
                                        if (value == ']') {
                                            break;
                                        }
                                        faceSet->points.push_back(value);
                                    }
                                }
                            }
                        } else if (coordToken == "coordIndex") {
                            // Read and process coordIndex array
                            int value;
                            while (ss >> value) {
                                if (value == -1) {
                                    break;
                                }
                                faceSet->coordIndex.push_back(value);
                            }
                        }
                    }
                    objects.push_back(faceSet);
                }
            }
        }
    }
}

void setupView(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)width / (double)height, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(0.0, 5.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    for (size_t i = 0; i < objects.size(); ++i) {
        const Object3D* obj = objects[i];
        obj->draw();
    }

    glutSwapBuffers();
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
    glutCreateWindow("VRML Viewer");
    glEnable(GL_DEPTH_TEST);
    setupView(800, 600);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);


    string filePath = "pyramid.wrl"; // Replace with your actual file path
    readVRML(filePath);
    if (objects.empty()) {
        cerr << "No objects to display. Exiting." << endl;
        return 1;
    }

    glutMainLoop();
    return 0;
}

