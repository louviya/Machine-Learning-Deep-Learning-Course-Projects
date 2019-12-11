#include <stdlib.h>  
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>  
#include<iostream>
#include<cmath>
#pragma comment(lib, "glut32.lib")  
using namespace std;
GLfloat ambientm[] = {0.0215, 0.1745, 0.0215, 1.0};
GLfloat diffusem[] = {0.07568, 0.61424, 0.07568, 1.0};
GLfloat specularm[] = {0.633, 0.727811, 0.633, 1.0};
GLint num=0, mode=1;
GLfloat loc[100][3]={0};
GLfloat thick=2;
GLfloat matrix[16];

GLint movex, movey;
GLint spinx=0, spiny=0, des=0;

void init(){
	GLfloat position[]={0.0, 0.0, 50.0, 0.0};
	GLfloat amb[]={0.5, 0.8, 0.1, 0.1};
	GLfloat dif[]={1.0, 1.0, 1.0, 1.0};
	GLfloat lmodel_ambient[] = {0.4, 0.4, 0.4, 1.0};
	GLfloat local_view[] = {0.0};
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	//glClearDepth(1.0f);//
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);//
    glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);
}
void drawPolygon(int num){
	glPushMatrix();
	//glScaled(5, 4, 4);
	glTranslatef(0, 0, 0);
	glLoadIdentity();
	//glMultMatrixf(matrix);
	glBegin(GL_POLYGON);
		for(int i=0; i<num;i++){
			glNormal3f(0.0f,0.0f,1.0f);
			//glColor3f(1.0, 0, 0);
			glVertex3f(loc[i][0], loc[i][1], loc[i][2]);
		}
	glEnd();
	glPopMatrix();
	glFlush();
}

void extrude(int num){
	glPushMatrix();
	glBegin(GL_POLYGON);
	//glColor3f(0.0, 0.0, 1.0);
		for(int i=0; i+1<num;i++){
			glVertex3f(loc[i+1][0], loc[i+1][1], loc[i][2]);
			glVertex3f(loc[i][0], loc[i][1], loc[i][2]);
			glVertex3f(loc[i][0], loc[i][1], thick);
			glVertex3f(loc[i+1][0], loc[i+1][1], thick);
		}
		glVertex3f(loc[0][0], loc[0][1], 0.0);
		glVertex3f(loc[num-1][0], loc[num-1][1], 0.0);
		glVertex3f(loc[num-1][0], loc[num-1][1], thick);
		glVertex3f(loc[0][0], loc[0][1], thick);	
	glEnd();
	glPopMatrix();
	glFlush();
}

void display(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glMatrixMode(GL_MODELVIEW);
	glColor3f(1.0, 1.0, 1.0); 
	glLoadIdentity();
	//glPointSize(3);
	gluLookAt(0.0,0.0, 20.0,0.0,0.0,0.0,0.0,1.0,0.0);
	//glOrtho(-50, 50, -50, 50, 0, 100);
	glPushMatrix();
	glRotated(spinx, 0, 1, 0);
	glRotated(spiny, 1, 0, 0);	
	glTranslated(0,0,des);
	glPolygonMode(GL_FRONT, GL_FILL); //front is filled
	//glPolygonMode(GL_BACK, GL_LINE); //back is line
	glFrontFace(GL_CCW); //counter-clockwise is front
	drawPolygon(num);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientm);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffusem);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularm);
	//glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.6);
	extrude(num);
	glPopMatrix();
	glutSwapBuffers();
	glFlush();
}


void mouse(int button, int state, int x, int y){
	if (state==GLUT_DOWN && button == GLUT_LEFT_BUTTON){
		if(mode==1){
			loc[num][0]=x/50;
			loc[num][1]=(500-y)/50;
			loc[num][2]=0;
			printf("%f, %f\n", loc[num][0], loc[num][1]);
			num++;
		}
		if(mode==2){
			movex=x;
			movey=y;
		}
	}
	if (state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON){
		mode=2;
	}
}
void motion(int x, int y){
		GLint dx=x-movex;
		GLint dy=y-movey;
		printf("%dx, %dy\n", dx, dy);
		spinx +=dx;
		spiny +=dy;
		glutPostRedisplay();
		movex=x;
		movey=y;
		
}
void reshape(int w, int h){
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w/(GLfloat)h, 0.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("polygon");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutMainLoop();
	return 0;
}