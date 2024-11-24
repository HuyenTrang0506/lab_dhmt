#include <GL/glut.h>
#include <cmath>
const double M_PI = 3.14159265358979323846; // Ð?nh nghia giá tr? p

GLfloat vertices[5][3] = {
    {0.0, 1.0, 0.0},   // Ð?nh A
    {0.0, 0.0, 1.0},   // Ð?nh B
    {1.0, 0.0, 1.0},   // Ð?nh C
    {2.0, 0.0, 0.0},   // Ð?nh D
    {0.0, 0.0, 0.0}    // Ð?nh E
};

GLfloat angleTheta = 0.0f; 
GLfloat anglePhi = 0.0f;  
GLfloat cameraDistance = 5.0f;
GLfloat zoom = 1.0f; 
bool isPerspective = true;
bool isParallel = false; 

// Bi?n góc quay
GLfloat angleX = 0.0f;
GLfloat angleY = 0.0f;
GLfloat angleZ = 0.0f;

void applyPerspectiveProjection(GLfloat vertex[3], GLfloat d) {
    if (vertex[2] != 0) {
        vertex[0] = (d * vertex[0]) / vertex[2];
        vertex[1] = (d * vertex[1]) / vertex[2];
    }
}

void multiplyMatrix(GLfloat vertex[3], GLfloat matrix[4][4]) {
    GLfloat result[3];
    for (int i = 0; i < 3; i++) {
        result[i] = matrix[i][0] * vertex[0] + matrix[i][1] * vertex[1] + matrix[i][2] * vertex[2] + matrix[i][3];
    }
    for (int i = 0; i < 3; i++) {
        vertex[i] = result[i];
    }
}

void rotateX(GLfloat angle) {
    GLfloat rad = angle * M_PI / 180.0f;
    GLfloat matrix[4][4] = {
        {1, 0, 0, 0},
        {0, cos(rad), -sin(rad), 0},
        {0, sin(rad), cos(rad), 0},
        {0, 0, 0, 1}
    };
    for (int i = 0; i < 5; i++) {
        multiplyMatrix(vertices[i], matrix);
    }
}

void rotateY(GLfloat angle) {
    GLfloat rad = angle * M_PI / 180.0f;
    GLfloat matrix[4][4] = {
        {cos(rad), 0, sin(rad), 0},
        {0, 1, 0, 0},
        {-sin(rad), 0, cos(rad), 0},
        {0, 0, 0, 1}
    };
    for (int i = 0; i < 5; i++) {
        multiplyMatrix(vertices[i], matrix);
    }
}

void rotateZ(GLfloat angle) {
    GLfloat rad = angle * M_PI / 180.0f;
    GLfloat matrix[4][4] = {
        {cos(rad), -sin(rad), 0, 0},
        {sin(rad), cos(rad), 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };
    for (int i = 0; i < 5; i++) {
        multiplyMatrix(vertices[i], matrix);
    }
}

void setPerspective(GLfloat fovY, GLfloat aspect, GLfloat nearZ, GLfloat farZ) {
    GLfloat f = 1.0f / tan(fovY * M_PI / 360.0f); // Ð? cao c?a lát c?t
    GLfloat matrix[4][4] = {
        {f / aspect, 0, 0, 0},
        {0, f, 0, 0},
        {0, 0, (farZ + nearZ) / (nearZ - farZ), -1},
        {0, 0, (2 * farZ * nearZ) / (nearZ - farZ), 0}
    };
    glMultMatrixf(&matrix[0][0]);
}

void setOrtho(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat nearZ, GLfloat farZ) {
    GLfloat matrix[4][4] = {
        {2 / (right - left), 0, 0, -(right + left) / (right - left)},
        {0, 2 / (top - bottom), 0, -(top + bottom) / (top - bottom)},
        {0, 0, -2 / (farZ - nearZ), -(farZ + nearZ) / (farZ - nearZ)},
        {0, 0, 0, 1}
    };
    glMultMatrixf(&matrix[0][0]);
}

void setParallel() {
    GLfloat projectionMatrix[4][4] = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 1}
    };
    
    for (int i = 0; i < 5; i++) {
        multiplyMatrix(vertices[i], projectionMatrix);
    }
}


void drawPyramid() {
    // M?t 1: (A, B, C)
    glBegin(GL_POLYGON);
        glColor3f(1.0, 0.0, 0.0); // Màu d?
        glVertex3fv(vertices[0]);
        glVertex3fv(vertices[1]);
        glVertex3fv(vertices[2]);
    glEnd();

    // M?t 2: (A, C, D)
    glBegin(GL_POLYGON);
        glColor3f(0.0, 1.0, 0.0); // Màu xanh lá
        glVertex3fv(vertices[0]);
        glVertex3fv(vertices[2]);
        glVertex3fv(vertices[3]);
    glEnd();

    // M?t 3: (A, D, E)
    glBegin(GL_POLYGON);
        glColor3f(0.0, 0.0, 1.0); // Màu xanh duong
        glVertex3fv(vertices[0]);
        glVertex3fv(vertices[3]);
        glVertex3fv(vertices[4]);
    glEnd();

    // M?t 4: (A, E, B)
    glBegin(GL_POLYGON);
        glColor3f(1.0, 1.0, 0.0); // Màu vàng
        glVertex3fv(vertices[0]);
        glVertex3fv(vertices[4]);
        glVertex3fv(vertices[1]);
    glEnd();

    // M?t dáy: (B, C, D, E)
    glBegin(GL_POLYGON);
        glColor3f(0.5, 0.5, 0.5); // Màu xám
        glVertex3fv(vertices[1]);
        glVertex3fv(vertices[2]);
        glVertex3fv(vertices[3]);
        glVertex3fv(vertices[4]);
    glEnd();
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0); // N?n tr?ng
    glEnable(GL_DEPTH_TEST);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(cameraDistance * cos(anglePhi) * cos(angleTheta), 
              cameraDistance * sin(anglePhi), 
              cameraDistance * cos(anglePhi) * sin(angleTheta), 
              0.0, 0.0, 0.0, 
              0.0, 1.0, 0.0);

    glScalef(zoom, zoom, zoom);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (isPerspective) {
        setPerspective(45.0, 1.0, 1.0, 100.0);
    } else {
        setOrtho(-2.0, 2.0, -2.0, 2.0, 1.0, 100.0);
    }

    glMatrixMode(GL_MODELVIEW);
    drawPyramid();
    glutSwapBuffers();
}


void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / (float)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            anglePhi += 0.05f;
            break;
        case GLUT_KEY_DOWN:
            anglePhi -= 0.05f;
            break;
        case GLUT_KEY_LEFT:
            angleTheta -= 0.05f;
            break;
        case GLUT_KEY_RIGHT:
            angleTheta += 0.05f;
            break;
    }
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'x':
            rotateX(5.0f);
            break;
        case 'y':
            rotateY(5.0f);
            break;
        case 'z':
            rotateZ(5.0f);
            break;
        case 'i':
            cameraDistance -= 0.1f;
            break;
        case 'o':
            cameraDistance += 0.1f;
            break;
        case '-':
            zoom -= 0.1f;
            break;
        case '+':
            zoom += 0.1f;
            break;
        case 'p':
            isPerspective = !isPerspective;
            break;
        case 'q':  
            setParallel();
            break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("3D Pyramid with Perspective Projection");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(specialKeys);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
