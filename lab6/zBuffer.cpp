#include <GL/glut.h>
#include <vector>
#include <cmath>
#include <iostream>

// C?u tr�c d? luu tr? d?nh 3D
struct Vertex {
    float x, y, z;
    float r, g, b;
    Vertex(float _x, float _y, float _z, float _r, float _g, float _b)
        : x(_x), y(_y), z(_z), r(_r), g(_g), b(_b) {}
};

// C?u tr�c d? luu tr? m?t
struct Face {
    std::vector<int> vertexIndices;
};

// Bi?n to�n c?c
std::vector<Vertex> vertices;
std::vector<Face> faces;
float rotationX = 0.0f;
float rotationY = 0.0f;
bool wireframe = false;

// H�m kh?i t?o h�nh ch�p
void initializePyramid() {
    // X�a d? li?u cu
    vertices.clear();
    faces.clear();

    // Th�m c�c d?nh (4 d?nh d�y v� 1 d?nh ch�p)
    vertices.push_back(Vertex(-1, -1, -1, 1, 0, 0)); // �?nh d�y 1 (d?)
    vertices.push_back(Vertex(1, -1, -1, 0, 1, 0));  // �?nh d�y 2 (xanh l�)
    vertices.push_back(Vertex(1, -1, 1, 0, 0, 1));   // �?nh d�y 3 (xanh duong)
    vertices.push_back(Vertex(-1, -1, 1, 1, 1, 0));  // �?nh d�y 4 (v�ng)
    vertices.push_back(Vertex(0, 1, 0, 1, 0, 1));    // �?nh ch�p (t�m)

    // �?nh nghia c�c m?t (m?i m?t g?m 3 ho?c 4 d?nh)
    // M?t d�y
    faces.push_back(Face{{0, 1, 2, 3}});

    // C�c m?t b�n
    faces.push_back(Face{{0, 1, 4}});
    faces.push_back(Face{{1, 2, 4}});
    faces.push_back(Face{{2, 3, 4}});
    faces.push_back(Face{{3, 0, 4}});
}

// H�m v? c?nh
void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // X�a b? d?m m�u v� d? s�u

    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -5.0f);   // D?ch chuy?n m� h�nh ra xa
    glRotatef(rotationX, 1.0f, 0.0f, 0.0f); // Xoay quanh tr?c X
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f); // Xoay quanh tr?c Y

    // V? c�c m?t c?a h�nh ch�p
    for (const Face& face : faces) {
        if (wireframe) {
            glBegin(GL_LINE_LOOP);
        } else {
            glBegin(GL_POLYGON);
        }

        for (int idx : face.vertexIndices) {
            const Vertex& v = vertices[idx];
            glColor3f(v.r, v.g, v.b);  // M�u c?a d?nh
            glVertex3f(v.x, v.y, v.z); // T?a d? c?a d?nh
        }

        glEnd();
    }

    glPopMatrix();
    glutSwapBuffers(); // Ho�n d?i b? d?m d? hi?n th?
}

// X? l� c�c ph�m mui t�n d? xoay m� h�nh
void handleSpecialKeypress(int key, int x, int y) {
    if (key == GLUT_KEY_UP) rotationX -= 5.0f;
    if (key == GLUT_KEY_DOWN) rotationX += 5.0f;
    if (key == GLUT_KEY_LEFT) rotationY -= 5.0f;
    if (key == GLUT_KEY_RIGHT) rotationY += 5.0f;

    glutPostRedisplay();
}

// H�m kh?i t?o OpenGL
void initializeOpenGL() {
    glEnable(GL_DEPTH_TEST);  // K�ch ho?t ki?m tra d? s�u
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // M�u n?n

    // Thi?t l?p ma tr?n chi?u
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f); // G�c nh�n

    // Thi?t l?p ma tr?n nh�n
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(1.0f, 1.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f); // V? tr� viewpoint
}

// H�m ch�nh
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Z-buffer Demo");

    initializeOpenGL();
    initializePyramid();
    glutDisplayFunc(renderScene);
    glutSpecialFunc(handleSpecialKeypress);

    glutMainLoop();
    return 0;
}

