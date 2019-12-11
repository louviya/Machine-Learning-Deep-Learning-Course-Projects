#include <stdlib.h>  
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>  
#include<iostream>
#include<cmath>
#pragma comment(lib, "glut32.lib")  
using namespace std;
GLfloat ambientm[] = {0.0215, 0.1745, 0.0215, 1.0};
GLfloat diffusem[] = {0.07568, 0.61424, 0.7568, 1.0};
GLfloat specularm[] = {0.633, 0.727811, 0.0633, 1.0};
GLint num=0, mode=1;
GLfloat loc[100][3]={0};
GLfloat thick=2;
GLfloat matrix[16];

GLint movex, movey;
GLint spinx=0, spiny=0, des=0;



void init(){
	GLfloat position[]={0.0, 0.0, 350.0, 1.0};
	GLfloat amb[]={1.0, 1.0, 1.0, 1.0};
	GLfloat dif[]={1.0, 1.0, 1.0, 1.0};
	GLfloat spe[]={1.0, 1.0, 1.0, 1.0};
	GLfloat lmodel_ambient[] = {0.2, 0.2, 0.2, 1.0};
	GLfloat local_view[] = {0.0};
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spe);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);
	glEnable(GL_TEXTURE_3D);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
}

GLfloat vertices[8][3] = {{0,0,0},{0,0,4},{0,4,0},{0,4,4},{4,0,0},{4,0,4},{4,4,0},{4,4,4}};
GLint faces[6][4] = {{1, 3, 5, 7}, {6, 7, 5, 4}, {2, 0, 6, 4},
  {0, 1, 7, 6}, {0, 1, 3, 2}, {2, 3, 5, 4}};
GLfloat vertexColors[8][3] = {
  {0.0, 0.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 1.0},
  {1.0, 0.0, 0.0}, {1.0, 0.0, 1.0}, {1.0, 1.0, 0.0}, {1.0, 1.0, 1.0}};

	
GLint n=1; //here n=1 equal to state 2 in assignment
void draw(GLfloat x, GLfloat y, GLfloat z, GLfloat s, GLint n){
	if(n<0){
	GLfloat vv[8][3];   //generate 8 vertices of a cube
	vv[0][0]= x-s/2; vv[0][1]= y-s/2; vv[0][2]= z-s/2;
	vv[1][0]= x+s/2; vv[1][1]= y-s/2; vv[1][2]= z-s/2;
	vv[2][0]= x-s/2; vv[2][1]= y-s/2; vv[2][2]= z+s/2;
	vv[3][0]= x+s/2; vv[3][1]= y-s/2; vv[3][2]= z+s/2;
	vv[4][0]= x-s/2; vv[4][1]= y+s/2; vv[4][2]= z+s/2;
	vv[5][0]= x+s/2; vv[5][1]= y+s/2; vv[5][2]= z+s/2;
	vv[6][0]= x-s/2; vv[6][1]= y+s/2; vv[6][2]= z-s/2;
	vv[7][0]= x+s/2; vv[7][1]= y+s/2; vv[7][2]= z-s/2;
	glPushMatrix();
		glBegin(GL_QUADS); //draw cube
        for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 4; j++) {
				glNormal3fv((GLfloat*)&vv[faces[i][j]]);
				glColor3fv((GLfloat*)&vertexColors[faces[i][j]]);
				glVertex3fv((GLfloat*)&vv[faces[i][j]]);	
		    }
	    }
	    glEnd();
	glPopMatrix();
	glFlush();}
	else{  //when n is not less than 0, repeat drawing
		s=s/3;
		draw(x-s,y+s,z-s,s, n-1);draw(x,y+s,z-s,s,n-1);draw(x+s,y+s,z-s,s,n-1);
		draw(x-s,y,z-s,s, n-1);draw(x+s,y,z-s,s, n-1);draw(x-s,y-s,z-s,s, n-1);
		draw(x,y-s,z-s,s, n-1);draw(x+s,y-s,z-s,s, n-1);draw(x-s,y+s,z,s, n-1);
		draw(x+s,y+s,z,s, n-1);draw(x-s,y-s,z,s, n-1);draw(x+s,y-s,z,s, n-1);
		draw(x-s,y+s,z+s,s, n-1);draw(x,y+s,z+s,s, n-1);draw(x+s,y+s,z+s,s, n-1);
		draw(x-s,y,z+s,s, n-1);draw(x+s,y,z+s,s, n-1);draw(x-s,y-s,z+s,s, n-1);
		draw(x,y-s,z+s,s, n-1);draw(x+s,y-s,z+s,s, n-1);
	    }
}



void display(void){
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0); 
	gluLookAt(0.0,0.0, 10.0,0.0,0.0,0.0,0.0,1.0,0.0);
	//glOrtho(-50, 50, -50, 50, 0, 100);
	glPushMatrix();
	glRotated(spinx, 0, 1, 0);
	glRotated(spiny, 1, 0, 0);	
	//glTranslated(-2,-2,-2);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientm);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffusem);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularm);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.6);
	glPolygonMode(GL_FRONT, GL_FILL); //front is filled
	//glPolygonMode(GL_BACK, GL_LINE); //back is line
	//glFrontFace(GL_CCW); //counter-clockwise is front
	draw(0,0,0,3,n);
	glPopMatrix();
	glutSwapBuffers();
	glFlush();
	glutPostRedisplay();
}


void mouse(int button, int state, int x, int y){
	if (state==GLUT_DOWN && button == GLUT_LEFT_BUTTON){
		if(mode==1){
			loc[num][0]=x/50;
			loc[num][1]=(500-y)/50;
			loc[num][2]=0;
			//printf("%f, %f\n", loc[num][0], loc[num][1]);
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

void keyboard(unsigned char key, int w, int h){
    if(key=='+') {
		n++;
	}
    if(key=='-') {
		n--;
	}
}
void reshape(int w, int h){
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w/(GLfloat)h, 0.5, 40.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char** argv){
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("sponge");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}