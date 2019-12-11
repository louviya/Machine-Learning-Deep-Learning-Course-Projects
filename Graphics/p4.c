#include <stdlib.h>  
#include <stdio.h>
#include <math.h>
#include <GL/gl.h> 
#include <GL/glu.h> 
#include <GL/glut.h>  
#include<iostream>
#include<cmath>
#pragma comment(lib, "glut32.lib")  
#include<SOIL.h>
#include <GL/freeglut.h>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <ctime>
#include <GL/glut.h>
#include "Image565.h"
using namespace std;

GLfloat ambientm[] = {0.0215, 0.1745, 0.0215, 1.0};
GLfloat diffusem[] = {0.07568, 0.61424, 0.7568, 1.0};
GLfloat specularm[] = {0.633, 0.727811, 0.0633, 1.0};
GLint num=0, mode=1;
GLfloat loc[100][3]={0};
GLfloat thick=2;
GLfloat matrix[16];
GLfloat MAGENTA[] = { 1, 1, 1 }; 
GLint movex, movey;
GLint spinx=0, spiny=0, des=0;
#define WIDTH 1024
#define HEIGHT 1024
const int MaxWidth=1024;
const int MaxHeight=1024;
char InputColorImage[64]="download.PPM";
char input[64]="e2.ppm";//t1.ppm";
GLubyte InputColorData[WIDTH][HEIGHT][4];
GLubyte image[WIDTH][HEIGHT][4];
GLubyte image2[WIDTH][HEIGHT][4];
int ImageHeight;
int ImageWidth;
unsigned char Alpha=100;


//load ppm
int LoadPPMImage(char* filename, unsigned char alpha, unsigned char pdata[MaxHeight][MaxWidth][4]){
    ByteImage texture;
    texture.ReadPnmFile(filename);
    ImageHeight=texture.NumRows();
    ImageWidth=texture.NumCols();
	
    for(int i=0; i<ImageHeight; ++i){
        for(int j=0; j<ImageWidth; ++j){
            pdata[i][j][0]=texture.GetPixel(0,i,j);
            pdata[i][j][1]=texture.GetPixel(1,i,j);
            pdata[i][j][2]=texture.GetPixel(2,i,j);
            pdata[i][j][3]=alpha;
        }
    }
    return 0;
}
static GLuint texName;
static GLuint texName2;
GLint WinWidth;
GLint WinHeight;
//创建位图函数
void MakeImage(void){
	int i,j,c;
	LoadPPMImage(InputColorImage, Alpha, InputColorData);
	for(i=0;i< HEIGHT;i++){
		for(j=0;j<WIDTH;j++){
			//c=(((i & 0x8) == 0) ^((j & 0x8) == 0)) *255;
			//计算纹理颜色
			image[i][j][0] = InputColorData[i][j][0];
			image[i][j][1] = InputColorData[i][j][1];
			image[i][j][2] = InputColorData[i][j][2];
			image[i][j][3] = GLubyte(255);
 
		}
	}
}
void MakeImage2(void){
	int i,j,c;
	LoadPPMImage(input, Alpha, InputColorData);
	for(i=0;i< HEIGHT;i++){
		for(j=0;j<WIDTH;j++){
			//c=(((i & 0x8) == 0) ^((j & 0x8) == 0)) *255;
			//计算纹理颜色
			image2[i][j][0] = InputColorData[i][j][0];
			image2[i][j][1] = InputColorData[i][j][1];
			image2[i][j][2] = InputColorData[i][j][2];
			image2[i][j][3] = GLubyte(255);
		}
	}
}
GLint n=0; 
GLUquadricObj* qobj;
GLfloat corner=0.0;
GLuint flag=0;
GLfloat xrot=0.0, yrot=0.0, zrot=0.0, xspeed=1.0, yspeed=1.0, zspeed=1.0;
static GLfloat xequalzero[] = {1.0, 0.0, 0.0, 0.0};
static GLfloat slanted[] = {1.0, 1.0, 1.0, 0.0};
static GLfloat *currentCoeff;
static GLenum currentPlane;
static GLint currentGenMode;
static float roangles;

void Moving() {    //球旋转
 if (flag == 1){
  corner += 0.20f;
  glutPostRedisplay();
 }
}
class Ball {
    double radius;
    GLfloat* color;
    double maximumHeight;
    double x;
    double y;
    double z;
    int direction;   
public:
    Ball(double r, GLfloat* c, double h, double x, double y, double z) :
        radius(r), color(c), maximumHeight(h), direction(-1),
        y(h), x(h), z(z) {
    }
    void drawball(){
		glEnable(GL_TEXTURE_2D);
		glPushMatrix();
		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
        glBindTexture(GL_TEXTURE_2D, texName2);
		//glRotatef(xrot, 1.0f, 0.0f, 0.0f);
		//glTranslatef(x+2, y-2, z);
		//glRotatef(yrot, 1.0f, 1.0f, 0.0f);
		//glTranslatef(-x-2,2-y,-z);
		/*
		glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
		glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
		//glEnable(GL_TEXTURE_2D);*/
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
        glTranslated(x+2, y-2, z);
		GLUquadric *q=gluNewQuadric();
		gluQuadricTexture(q, GL_TRUE);
        //glutSolidSphere(radius, 30, 30);
		gluSphere(q,radius, 30,30);
        glPopMatrix();
		glFlush();
		glDisable(GL_TEXTURE_2D);
		//xrot+=xspeed;
		//yrot+=yspeed;
	}
    void update() {
        x += direction * 0.05;
        if (x > maximumHeight) {
            x = maximumHeight;
            direction = -1;
        }
        else if (x < radius) {
            x = radius;
            direction = 1;
        }
		glEnable(GL_TEXTURE_2D);///
        glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, texName2);////
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
        glTranslated(x+2, y-2, z);
		GLUquadric *q=gluNewQuadric();///
		gluQuadricTexture(q, GL_TRUE);///
        //glutSolidSphere(radius, 30, 30);
		glRotatef(xrot,0.f,0.f,1.f);
        glRotatef(yrot,0.f,1.f,0.f);
		gluSphere(q,radius, 30,30);///
        glPopMatrix();
		glFlush();
		glDisable(GL_TEXTURE_2D);
		xrot+=xspeed;
		yrot+=yspeed;
    }
	void update2() {       
        if (x > maximumHeight*(-1)/3) {
            direction=1;
			x -= direction * 0.05;
			y += direction * 0.05;
			xrot+=xspeed;
		    yrot+=yspeed;
		    zrot+=zspeed;
        }
		else if((x<maximumHeight*(-1)/3)&&(x>-8.0)){
			direction=-1;
			x += direction * 0.05;
			y += direction * 0.05;
			//printf("%f", x);
			xrot+=xspeed;
		    yrot+=yspeed;
		    zrot+=zspeed;
		}
		else if(x==-8.0){
			direction=0;
			x += direction * 0.05;
			y -= direction * 0.05;
			xrot=0;
			yrot=0;
			zrot=0;
		}
        glEnable(GL_TEXTURE_2D);///
        glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, texName2);////
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
        glTranslated(x+2, y-1, z);
		GLUquadric *q=gluNewQuadric();///
		gluQuadricTexture(q, GL_TRUE);///
        //glutSolidSphere(radius, 30, 30);
		glRotatef(xrot,0.f,0.f,1.f);
        glRotatef(yrot,0.f,1.f,0.f);
		glRotatef(zrot,1.f,0.f,0.f);
		gluSphere(q,radius, 30,30);///
        glPopMatrix();
		glFlush();
		glDisable(GL_TEXTURE_2D);
		//xrot+=xspeed;
		//yrot+=yspeed;
		//zrot+=zspeed;
    }
};
Ball balls[] = {Ball(1.0, MAGENTA, 5, 8, 0, 14)};
GLfloat ty=0;
void b(GLint n){
	if(n==0){
		balls[0].drawball();
	}
	if(n==1){
		flag=1;
        balls[0].update();
    }
	if(n==2){
		flag=1;
		//balls[0].drawball();
		balls[0].update2();
	}
}





void init(){
	GLfloat position[]={0.0, 0.0, 350.0, 1.0};
	GLfloat amb[]={1.0, 1.0, 1.0, 1.0};
	GLfloat dif[]={1.0, 1.0, 1.0, 1.0};
	GLfloat spe[]={1.0, 1.0, 1.0, 1.0};
	GLfloat lmodel_ambient[] = {0.2, 0.2, 0.2, 1.0};
	GLfloat local_view[] = {0.0};
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glShadeModel(GL_FLAT);
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
	
	MakeImage();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
 
	glGenTextures(1, &texName);
	glBindTexture(GL_TEXTURE_2D, texName);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 
		0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		
	MakeImage2();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
 
	glGenTextures(1, &texName2);
	glBindTexture(GL_TEXTURE_2D, texName2);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT,
		0, GL_RGBA, GL_UNSIGNED_BYTE, image2);
   
   corner = 90.0f;
}



	


void draw(){
	glEnable(GL_TEXTURE_2D);
	GLfloat border[] = {1.00,0.00,0.00,1.00};
	GLfloat env[] = {1.00,1.00,1.00,1.00};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, env);
	
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	
	glBindTexture(GL_TEXTURE_2D, texName);
	//gluBuild2DMipmaps(GL_TEXTURE_2D,3, ww, hh, GL_RGB, GL_UNSIGNED_BYTE, "grass2.jpg");
	glColor4f(0.60,0.60,0.60,1.00);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(8.0f, 4.0f, 16.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-64.0f, 4.0f, 16.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-64.0f, -28.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(44.0f, -28.0f, 0.0f);
	glEnd();
	
	glFlush();
	glDisable(GL_TEXTURE_2D);
}



void display(void){
	//glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0); 
	
	gluLookAt(0.0,0.0, -1.0,0.0,0.0,0.0,0.0,1.0,0.0);
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
	draw();
	b(n);
    //glFlush();

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
    if(key=='h') {
		n=1;
	}
    if(key=='v') {
		n=2;
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
	glutInitWindowSize(600, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("ball");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(Moving);
	glutMainLoop();
	return 0;
}