#include <GL/glut.h>
#include <iostream>
#include <stack>


float fillColor[3] = {0.0, 1.0, 0.0};
float boundaryColor[3] = {1.0, 0.0, 0.0}; 

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
   
    std::stack<std::pair<int, int>> pixelStack;
    pixelStack.push({x, y}); 

    while (!pixelStack.empty()) {
       
        std::pair<int, int> currentPixel = pixelStack.top();
        int cx = currentPixel.first;
        int cy = currentPixel.second;

        pixelStack.pop(); 

        float currentColor[3];
        getPixel(cx, cy, currentColor); 

        if ((currentColor[0] != boundaryColor[0] || currentColor[1] != boundaryColor[1] || currentColor[2] != boundaryColor[2]) &&
            (currentColor[0] != fillColor[0] || currentColor[1] != fillColor[1] || currentColor[2] != fillColor[2])) {
            
			setPixel(cx, cy, fillColor);             
            pixelStack.push({cx - 1, cy}); 
            pixelStack.push({cx + 1, cy}); 
            pixelStack.push({cx, cy - 1}); 
            pixelStack.push({cx, cy + 1}); 
        }
    }
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT); 

    
    glColor3f(1.0, 0.0, 0.0); 
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
       
        boundaryFill(x, 500 - y, fillColor, boundaryColor); 
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

// Ham main
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); 
    glutInitWindowSize(500, 500); 
    glutInitWindowPosition(100, 100); 
    glutCreateWindow("Stack-based Boundary Fill Algorithm"); 
    init(); 
    glutDisplayFunc(display); 
    glutMouseFunc(mouse); 
    glutMainLoop(); 
    return 0;
}

