//#include <GL/glut.h>
//#include <fstream>
//#include <sstream>
//#include <vector>
//#include <cmath>
//
//using namespace std;
//
//// L?p d?i di?n cho thông tin c?a v?t th? 3D
//class Object3D {
//public:
//    float bottomRadius;
//    float height;
//
//    Object3D(float br, float h) : bottomRadius(br), height(h) {}
//};
//
//vector<Object3D> objects;
//
//// Ð?c node
//void readNode(stringstream& ss) {
//    string line;
//    while (getline(ss, line)) {
//        if (line.find("Shape") != string::npos) {
//            // Ð?c thông tin d?i tu?ng 3D
//            float bottomRadius, height;
//            while (getline(ss, line) && line.find("geometry Cone") == string::npos) {
//                if (line.find("bottomRadius") != string::npos) {
//                    sscanf(line.c_str(), "%*s %f", &bottomRadius);
//                }
//                else if (line.find("height") != string::npos) {
//                    sscanf(line.c_str(), "%*s %f", &height);
//                }
//            }
//            objects.push_back(Object3D(bottomRadius, height));
//        }
//    }
//}
//
//// Ð?c file VRML
//void readVRML(const string& filePath) {
//    ifstream file(filePath.c_str());
//    stringstream ss;
//    ss << file.rdbuf();
//    file.close();
//
//    readNode(ss);
//}
//
//// Hàm hi?n th?
//void display() {
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    glLoadIdentity();
//    gluLookAt(0.0, 0.0, 15.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
//
//    // V? d?i tu?ng b?ng OpenGL
//    for (size_t i = 0; i < objects.size(); ++i) {
//        const Object3D& obj = objects[i];
//        glColor3f(1.0, 1.0, 0.0);
//        glutWireCone(obj.bottomRadius, obj.height, 20, 20);
//    }
//
//    glutSwapBuffers();
//}
//
//int main(int argc, char** argv) {
//    glutInit(&argc, argv);
//    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
//    glutCreateWindow("OpenGL VRML Viewer");
//
//    glEnable(GL_DEPTH_TEST);
//
//    readVRML("D:\\A BT CODE\\Code Dev C++\\C_OpenGl\\VRML\\yellowcone.wrl");
//
//    glutDisplayFunc(display);
//    glutMainLoop();
//
//    return 0;
//}

