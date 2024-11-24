#include <GL/glut.h>
#include <iostream>
// Define the fill and boundary colors
float fillColor[3] = {0.0, 1.0, 0.0}; // Green
float boundaryColor[3] = {1.0, 0.0, 0.0}; // Red

void setPixel(int x, int y, float color[3]) {
    glBegin(GL_POINTS);
    glColor3fv(color);
    glVertex2i(x, y);
    glEnd();
    glFlush();
}

void getPixel(int x, int y, float color[3]) {
    glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, color);
}

void boundaryFill(int x, int y, float fillColor[3], float boundaryColor[3]) {
    float currentColor[3];
    getPixel(x, y, currentColor);

    if ((currentColor[0] != boundaryColor[0] || currentColor[1] != boundaryColor[1] || currentColor[2] != boundaryColor[2]) &&
        (currentColor[0] != fillColor[0] || currentColor[1] != fillColor[1] || currentColor[2] != fillColor[2])) {
        setPixel(x, y, fillColor);
        boundaryFill(x - 1, y, fillColor, boundaryColor);
        boundaryFill(x, y + 1, fillColor, boundaryColor);
        boundaryFill(x + 1, y, fillColor, boundaryColor);
        boundaryFill(x, y - 1, fillColor, boundaryColor);
    }   
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the boundary (a simple square)
    glColor3f(1.0, 0.0, 0.0); // Red
    glBegin(GL_LINE_LOOP);
    glVertex2i(100, 100);
    glVertex2i(200, 100);
    glVertex2i(200, 200);
    glVertex2i(100, 200);
    glEnd();

    glFlush();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        boundaryFill(x, 500 - y, fillColor, boundaryColor); // Assuming window height is 500
    }
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glColor3f(0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 500, 0, 500);
    glViewport(0, 0, 500, 500);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Recursive Boundary Fill Algorithm");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}
