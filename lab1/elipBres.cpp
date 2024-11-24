#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <cstdlib>


// Bi?n toàn c?c
int centerX, centerY;
int radiusX, radiusY;
bool isCenterSet = false;
bool isMajorAxisSet = false;

// Hàm v? ellipse theo thu?t toán Bresenham
void EllipseBres(int xc, int yc, int a, int b) {
    int x = 0, y = b;
    int a2 = a * a, b2 = b * b;
    int d1 = b2 - a2 * b + 0.25 * a2;

    glBegin(GL_POINTS);

    // Vùng 1
    while (b2 * x <= a2 * y) {
        glVertex2i(xc + x, yc + y);
        glVertex2i(xc - x, yc + y);
        glVertex2i(xc + x, yc - y);
        glVertex2i(xc - x, yc - y);

        if (d1 < 0) {
            d1 += b2 * (2 * x + 3);
        } else {
            d1 += b2 * (2 * x + 3) + a2 * (-2 * y + 2);
            y--;
        }
        x++;
    }

    // Vùng 2
    int d2 = b2 * (x + 0.5) * (x + 0.5) + a2 * (y - 1) * (y - 1) - a2 * b2;
    while (y >= 0) {
        glVertex2i(xc + x, yc + y);
        glVertex2i(xc - x, yc + y);
        glVertex2i(xc + x, yc - y);
        glVertex2i(xc - x, yc - y);

        if (d2 < 0) {
            d2 += b2 * (2 * x + 2) + a2 * (-2 * y + 3);
            x++;
        } else {
            d2 += a2 * (-2 * y + 3);
        }
        y--;
    }

    glEnd();
}

// Hàm x? lý s? ki?n chu?t
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Chuy?n t?a d? chu?t t? h? t?a d? c?a s? sang h? t?a d? OpenGL
        int winHeight = glutGet(GLUT_WINDOW_HEIGHT);
        y = winHeight - y;

        if (!isCenterSet) {
            centerX = x;
            centerY = y;
            isCenterSet = true;
            std::cout << "Center set at: (" << centerX << ", " << centerY << ")" << std::endl;
        } else if (!isMajorAxisSet) {
            radiusX = abs(x - centerX);
            isMajorAxisSet = true;
            std::cout << "Major axis set with length: " << radiusX << std::endl;
        } else {
            radiusY = abs(y - centerY);
            std::cout << "Minor axis set with length: " << radiusY << std::endl;
            glutPostRedisplay(); // Yêu c?u v? l?i c?a s? d? hi?n th? ellipse
        }
    }
}

// Hàm hi?n th? (render) trong OpenGL
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (isCenterSet && isMajorAxisSet && radiusY != 0) {
        glColor3f(1.0, 0.0, 1.0); // Màu MAGENTA
        EllipseBres(centerX, centerY, radiusX, radiusY);
    }

    glFlush();
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0); // Màu n?n là den
    glColor3f(1.0, 1.0, 1.0);         // Màu v? m?c d?nh là tr?ng
    glPointSize(1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 500, 0, 500);       // Thi?t l?p h? t?a d? 2D
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Bresenham Ellipse Drawing with Mouse");
    
    init();
    
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMainLoop();
    
    return 0;
}

