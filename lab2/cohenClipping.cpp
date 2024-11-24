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
struct Point {
    double x, y;
    Point(double x, double y) : x(x), y(y) {}
};
// Function to compute the region code for a point (x, y)
int computeCode(double x, double y) {
    int code = INSIDE;
    if (x < x_min) code |= LEFT;
    if (x > x_max) code |= RIGHT;
    if (y < y_min) code |= BOTTOM;
    if (y > y_max) code |= TOP;
    return code;
}

void cohenSutherlandClip(Point A, Point B, Point wmin, Point wmax) {
    int escape = 0, draw = 1;
    double m;

    while (escape == 0) {
        if ((computeCode(A.x, A.y) | computeCode(B.x, B.y)) == 0) {
            escape = 1;
        } else if ((computeCode(A.x, A.y) & computeCode(B.x, B.y)) != 0) {
            escape = 1;
            draw = 0;
        } else {
            if (computeCode(A.x, A.y) == 0) {
                std::swap(A, B);
            }
            if (A.x == B.x) {
                if (A.y > wmax.y) A.y = wmax.y;
                else A.y = wmin.y;
            } else {
                m = (double)(B.y - A.y) / (B.x - A.x);
                if (A.x < wmin.x) {
                    A.y = A.y + (wmin.x - A.x) * m;
                    A.x = wmin.x;
                } else if (A.x > wmax.x) {
                    A.y = A.y + (wmax.x - A.x) * m;
                    A.x = wmax.x;
                } else if (A.y < wmin.y) {
                    A.x = A.x + (wmin.y - A.y) / m;
                    A.y = wmin.y;
                } else if (A.y > wmax.y) {
                    A.x = A.x + (wmax.y - A.y) / m;
                    A.y = wmax.y;
                }
            }
        }
    }

    if (draw) {
        glColor3f(0.0, 1.0, 0.0);
        glBegin(GL_LINES);
        glVertex2d(A.x, A.y);
        glVertex2d(B.x, B.y);
        glEnd();
        glFlush();
    }
}

// Function to draw the clipping window
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the clipping window
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
            Point A(p1.first, p1.second);
            Point B(p2.first, p2.second);
            drawNormalLine(p1.first, p1.second, p2.first, p2.second);
            cohenSutherlandClip(A, B, Point(x_min, y_min), Point(x_max, y_max));
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
