#include <GL/glut.h>
#include <iostream>
#include <vector>

using namespace std;

struct Point {
    float x, y;
};

// Original triangle
Point A = {0.0f, 0.5f};
Point B = {-0.5f, -0.5f};
Point C = {0.5f, -0.5f};

// Scaled triangle
Point newA = {0.0f, 0.5f};
Point newB = {-0.5f, -0.5f};
Point newC = {0.5f, -0.5f};

// Scaling center
Point scaleCenter = {0.0f, 0.0f};

// Scaling factors
float scaleX = 1.0f, scaleY = 1.0f;

//nhan ma tran
vector<float> multiplyMatrices(const vector<vector<float>>& matrix3, const vector<float>& matrix1x3) {
    // Initialize the result as a 1x3 matrix with zeros
    vector<float> result(3, 0);
    
    // Multiply each row of the 3x3 matrix by the corresponding element of the 1x3 matrix
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            result[i] += matrix3[i][j] * matrix1x3[j];
        }
    }

    return result;
}

void drawTriangle(Point a, Point b, Point c) {
    glBegin(GL_TRIANGLES);
    glVertex2f(a.x, a.y);
    glVertex2f(b.x, b.y);
    glVertex2f(c.x, c.y);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Draw original triangle in orange
    glColor3f(1.0f, 0.647f, 0.0f);
    drawTriangle(A, B, C);

    // Draw scaled triangle in green
    glColor3f(0.5f, 1.0f, 0.5f);
    drawTriangle(newA, newB, newC);

    glFlush();
}

void scaleTriangle() {
    // Translate to origin
    vector<vector<float>> translateToOrigin = {
        {1.0f, 0.0f, -scaleCenter.x},
        {0.0f, 1.0f, -scaleCenter.y},
        {0.0f, 0.0f, 1.0f}
    };

    // Scaling matrix
    vector<vector<float>> scalingMatrix = {
        {scaleX, 0.0f, 0},
        {0.0f, scaleY, 0},
        {0.0f, 0.0f, 1.0f}
    };

    // Translate back to scale center
    vector<vector<float>> translateBack = {
        {1.0f, 0.0f, scaleCenter.x},
        {0.0f, 1.0f, scaleCenter.y},
        {0.0f, 0.0f, 1.0f}
    };

    // Apply transformations to each point
    vector<float> tempA = multiplyMatrices(translateToOrigin, {A.x, A.y, 1});
    tempA = multiplyMatrices(scalingMatrix, tempA);
    vector<float> scaledA = multiplyMatrices(translateBack, tempA);
    newA.x = scaledA[0];
    newA.y = scaledA[1];

    vector<float> tempB = multiplyMatrices(translateToOrigin, {B.x, B.y, 1});
    tempB = multiplyMatrices(scalingMatrix, tempB);
    vector<float> scaledB = multiplyMatrices(translateBack, tempB);
    newB.x = scaledB[0];
    newB.y = scaledB[1];

    vector<float> tempC = multiplyMatrices(translateToOrigin, {C.x, C.y, 1});
    tempC = multiplyMatrices(scalingMatrix, tempC);
    vector<float> scaledC = multiplyMatrices(translateBack, tempC);
    newC.x = scaledC[0];
    newC.y = scaledC[1];
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        scaleCenter.x = (x / 250.0f) - 1.0f;
        scaleCenter.y = 1.0f - (y / 250.0f);
        std::cout << "Scale center set to: (" << scaleCenter.x << ", " << scaleCenter.y << ")\n";
    } else if (button == 3) { // Scroll up
        scaleX *= 1.1f;
        scaleY *= 1.1f;
        scaleTriangle();
        glutPostRedisplay();
    } else if (button == 4) { // Scroll down
        scaleX *= 0.9f;
        scaleY *= 0.9f;
        scaleTriangle();
        glutPostRedisplay();
    }
}

void keyboard(unsigned char key, int x, int y) {

    switch (key) {
        case 'e': 
	        scaleX *= 1.1f;
	        scaleY *= 1.1f;
	        scaleTriangle();
	        glutPostRedisplay();
            break;
        case 'q':  
	        scaleX *= 0.9f;
	        scaleY *= 0.9f;
	        scaleTriangle();
	        glutPostRedisplay();
            break;
        default:
            std::cout << "Phím không du?c x? lý!" << std::endl;
            break;
    }
}


void init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("2D Scaling Transformation");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}
