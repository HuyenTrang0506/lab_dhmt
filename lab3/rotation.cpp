#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <vector>

#ifndef M_PI
#define M_PI 3.14159265358979323846  // Define M_PI for compatibility
#endif

using namespace std;

// Structure to represent a point with x and y coordinates
struct Point {
	float x, y;
};

// Initial triangle points
Point A = {0.0f, 0.5f};
Point B = {-0.5f, -0.5f};
Point C = {0.5f, -0.5f};

// New points for the triangle after rotation
Point newA = {0.0f, 0.5f};
Point newB = {-0.5f, -0.5f};
Point newC = {0.5f, -0.5f};

// Center of rotation, initialized to origin (0, 0)
Point center = {0.0f, 0.0f};

// Translation vector for adjusting the center of rotation
Point translation = {0.0f, 0.0f};

// Rotation angle (in degrees)
float angle = 0.0f;

vector<float> multiplyMatrices(const vector<vector<float>>& matrix3, vector<float>& matrix1x3) {
   
    vector<float> result(3, 0);

    // Adjust the vector by subtracting the translation before rotation
    matrix1x3[0] -= translation.x;
	matrix1x3[1] -= translation.y;

    // Multiply the 3x3 rotation matrix with the 1x3 vector
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            result[i] += matrix3[j][i] * matrix1x3[j];
        }
    }
	
	// Reapply the translation after rotation
	result[0] += translation.x;
	result[1] += translation.y;

    return result;
}

// Display function to render the triangles before and after rotation
void display() {
	glClear(GL_COLOR_BUFFER_BIT);  // Clear the screen

	// Draw the original triangle in red
	glColor3f(1.0f, 0.0f, 0.0f);  
	glBegin(GL_TRIANGLES);
	glVertex2f(A.x, A.y);
	glVertex2f(B.x, B.y);
	glVertex2f(C.x, C.y);
	glEnd();

	// Draw the rotated triangle in blue
	glColor3f(0.0f, 0.0f, 1.0f);  
	glBegin(GL_TRIANGLES);
	glVertex2f(newA.x, newA.y);
	glVertex2f(newB.x, newB.y);
	glVertex2f(newC.x, newC.y);
	glEnd();

	glutSwapBuffers();  // Swap buffers to display the updated frame
}

// Mouse function to set the center of rotation based on mouse click position
void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		// Convert mouse position to OpenGL coordinates (normalized)
		center.x = (float)x / (glutGet(GLUT_WINDOW_WIDTH) / 2) - 1.0f;
		center.y = 1.0f - (float)y / (glutGet(GLUT_WINDOW_HEIGHT) / 2);

		// Set the translation vector to adjust the center of rotation
		translation.x = center.x - 0.0f;
		translation.y = center.y - 0.0f;

		// Output the center of rotation to the console
		std::cout << "Center of rotation: (" << center.x << ", " << center.y << ")\n";
	}
}

// Keyboard function to handle rotation on key press
void keyboard(unsigned char key, int x, int y) {
	if (key == 'r') {
		// Increase the rotation angle by 10 degrees
		angle += 10.0f;
		if (angle >= 360.0f) {
			angle -= 360.0f;  // Keep the angle within [0, 360)
		}

		// Calculate the rotation matrix based on the angle
		float rad = angle * M_PI / 180.0f; 
		float cosA = cos(rad);
		float sinA = sin(rad);
		
		// 2D rotation matrix
        vector<vector<float>> rotationMatrix = {
            {cosA, sinA, 0.0f},
            {-sinA, cosA, 0.0f},
            {0.0f, 0.0f, 1.0f}
        };

		// Apply the rotation to each point of the triangle
		vector<float> pointA = {A.x, A.y, 1};
		vector<float> rotatedA = multiplyMatrices(rotationMatrix, pointA);
		newA.x = rotatedA[0];
		newA.y = rotatedA[1];

		vector<float> pointB = {B.x, B.y, 1};
		vector<float> rotatedB = multiplyMatrices(rotationMatrix, pointB);
		newB.x = rotatedB[0];
		newB.y = rotatedB[1];

		vector<float> pointC = {C.x, C.y, 1};
		vector<float> rotatedC = multiplyMatrices(rotationMatrix, pointC);
		newC.x = rotatedC[0];
		newC.y = rotatedC[1];

		// Redraw the window with the updated rotated triangle
		glutPostRedisplay();
	}
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);  
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);  
	glutInitWindowSize(800, 600); 
	glutCreateWindow("2D Rotation");  

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);  
	glMatrixMode(GL_PROJECTION);  
	glLoadIdentity();  
	gluOrtho2D(-1.0, 1.0, -1.0, 1.0);  

	// Register GLUT callback functions
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);

	glutMainLoop(); 
	return 0;
}

