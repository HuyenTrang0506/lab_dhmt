#include <GL/glut.h>
#include <math.h>
#include"stdio.h"

#define M_PI    3.14159265358979323846
static GLfloat gocX = 0.0f;
static GLfloat gocY = 0.0f;		
static GLfloat gocZ = 0.0f;		

static GLfloat size = 0.5;		
int HinhVe=2;
//----------------------------------------------------


void DrawSphere(float r) {
		
	float x,y,z,Delta_U,Delta_V,u,v,Pi_2;
	Pi_2 = M_PI/2;	Delta_U = 0.1;	Delta_V=0.1;
	
	//glBegin(GL_POINTS);
	//glBegin(GL_LINE_LOOP);
	glBegin(GL_LINE_STRIP);
	for(v = -Pi_2;v<Pi_2;v+=Delta_V){
		for(u=0;u<2*M_PI;u+=Delta_U){
			x= r*cos(u)*cos(v);
			y= r*sin(u)*cos(v);
			z= r*sin(v);
			glVertex3f(x,y,z);
		}
	}
	glEnd();
}

void DrawCylinder(float R, float h) {
    float x, y, z;
    float Delta_U = 0.06; 
    float Delta_V = 0.03; 

    glBegin(GL_LINE_STRIP);
    for (float v = 0; v <= h; v += Delta_V) {
        for (float u = 0; u <= 2 * M_PI; u += Delta_U) {
            x = R * cos(u);
            y = R * sin(u);
            z = v * h;
            glVertex3f(x, y, z);
        }
    }
    glEnd();
    
    for (float r = R; r > 0; r -= 0.1) {
        glBegin(GL_LINE_LOOP);
        for (float u = 0; u <= 2 * M_PI; u += Delta_U) {
            x = r * cos(u);
            y = r * sin(u);
            glVertex3f(x, y, 0); // Vòng tròn dáy
        }
        glEnd();
        
        glBegin(GL_LINE_LOOP);
        for (float u = 0; u <= 2 * M_PI; u += Delta_U) {
            x = r * cos(u);
            y = r * sin(u);
            glVertex3f(x, y, h); // Vòng tròn d?nh
        }
        glEnd();
    }
}
void DrawEllipsoid(float rx, float ry, float rz) {
    float x, y, z;
    float Delta_U = 0.1; 
    float Delta_V = 0.1;

    glBegin(GL_LINE_STRIP);
    for (float v = -M_PI / 2; v <= M_PI / 2; v += Delta_V) {
        for (float u = 0; u <= 2 * M_PI; u += Delta_U) {
            x = rx * cos(u) * cos(v);
            y = ry * sin(u) * cos(v);
            z = rz * sin(v);
            glVertex3f(x, y, z);
        }
    }
    glEnd();
}

void DrawToroid(float R, float r) {
    float x, y, z;
    float Delta_U = 0.1; 
    float Delta_V = 0.1;

    glBegin(GL_LINE_STRIP);
    for (float v = 0; v <= 2 * M_PI; v += Delta_V) {
        for (float u = 0; u <= 2 * M_PI; u += Delta_U) {
            x = (R + r * cos(v)) * cos(u);
            y = (R + r * cos(v)) * sin(u);
            z = r * sin(v);
            glVertex3f(x, y, z);
        }
    }
    glEnd();
}

void DrawHyperboloid(float a, float b, float c, bool oneSheet) {
    float x, y, z;
    float Delta_U = 0.1;
    float Delta_V = 0.1;

    glBegin(GL_LINE_STRIP);
    for (float v = -2; v <= 2; v += Delta_V) { // Kho?ng giá tr? v xác d?nh chi?u cao
        for (float u = 0; u <= 2 * M_PI; u += Delta_U) {
            x = a * cos(u) * sqrt(1 + (oneSheet ? v * v : -v * v));
            y = b * sin(u) * sqrt(1 + (oneSheet ? v * v : -v * v));
            z = c * v;
            glVertex3f(x, y, z);
        }
    }
    glEnd();
}


void DrawCone(float R, float h) {
    float x, y, z;
    float Delta_U = 0.03; 
    float Delta_V = 0.1; 

    glBegin(GL_LINE_STRIP);
    for (float v = 0; v <= 1; v += Delta_V) { 
        float r = R * (1 - v); 
        for (float u = 0; u <= 2 * M_PI; u += Delta_U) {
            x = r * cos(u);
            y = r * sin(u);
            z = v * h;
            glVertex3f(x, y, z);
        }
    }
    glEnd();
    
    for (float r = R; r > 0; r -= 0.1) {
        glBegin(GL_LINE_LOOP);
        for (float u = 0; u <= 2 * M_PI; u += Delta_U) {
            x = r * cos(u);
            y = r * sin(u);
            glVertex3f(x, y, 0); 
        }
        glEnd();
    }
}



//----------------------------------------------------
void VeHinh(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
    glPushMatrix();
	glRotatef(gocX, 1.0f, 0.0f, 0.0f);
	glRotatef(gocY, 0.0f, 1.0f, 0.0f);
	glRotatef(gocZ, 0.0f, 0.0f, 1.0f);	
	
	switch (HinhVe) { 
		
		case 2 :	DrawSphere(size);
					break; 
		case 3: 	DrawCylinder(size, 1.0f);  // Drawing cylinder with radius = size, height = 1.0
                	break;
        case 4: 	DrawCone(size, 1.0f);  // Drawing cone with base radius = size, height = 1.0
                	break;
        case 5: DrawEllipsoid(0.5, 0.3, 0.7); break;  // Bán kính theo 3 tr?c
		case 6: DrawHyperboloid(0.3, 0.3, 0.5, true); break;  // Hyperboloid m?t m?t
		
        case 7: DrawToroid(0.7, 0.2); break;  // R = 0.7, r = 0.2

		default: 
					break; 
	}
	glPopMatrix();
	glutSwapBuffers(); 
} 

void PhimNhanXoayHinh(int key, int x, int y)
{
	if(key == GLUT_KEY_END)			exit(0);
	if(key == GLUT_KEY_DOWN)		gocX-= 5.0f;
	if(key == GLUT_KEY_UP)			gocX += 5.0f;
	if(key == GLUT_KEY_RIGHT)		gocY -= 5.0f;
	if(key == GLUT_KEY_LEFT)		gocY += 5.0f;
	if(key == GLUT_KEY_PAGE_DOWN)	gocZ -= 5.0f;
	if(key == GLUT_KEY_PAGE_UP)		gocZ += 5.0f;
 	glutPostRedisplay();
}
void PhimNhanChon_Mau_Hinh(unsigned char k, int x, int y)
{
    printf("k= %d\n", k);

    switch (k) { 
        case 114 : glColor3f(1, 0, 0); // Red
                    glutPostRedisplay(); 
                    break; 
        case 103 : glColor3f(0, 1, 0); // Green
                    glutPostRedisplay(); 
                    break; 
        case 98 :  glColor3f(0, 0, 1); // Blue
                    glutPostRedisplay(); 
                    break; 

       
        case 50 :  HinhVe = 2;    
                    glutPostRedisplay(); 
                    glutSetWindowTitle("Quan Sat Vat The 3D - Sphere");
                    break; 
        case 51 :  HinhVe = 3;    
                    glutPostRedisplay(); 
                    glutSetWindowTitle("Quan Sat Vat The 3D - Cylinder");
                    break; 
        case 52 :  HinhVe = 4;    
                    glutPostRedisplay(); 
                    glutSetWindowTitle("Quan Sat Vat The 3D - Cone");
                    break;
        case 53 :  HinhVe = 5; 
                    glutPostRedisplay(); 
                    glutSetWindowTitle("Quan Sat Vat The 3D - Ellipsoid");
                    break; 
        case 54 :  HinhVe = 6; 
                    glutPostRedisplay(); 
                    glutSetWindowTitle("Quan Sat Vat The 3D - Hyperboloid (One Sheet)");
                    break; 
      
        case 56 :  HinhVe = 7; 
                    glutPostRedisplay(); 
                    glutSetWindowTitle("Quan Sat Vat The 3D - Toroid");
                    break;

        default: 
                    break; 
    }
}

void mouse(int button, int state, int x, int y)  
{ 
	if ((button==GLUT_LEFT_BUTTON)  & (state==GLUT_DOWN))	size = size + 0.05;
	if ((button==GLUT_RIGHT_BUTTON) & (state==GLUT_DOWN))	size = size - 0.05; 
	glutPostRedisplay();
} 
//----------------------------------------------------  
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB); 
	glutInitWindowSize (600, 600); 
	glutInitWindowPosition (50, 50);
	glutCreateWindow("Quan Sat Vat The 3D");
	glClearColor(0.0, 0.0, 0.0, 0.0 );	
	glColor3f(0, 0, 1);
	glutDisplayFunc(VeHinh);
	glutSpecialFunc(PhimNhanXoayHinh);
	glutKeyboardFunc(PhimNhanChon_Mau_Hinh);
	glutMouseFunc(mouse);
	glutMainLoop();
	return 0;
}
