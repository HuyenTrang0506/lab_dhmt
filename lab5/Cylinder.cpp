#include <GL/glut.h>
#include <math.h>
#include "stdio.h"

#define M_PI 3.14159265358979323846

static GLfloat gocX = 0.0f;
static GLfloat gocY = 0.0f;
static GLfloat gocZ = 0.0f;

static GLfloat sizeR = 0.5; 
void DrawCylinder(float R, float y_begin, float y_end) {
    float x, y, z;
    float Delta_U = 0.06;
    float h = y_end - y_begin;  // Tính chi?u cao t? y_end - y_begin

    // V? b? m?t bên c?a Cylinder (theo tr?c y)
    glBegin(GL_LINE_STRIP);
    for (float y_pos = y_begin; y_pos <= y_end; y_pos += 0.03) { // Chi?u dài c?a tr? theo tr?c y
        for (float u = 0; u <= 2 * M_PI; u += Delta_U) {
            x = R * cos(u); // T?a d? x c?a vòng tròn
            z = R * sin(u); // T?a d? z c?a vòng tròn
            y = y_pos;      // Chi?u dài theo tr?c y
            glVertex3f(x, y, z);
        }
    }
    glEnd();

    // V? hai vòng tròn dáy và d?nh c?a hình tr? (? y = y_begin và y = y_end)
    glBegin(GL_LINE_LOOP);
    for (float u = 0; u <= 2 * M_PI; u += Delta_U) {
        x = R * cos(u); // T?a d? x c?a vòng tròn
        z = R * sin(u); // T?a d? z c?a vòng tròn
        glVertex3f(x, y_begin, z); // Ðáy, y = y_begin
    }
    glEnd();

    glBegin(GL_LINE_LOOP);
    for (float u = 0; u <= 2 * M_PI; u += Delta_U) {
        x = R * cos(u); // T?a d? x c?a vòng tròn
        z = R * sin(u); // T?a d? z c?a vòng tròn
        glVertex3f(x, y_end, z); // Ð?nh, y = y_end
    }
    glEnd();
}


void VeHinh(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
   

  
    glPushMatrix();
    glRotatef(gocX, 1.0f, 0.0f, 0.0f);
    glRotatef(gocY, 0.0f, 1.0f, 0.0f);
    glRotatef(gocZ, 0.0f, 0.0f, 1.0f);

    // V? Cylinder
    DrawCylinder(sizeR,0.2f, 0.4f);
    DrawCylinder(sizeR,-0.4f, -0.2f);

    glPopMatrix();
    glutSwapBuffers();
}

void PhimNhanXoayHinh(int key, int x, int y) {
    if (key == GLUT_KEY_END)
        exit(0);
    if (key == GLUT_KEY_DOWN)
        gocX -= 5.0f;
    if (key == GLUT_KEY_UP)
        gocX += 5.0f;
    if (key == GLUT_KEY_RIGHT)
        gocY -= 5.0f;
    if (key == GLUT_KEY_LEFT)
        gocY += 5.0f;
    if (key == GLUT_KEY_PAGE_DOWN)
        gocZ -= 5.0f;
    if (key == GLUT_KEY_PAGE_UP)
        gocZ += 5.0f;
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    if ((button == GLUT_LEFT_BUTTON) & (state == GLUT_DOWN))
        sizeR = sizeR + 0.05;
    if ((button == GLUT_RIGHT_BUTTON) & (state == GLUT_DOWN))
        sizeR = sizeR - 0.05;
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("3D Cylinder Viewer");
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glColor3f(0, 0, 1);
    glutDisplayFunc(VeHinh);
    glutSpecialFunc(PhimNhanXoayHinh);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}

