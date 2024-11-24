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

void matcong(GLfloat s){
	glutWireTeapot(s);
}

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
		case 1 :	matcong(size);
					break; 
		case 2 :	DrawSphere(size);
					break; 
		case 3: 	DrawCylinder(size, 1.0f);  // Drawing cylinder with radius = size, height = 1.0
                	break;
        case 4: 	DrawCone(size, 1.0f);  // Drawing cone with base radius = size, height = 1.0
                	break;
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
	printf("k= %d",k);
	
	switch (k) { 
		case 114 :	glColor3f(1, 0, 0);	  //r
					glutPostRedisplay(); 
					break; 
		case  103 : glColor3f(0, 1, 0);   //g
					glutPostRedisplay(); 
					break; 
		case  98 :	glColor3f(0, 0, 1);   //b
					glutPostRedisplay(); 
					break; 

		case 49 :	HinhVe=1;	  
					glutPostRedisplay(); 
					break; 
		case 50 :	HinhVe=2;	  
					glutPostRedisplay(); 
					break; 
		case 51 :	HinhVe=3;	  
					glutPostRedisplay(); 
					break; 
		case 52 :	HinhVe=4;	  
					glutPostRedisplay(); 
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
