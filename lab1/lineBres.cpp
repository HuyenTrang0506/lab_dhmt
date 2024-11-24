#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

typedef struct {
	int x,y;
} Point;

Point P[3];
int n=-1;
float m=1;

void LineBres1(int x1,int y1,int x2,int y2){
	int Dx,Dy,P,x,y,const1,const2;
	Dx=x2-x1; 
	Dy=y2-y1;
	const1=2*Dy; 
	const2=2*(Dy-Dx);
	P=2*Dy-Dx;
	x=x1; 
	y=y1;
	glBegin(GL_POINTS);
		while (x<=x2){
			glVertex3i(x,y,0);
			if (P<0) P+=const1;
			else{ 
				P+=const2;
				y++;
			}
			x++;
		}
	glEnd();
}

void LineBres2(int x1,int y1,int x2,int y2){
	int Dx,Dy,P,x,y,const1,const2;
	Dx=x2-x1; 
	Dy=y2-y1;
	const1=2*Dx; 
	const2=2*(Dx-Dy);
	P=2*Dx-Dy;
	x=x1; 
	y=y1;
	glBegin(GL_POINTS);
		while (y<=y2){
			glVertex3i(x,y,0);
			if (P<0) P+=const1;
			else{ 
				P+=const2;
				x++;
			}
			y++;
		}
	glEnd();
}

void LineBres3(int x1,int y1,int x2,int y2){
	int Dx,Dy,P,x,y,const1,const2;
	Dx=x2-x1; 
	Dy=y2-y1;
	const1=2*Dy; 
	const2=2*(Dx+Dy);
	P=2*Dy+Dx;
	x=x1; 
	y=y1;
	glBegin(GL_POINTS);
		while (x<=x2){
			glVertex3i(x,y,0);
			if (P>=0) P+=const1;
			else{ 
				P+=const2;
				y--;
			}
			x++;
		}
	glEnd();
}

void LineBres4(int x1,int y1,int x2,int y2){
	int Dx,Dy,P,x,y,const1,const2;
	Dx=x2-x1; 
	Dy=y2-y1;
	const1=2*Dx; 
	const2=2*(Dx+Dy);
	P=2*Dx+Dy;
	x=x1; 
	y=y1;
	glBegin(GL_POINTS);
		while (y>=y2){
			glVertex3i(x,y,0);
			if (P<0) P+=const1;
			else{ 
				P+=const2;
				x++;
			}
			y--;
		}
	glEnd();
}


void MouseEventHandler(int button, int state, int x, int y)
{
	if(button == GLUT_LEFT_BUTTON && state ==  GLUT_UP)
		{   n++;
			P[n].x=x-300;
			P[n].y=300-y;		printf("\n n=%i x=%d y=%d",n,P[n].x,P[n].y);
			if(n==1){
				m = float(P[1].y - P[0].y)/float(P[1].x - P[0].x);
				printf("\n m=%f",m);
				n=-1;
			}
			glutPostRedisplay();
		}
}	

void MyDisplay(){
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);
	if(m>0 && m<=1) LineBres1(P[0].x, P[0].y, P[1].x, P[1].y);
	else if(m>1) LineBres2(P[0].x, P[0].y, P[1].x, P[1].y);
	else if(m>=-1 && m<0) LineBres3(P[0].x, P[0].y, P[1].x, P[1].y);
	else if(m<-1) LineBres4(P[0].x, P[0].y, P[1].x, P[1].y);
	glFlush();
}

int main(int argc,char* arg[]){

	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB); 
	glutInitWindowSize (600, 600); 
	glutInitWindowPosition (10, 10); 
	glutCreateWindow("Circle 1");
	gluOrtho2D(-300, 300, -300, 300);
	glClearColor(0,0,0,0);
	glutMouseFunc(MouseEventHandler);	
	glutDisplayFunc(MyDisplay);
	glutMainLoop();
}
