#include <GL/glut.h>
#include <cmath>
#include <iostream>

// Define the triangle vertices
struct Point {
	float x, y;
};

Point A = {100, 100};
Point B = {200, 100};
Point C = {150, 200};

// Line equation parameters
float a = 1.0f, b = 0.0f;

// Function to reflect a point across the line y = ax + b
Point reflect(Point p, float a, float b) {
	//distance from p to y=ax+b
	float d = (p.x + (p.y - b) * a) / (1 + a * a);
	Point reflected;
	reflected.x = 2 * d - p.x;
	reflected.y = 2 * d * a - p.y + 2 * b;
	return reflected;
}

// Mouse click event handler
void mouseClick(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		// Convert screen coordinates to world coordinates
		float wx = (float)x;
		float wy = (float)(glutGet(GLUT_WINDOW_HEIGHT) - y);

		// Update the line equation parameters based on the click
		a = (wy - b) / wx;
		b = wy - a * wx;

		// Redraw the scene
		glutPostRedisplay();
	}
}

// Display callback function
void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	// Draw the original triangle
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_TRIANGLES);
	glVertex2f(A.x, A.y);
	glVertex2f(B.x, B.y);
	glVertex2f(C.x, C.y);
	glEnd();

	// Reflect the triangle vertices
	Point A_ref = reflect(A, a, b);
	Point B_ref = reflect(B, a, b);
	Point C_ref = reflect(C, a, b);

	// Draw the reflected triangle
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_TRIANGLES);
	glVertex2f(A_ref.x, A_ref.y);
	glVertex2f(B_ref.x, B_ref.y);
	glVertex2f(C_ref.x, C_ref.y);
	glEnd();

	// Draw the reflection line
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES);
	glVertex2f(0, b);
	glVertex2f(glutGet(GLUT_WINDOW_WIDTH), a * glutGet(GLUT_WINDOW_WIDTH) + b);
	glEnd();

	glFlush();
}

// Initialize OpenGL
void init() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glColor3f(0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 500.0, 0.0, 500.0);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("2D Reflection");
	init();
	glutDisplayFunc(display);
	glutMouseFunc(mouseClick);
	glutMainLoop();
	return 0;
}
