#include <GL/glut.h>
#include <iostream>
using namespace std;

int centerx = 0, centery = 0;
int a = 0;
int b = 0;



void plot(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(centerx + x, centery + y);
    glVertex2i(centerx - x, centery + y);
    glVertex2i(centerx + x, centery - y);
    glVertex2i(centerx - x, centery - y);
    glEnd();
}

void drawEllipse() {
    int x = 0;
    int y = b;

    float d1 = (b * b) - (a * a * b) + (0.25 * a * a);
    int dx = 2 * b * b * x;
    int dy = 2 * a * a * y;

    while (dx < dy) {
        plot(x, y);

        if (d1 < 0) {
            x++;
            dx = dx + (2 * b * b);
            d1 = d1 + dx + (b * b);
        } else {
            x++;
            y--;
            dx = dx + (2 * b * b);
            dy = dy - (2 * a * a);
            d1 = d1 + dx - dy + (b * b);
        }
    }

    float d2 = ((b * b) * ((x + 0.5) * (x + 0.5))) + ((a * a) * ((y - 1) * (y - 1))) - (a * a * b * b);

    while (y >= 0) {
        plot(x, y);

        if (d2 > 0) {
            y--;
            dy = dy - (2 * a * a);
            d2 = d2 + (a * a) - dy;
        } else {
            y--;
            x++;
            dx = dx + (2 * b * b);
            dy = dy - (2 * a * a);
            d2 = d2 + dx - dy + (a * a);
        }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);
    drawEllipse();
    glFlush();
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glColor3f(1.0, 0.0, 0.0);
    gluOrtho2D(-300, 300, -300, 300);
}

void MouseEventHandler(int button, int state, int x, int y)
{
	if(button == GLUT_LEFT_BUTTON && state ==  GLUT_UP)
	{   		
		y = 600 - y;  
		if(centerx == 0 && centery == 0) {
			centerx = x-300;
			centery = y-300;
			return;
		}

        if(a==0) {
			a = x-300;
			return;
		}
		if(b==0) {
			b = y-300;
			display();
		}

	}
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Midpoint Ellipse Drawing Algorithm");
    init();
    glutDisplayFunc(display);
	glutMouseFunc(MouseEventHandler);

    glutMainLoop();
    return 0;
}
