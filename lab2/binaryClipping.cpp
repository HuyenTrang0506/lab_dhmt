#include <GL/glut.h>
#include <vector>
#include <iostream>

// Constants for window dimensions
const int WINDOW_WIDTH = 500;
const int WINDOW_HEIGHT = 500;

// Constants for the clipping window
const int x_min = 50;
const int y_min = 50;
const int x_max = 300;
const int y_max = 300;

// Constants for region codes
const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000
std::vector<std::pair<int, int>> points;

// Function to compute the region code for a point (x, y)
int computeCode(double x, double y) {
    int code = INSIDE;
    if (x < x_min) code |= LEFT;
    if (x > x_max) code |= RIGHT;
    if (y < y_min) code |= BOTTOM;
    if (y > y_max) code |= TOP;
    return code;
}

// Binary Subdivision clipping algorithm
void binarySubdivisionClip(double x1, double y1, double x2, double y2) {
    int code1 = computeCode(x1, y1);
    int code2 = computeCode(x2, y2);
    
    std::cout << "Clipping line from (" << x1 << ", " << y1 << ") to (" << x2 << ", " << y2 << ")" << std::endl;

    if ((code1 == 0) && (code2 == 0)) {
        
        glColor3f(0.0, 1.0, 0.0);   
        glBegin(GL_LINES);
        glVertex2d(x1, y1);
        glVertex2d(x2, y2);
        glEnd();
        glFlush();
    } else if (code1 & code2) {
        
        return;
    } else {
       
        double x_mid = (x1 + x2) / 2;
        double y_mid = (y1 + y2) / 2;

       
        if ((x1 == x_mid && y1 == y_mid) || (x2 == x_mid && y2 == y_mid)) {
            return;
        }

        binar ySubdivisionClip(x1, y1, x_mid, y_mid);
        binarySubdivisionClip(x_mid, y_mid, x2, y2);
    }
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT);

  
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_QUADS);
    glVertex2d(x_min, y_min);
    glVertex2d(x_max, y_min);
    glVertex2d(x_max, y_max);
    glVertex2d(x_min, y_max);
    glEnd();

    glFlush();
}

// Function to draw a normal line
void drawNormalLine(double x1, double y1, double x2, double y2) {
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    glVertex2d(x1, y1);
    glVertex2d(x2, y2);
    glEnd();
    glFlush();
}

// Mouse callback function
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        points.emplace_back(x, WINDOW_HEIGHT - y); // Convert from window to OpenGL coordinates
        if (points.size() % 2 == 0) {
            auto& p1 = points[points.size() - 2];
            auto& p2 = points[points.size() - 1];
            drawNormalLine(p1.first, p1.second, p2.first, p2.second);
            binarySubdivisionClip(p1.first, p1.second, p2.first, p2.second);
        }
    }
}

// Initialization function
void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT);
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Line Drawing with Mouse Clicks");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}
