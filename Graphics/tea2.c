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
GLfloat ambientm2[] = {0.2125, 0.1275, 0.054, 1.0};
GLfloat diffusem2[] = {0.714, 0.4284, 0.18144, 1.0};
GLfloat specularm2[] = {0.393548, 0.271906, 0.166721, 1.0};

GLfloat position[]={.0, 3.0, -3.0, 1.0};
GLfloat amb[]={1.0, 1.0, 1.0, 1.0};
GLfloat dif[]={1.0, 1.0, 1.0, 1.0};
GLfloat spc[]={1.0, 1.0, 1.0, 1.0};
GLfloat lmodel_ambient[] = {0.2, 0.2, 0.2, 1.0};
GLfloat local_view[] = {0.0};

GLfloat fovy = atan(5.0/1);
GLboolean control = true;
GLfloat matrix[9];

GLfloat rate1 = 0.0;// 设置旋转速率rate
GLfloat theta1 = 0.0;//旋转角度 
GLfloat theta11 = 0.0;
GLfloat anglex1 = 0.0;//X 轴旋转
GLfloat angley1 = 0.0;//Y 轴旋转
GLfloat rate2 = 0.0;// 设置旋转速率roate
GLfloat theta2 = 0.0;
GLfloat anglex2 = 0.0;
GLfloat angley2 = 0.0;

GLint WinW = 400;
GLint WinH = 400;
GLfloat oldx1; 
GLfloat oldy1;
GLfloat oldx2;  
GLfloat oldy2;

void init(){
	glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spc);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);
}
void reshape(int w, int h){
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glFrustum(-5.0, 5.0, -5.0, 5.0, 0, 5.0);
	gluPerspective(137.4, (GLfloat)w/(GLfloat)h, 1.0, 5.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);
}
void drawpot1(void){
	glColor3f(1.0, 0.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	//glPushMatrix();//
	gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);
	glTranslatef(-2.0, 0.0, -2.5);
	glRotatef(theta1, 1.0f, 0.0f, 0.0f);
	glRotatef(anglex1,0.0,1.0,0.0);
	glRotatef(theta11, 0.0f, 1.0f, 0.0f);
	glRotatef(angley1,1.0,0.0,0.0);
	glutSolidTeapot(1.0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientm);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffusem);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularm);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.6);
	//glPopMatrix();
}
void drawpot2(void){
	glColor3f(0.0, 1.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); 
	//glPushMatrix();
	gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);
	glTranslatef(2.0, 0.0, -2.5);
	glRotatef(theta2, 1.0f, 0.0f, 0.0f);
	glRotatef(anglex2,0.0,1.0,0.0);
	glRotatef(theta2, 0.0f, 1.0f, 0.0f);
	glRotatef(angley2,1.0,0.0,0.0);
	glutSolidTeapot(1.0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientm2);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffusem2);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularm2);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.2);
	//glPopMatrix();
}
void display(void){
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	theta1 += rate1;
	theta11 += rate1;
	drawpot1();
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	theta2 += rate2;
	drawpot2();
	glPopMatrix();
	glutSwapBuffers();
}


void mouse(int button, int state, int x, int y){
	if (button == GLUT_LEFT_BUTTON){
		if (state == GLUT_DOWN){
			rate1 = 0;
			theta1 = 0;
			theta11 = 0;
			oldx1 = x; 
			oldy1 = y;
			control = true;
		}	
	}
	if (button == GLUT_RIGHT_BUTTON){
		if (state == GLUT_DOWN){
			rate2 = 0;
			theta2 = 0;
			oldx2 = x; 
			oldy2 = y;
			control = false;
		}
	}
}
void motion(int x, int y){
	if(control){
		GLint deltax1 = oldx1 - x;
		GLint deltay1 = oldy1 - y;
		anglex1  += 360 * (GLfloat)deltax1 / sqrt(x*x + y*y);
		angley1 += 360 * (GLfloat)deltay1 / sqrt(x*x + y*y);
		oldx1 = x;  
		oldy1 = y;
		glutPostRedisplay();
	}
	else{
		GLint deltax2 = oldx2 - x;
		GLint deltay2 = oldy2 - y;
		anglex2  += 360 * (GLfloat)deltax2 / sqrt(x*x + y*y);
		angley2 += 360 * (GLfloat)deltay2 / sqrt(x*x + y*y);
		oldx2 = x; 
		oldy2 = y;
		glutPostRedisplay();
	}	
}
void keyboard(unsigned char key, int w, int h){
    if(key=='R') {
		//glPopMatrix();
		glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
		glutPostRedisplay();
	}
    if(key=='L') {
		glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	}
}


int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(display);
	glutMainLoop();
	return 0;
}
