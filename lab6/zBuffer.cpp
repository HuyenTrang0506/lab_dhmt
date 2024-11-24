#include <GL/glut.h>
#include <vector>
#include <cmath>
#include <iostream>

// C?u trúc d? luu tr? d?nh 3D
struct Vertex {
    float x, y, z;
    float r, g, b;
    Vertex(float _x, float _y, float _z, float _r, float _g, float _b)
        : x(_x), y(_y), z(_z), r(_r), g(_g), b(_b) {}
};

// C?u trúc d? luu tr? m?t
struct Face {
    std::vector<int> vertexIndices;
};

// Bi?n toàn c?c
std::vector<Vertex> vertices;
std::vector<Face> faces;
float rotationX = 0.0f;
float rotationY = 0.0f;
bool wireframe = false;

// Hàm kh?i t?o hình chóp
void initializePyramid() {
    // Xóa d? li?u cu
    vertices.clear();
    faces.clear();

    // Thêm các d?nh (4 d?nh dáy và 1 d?nh chóp)
    vertices.push_back(Vertex(-1, -1, -1, 1, 0, 0)); // Ð?nh dáy 1 (d?)
    vertices.push_back(Vertex(1, -1, -1, 0, 1, 0));  // Ð?nh dáy 2 (xanh lá)
    vertices.push_back(Vertex(1, -1, 1, 0, 0, 1));   // Ð?nh dáy 3 (xanh duong)
    vertices.push_back(Vertex(-1, -1, 1, 1, 1, 0));  // Ð?nh dáy 4 (vàng)
    vertices.push_back(Vertex(0, 1, 0, 1, 0, 1));    // Ð?nh chóp (tím)

    // Ð?nh nghia các m?t (m?i m?t g?m 3 ho?c 4 d?nh)
    // M?t dáy
    faces.push_back(Face{{0, 1, 2, 3}});

    // Các m?t bên
    faces.push_back(Face{{0, 1, 4}});
    faces.push_back(Face{{1, 2, 4}});
    faces.push_back(Face{{2, 3, 4}});
    faces.push_back(Face{{3, 0, 4}});
}

// Hàm v? c?nh
void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Xóa b? d?m màu và d? sâu

    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -5.0f);   // D?ch chuy?n mô hình ra xa
    glRotatef(rotationX, 1.0f, 0.0f, 0.0f); // Xoay quanh tr?c X
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f); // Xoay quanh tr?c Y

    // V? các m?t c?a hình chóp
    for (const Face& face : faces) {
        if (wireframe) {
            glBegin(GL_LINE_LOOP);
        } else {
            glBegin(GL_POLYGON);
        }

        for (int idx : face.vertexIndices) {
            const Vertex& v = vertices[idx];
            glColor3f(v.r, v.g, v.b);  // Màu c?a d?nh
            glVertex3f(v.x, v.y, v.z); // T?a d? c?a d?nh
        }

        glEnd();
    }

    glPopMatrix();
    glutSwapBuffers(); // Hoán d?i b? d?m d? hi?n th?
}

// X? lý các phím mui tên d? xoay mô hình
void handleSpecialKeypress(int key, int x, int y) {
    if (key == GLUT_KEY_UP) rotationX -= 5.0f;
    if (key == GLUT_KEY_DOWN) rotationX += 5.0f;
    if (key == GLUT_KEY_LEFT) rotationY -= 5.0f;
    if (key == GLUT_KEY_RIGHT) rotationY += 5.0f;

    glutPostRedisplay();
}

// Hàm kh?i t?o OpenGL
void initializeOpenGL() {
    glEnable(GL_DEPTH_TEST);  // Kích ho?t ki?m tra d? sâu
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Màu n?n

    // Thi?t l?p ma tr?n chi?u
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f); // Góc nhìn

    // Thi?t l?p ma tr?n nhìn
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(1.0f, 1.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f); // V? trí viewpoint
}

// Hàm chính
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

