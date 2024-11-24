#include <GL/glut.h>
#include <iostream>
#include <cmath>

using namespace std;

int x1 = 0, y11 = 0; // Start point
int x2 = 0, y22 = 0; // End point
bool isFirstClick = true;

void plot(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

void drawLine() {
    int dx = abs(x2 - x1);
    int dy = abs(y22 - y11);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y11 < y22) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        plot(x1, y11);

        if (x1 == x2 && y11 == y22)
            break;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y11 += sy;
        }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0); // Red color
    drawLine();
    glFlush();
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0); // White background
    gluOrtho2D(-300, 300, -300, 300);
}

void MouseEventHandler(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        y = 600 - y; // Adjust y to match OpenGL coordinates
        x = x - 300; // Centering on the origin
        y = y - 300; // Centering on the origin
        
        if (isFirstClick) {
            x1 = x;
            y11 = y;
            isFirstClick = false;
        } else {
            x2 = x;
            y22 = y;
            display();
            isFirstClick = true; // Reset for next line
        }
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Midpoint Line Drawing Algorithm");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(MouseEventHandler);
    glutMainLoop();
    return 0;
}

