#include <GL/glut.h>
#include <cmath>
#include <iostream>

int x1, y1, x2, y2;
float a;
bool firstClick = true;

void print_pixel(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

void parabola_bresenham_horizontal(int x1, int y1, float a) {
    float d = 1 - 2 * a, x = 0, y = 0;
    while (y <= 2 * a) {
        print_pixel(x1 + x, y1 + y);
        print_pixel(x1 + x, y1 - y);
        if (d < 0) {
            d += 3 + 2 * y;
        } else {
            d += 5 + 2 * y - 4 * a;
            x++;
        }
        y++;
    }
    d = 1 / 4 - 2 * a;
    while (x <= 2 * a) {
        print_pixel(x1 + x, y1 + y);
        print_pixel(x1 + x, y1 - y);
        if (d > 0) {
            d += -4 * a;
        } else {
            d += 2 + 2 * y - 4 * a;
            y++;
        }
        x++;
    }
}

void parabola_bresenham_vertical(int x1, int y1, float a) {
    float d = 4 - 8 * a, x = 0, y = 0;
    while (y <= 2 * a) {
        print_pixel(x1 + x, y1 + y);
        print_pixel(x1 - x, y1 + y);
        if (d < 0) {
            d += 2 * (4 * y + 6);
        } else {
            d += 2 * (4 * y - 4 * a + 6);
            x++;
        }
        y++;
    }
    d = 1 - 8 * a;
    while (x <= 2 * a) {
        print_pixel(x1 + x, y1 + y);
        print_pixel(x1 - x, y1 + y);
        if (d > 0) {
            d += -16 * a;
        } else {
            d += 4 * (2 * x - 4 * a + 2);
            y++;
        }
        x++;
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0, 1.0, 1.0);  // Set the color to white

    float dx = x2 - x1;
    float dy = y2 - y1;
    if (fabs(dx) > fabs(dy)) {
        // Horizontal parabola
        parabola_bresenham_horizontal(x1, y1, a);
    } else {
        // Vertical parabola
        parabola_bresenham_vertical(x1, y1, a);
    }

    glutSwapBuffers();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (firstClick) {
            x1 = x;
            y1 = glutGet(GLUT_WINDOW_HEIGHT) - y;
            firstClick = false;
        } else {
            x2 = x;
            y2 = glutGet(GLUT_WINDOW_HEIGHT) - y;
            float dx = x2 - x1;
            float dy = y2 - y1;
            a = sqrt(pow(dx, 2) + pow(dy, 2));
            firstClick = true;
            glutPostRedisplay();
        }
    }
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glColor3f(1.0, 1.0, 1.0);
    glPointSize(1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 800.0, 0.0, 600.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Draw Parabola");

    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);

    glutMainLoop();
    return 0;
}

