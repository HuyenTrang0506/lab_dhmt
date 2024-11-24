#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

typedef struct {
	int x,y;
} Point;

Point P[10];
int n=-1;
//-----------------------------------------

//Cai dat ham Ve duong cong Bezier o day 
long 	GT(int n)
{   long gt;
	if (n==0)  gt=1;
	else
	{   gt=1;
		for (int i=1;i<=n;i++)     	gt=gt*i;
	}
	return gt;
}
double LT(double coso,int mu)
{
	double lt;
	if (mu==0)		lt=1;
	else
	{	lt=1;
		for (int i=1;i<=mu;i++)
			lt=lt*coso;
	}
	return lt;
}
int    CKN(int n, int k)
{
	long gtn,gtk,gtnk;
	int ckn;
	gtn = GT(n);	gtk = GT(k);	gtnk = GT(n-k);
	ckn=gtn/(gtk*gtnk);
	return ckn ;
}
double BNK(double t, int k, int n)
{
	double b;
	b=CKN(n,k)*LT(1-t,n-k)*LT(t,k);
	return b;
}
Point   Tpt (Point  p[], int n, double t)
{
	Point  pt;
	double B;
	pt.x=0; pt.y=0;
	for (int k=0; k<=n; k++)
	{
		B=BNK(t,k,n);
		pt.x=(long)(pt.x+p[k].x*B);
		pt.y=(long)(pt.y+p[k].y*B);
	}
	return pt;
}
void veBezier(Point  p[], int n)

{
	float dt,t=0;
	int i;
	int m=10;
	Point  pt;
	dt=1/float(m);
	glBegin(GL_LINE_STRIP);
	for (i=1;i<=m;i++)
	{
		pt=Tpt(p,n,t);
		glVertex2i(pt.x,pt.y);
		t=t+dt;	
	}
	glVertex2i(p[n].x,p[n].y);
	glEnd();

}
void VeDaGiacKiemSoat(Point P[], int n){
	glEnable(GL_LINE_STIPPLE);	// enable kiểu vẽ
	glLineStipple(1, 0xAAA);	// thiet lap kieu ve
	glBegin(GL_LINE_STRIP);
	for (int i=0; i<=n;	i++)
		glVertex2i(P[i].x,P[i].y);
	glEnd();
	glDisable (GL_LINE_STIPPLE); // disable kiểu vẽ
}

void Mydisplay(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(0.0, 1.0, 0.0);
	VeDaGiacKiemSoat(P,n);
	
	// goi ham Ve duong cong Bezier o day 
	glColor3f(1.0, 1.0, 0.0);
	veBezier(P,n);
	glFlush();
}

void MouseEventHandler(int button, int state, int x, int y)
{
	if(button == GLUT_LEFT_BUTTON && state ==  GLUT_UP)
		{   n++;
			P[n].x=x-300;
			P[n].y=300-y;		printf("\n n=%i x=%d y=%d",n,P[n].x,P[n].y);			
			glutPostRedisplay();
		}
}	

//-----------------------------------------
int main(int argc,char* arg[]){

	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB); 
	glutInitWindowSize (600, 600); 
	glutInitWindowPosition (10, 10); 
	glutCreateWindow("Ve duong cong Bezier");
	gluOrtho2D(-300, 300, -300, 300);
	glClearColor(0,0,0,0);
	glutDisplayFunc(Mydisplay);
	glutMouseFunc(MouseEventHandler);	
	glutMainLoop();
}
