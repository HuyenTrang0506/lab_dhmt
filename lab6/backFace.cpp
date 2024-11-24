#include <GL/glut.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include <string>
#include <iostream>

// Cau truc de luu tru dinh 3D
struct Vertex {
    float x, y, z;
    float r, g, b; 
    Vertex(float _x, float _y, float _z, float _r, float _g, float _b) 
        : x(_x), y(_y), z(_z), r(_r), g(_g), b(_b) {}
};

// Cau truc de luu tru mat
struct Face {
    std::vector<int> vertexIndices;
    float depth;
};

// Bien toan cuc
std::vector<Vertex> vertices;
std::vector<Face> faces;
float rotationX = 0.0f;
float rotationY = 0.0f;
int currentAlgorithm = 0; // 0: Back-face, 1: Z-buffer, 2: Painter's
bool wireframe = false;
float viewpoint[] = {1.0f, 1.0f, 5.0f}; 


void initializePyramid() {
  
    vertices.clear();
    faces.clear();

  
    vertices.push_back(Vertex(-1, -1, -1, 1, 0, 0)); // Dinh day 1 (do)
    vertices.push_back(Vertex(1, -1, -1, 0, 1, 0));  // Dinh day 2 (xanh la)
    vertices.push_back(Vertex(1, -1, 1, 0, 0, 1));   // Dinh day 3 (xanh duong)
    vertices.push_back(Vertex(-1, -1, 1, 1, 1, 0));  // Dinh day 4 (vang)
    vertices.push_back(Vertex(0, 1, 0, 1, 0, 1));    // Dinh chop (tim)

    
    faces.push_back(Face{{0, 1, 2, 3}, 0});

    
    faces.push_back(Face{{0, 1, 4}, 0});
    faces.push_back(Face{{1, 2, 4}, 0});
    faces.push_back(Face{{2, 3, 4}, 0});
    faces.push_back(Face{{3, 0, 4}, 0});
}


void calculateNormal(const Face& face, float* normal) {
  
    Vertex v1 = vertices[face.vertexIndices[0]];
    Vertex v2 = vertices[face.vertexIndices[1]];
    Vertex v3 = vertices[face.vertexIndices[2]];

   
    float u[3] = {v2.x - v1.x, v2.y - v1.y, v2.z - v1.z};
    float v[3] = {v3.x - v1.x, v3.y - v1.y, v3.z - v1.z};

  
    normal[0] = u[1] * v[2] - u[2] * v[1];
    normal[1] = u[2] * v[0] - u[0] * v[2];
    normal[2] = u[0] * v[1] - u[1] * v[0];
}

bool isBackFace(const Face& face) {
    float normal[3];
    calculateNormal(face, normal);


    Vertex v1 = vertices[face.vertexIndices[0]];
    float viewVector[3] = {v1.x - viewpoint[0], v1.y - viewpoint[1], v1.z - viewpoint[2]};

  
    float dotProduct = normal[0] * viewVector[0] + normal[1] * viewVector[1] + normal[2] * viewVector[2];
	 std::cout << "Dot Product: " << dotProduct << std::endl;
   
    return dotProduct < 0;
}

void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -5.0f); 
    glRotatef(rotationX, 1.0f, 0.0f, 0.0f); 
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f); 

   
    for (const Face& face : faces) {
        if (isBackFace(face)) continue;

        if (wireframe) {
            glBegin(GL_LINE_LOOP); 
        } else {
            glBegin(GL_POLYGON); 
        }

        for (int idx : face.vertexIndices) {
            const Vertex& v = vertices[idx];
            glColor3f(v.r, v.g, v.b); 
            glVertex3f(v.x, v.y, v.z); 
        }

        glEnd();
    }

    glPopMatrix();

    glutSwapBuffers();
}


void handleSpecialKeypress(int key, int x, int y) {
    if (key == GLUT_KEY_UP) rotationX -= 5.0f;
    if (key == GLUT_KEY_DOWN) rotationX += 5.0f;
    if (key == GLUT_KEY_LEFT) rotationY -= 5.0f;
    if (key == GLUT_KEY_RIGHT) rotationY += 5.0f;

    glutPostRedisplay();
}

void initializeOpenGL() {
    glEnable(GL_DEPTH_TEST);  
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 

    // Thiet lap ma tran chieu
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);  // Goc nhin

    // Thiet lap ma tran nhin
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(viewpoint[0], viewpoint[1], viewpoint[2], 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f); // Vi tri viewpoint
}


// Ham chinh
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Back-face Culling Demo");

    initializeOpenGL();
    initializePyramid();

    glutDisplayFunc(renderScene);
    glutSpecialFunc(handleSpecialKeypress);

    glutMainLoop();
    return 0;
}
