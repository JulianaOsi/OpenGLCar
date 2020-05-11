#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <GL/glut.h>

using namespace std;

static int useRGB = 1;
static int useLighting = 1;
static int useQuads = 1;

#define GRAY	0
#define RED	    1
#define GREEN	2
#define BLUE	3
#define CYAN	4
#define MAGENTA	5
#define YELLOW	6
#define BLACK	7
#define WHITE	8
#define DARK_GRAY	9

static float materialColor[10][4] =
{
  {0.8, 0.8, 0.8, 1.0},
  {0.8, 0.0, 0.0, 1.0},
  {0.27, 0.64, 0.27, 1.0},
  {0.43, 0.71, 0.9, 1.0},
  {0.0, 0.8, 0.8, 1.0},
  {0.8, 0.0, 0.8, 1.0},
  {0.8, 0.8, 0.0, 1.0},
  {0.0, 0.0, 0.0, 0.6},
  {1.0, 1.0, 1.0, 1.0},
  {0.5, 0.5, 0.5, 1.0}
};

static float lightPos[4] =
{ 1.0, 5.5, 5.0, 1.0 };

static float lightAmb[4] =
{ 0.2, 0.2, 0.2, 1.0 };
static float lightDiff[4] =
{ 0.8, 0.8, 0.8, 1.0 };
static float lightSpec[4] =
{ 0.4, 0.4, 0.4, 1.0 };

static float groundPlane[4] =
{ 0.0, 1.0, 0.0, 1.499 };
static float backPlane[4] =
{ 0.0, 0.0, 1.0, 0.899 };


static void setColor(int c)
{
	glMaterialfv(GL_FRONT_AND_BACK,
		GL_AMBIENT_AND_DIFFUSE, &materialColor[c][0]);
}

static void drawCheck(int w, int h, int evenColor, int oddColor)
{
	static int initialized = 0;
	static int usedLighting = 0;
	static GLuint checklist = 0;

	if (!initialized || (usedLighting != useLighting)) {
		static float square_normal[4] =
		{ 0.0, 0.0, 1.0, 0.0 };
		static float square[4][4];
		int i, j;

		if (!checklist) {
			checklist = glGenLists(1);
		}
		glNewList(checklist, GL_COMPILE_AND_EXECUTE);

		if (useQuads) {
			glNormal3fv(square_normal);
			glBegin(GL_QUADS);
		}
		for (j = 0; j < h; ++j) {
			for (i = 0; i < w; ++i) {
				square[0][0] = -1.0 + 2.0 / w * i;
				square[0][1] = -1.0 + 2.0 / h * (j + 1);
				square[0][2] = 0.0;
				square[0][3] = 1.0;

				square[1][0] = -1.0 + 2.0 / w * i;
				square[1][1] = -1.0 + 2.0 / h * j;
				square[1][2] = 0.0;
				square[1][3] = 1.0;

				square[2][0] = -1.0 + 2.0 / w * (i + 1);
				square[2][1] = -1.0 + 2.0 / h * j;
				square[2][2] = 0.0;
				square[2][3] = 1.0;

				square[3][0] = -1.0 + 2.0 / w * (i + 1);
				square[3][1] = -1.0 + 2.0 / h * (j + 1);
				square[3][2] = 0.0;
				square[3][3] = 1.0;

				if (i & 1 ^ j & 1) {
					setColor(oddColor);
				}
				else {
					setColor(evenColor);
				}

				if (!useQuads) {
					glBegin(GL_POLYGON);
				}
				glVertex4fv(&square[0][0]);
				glVertex4fv(&square[1][0]);
				glVertex4fv(&square[2][0]);
				glVertex4fv(&square[3][0]);
				if (!useQuads) {
					glEnd();
				}
			}
		}

		if (useQuads) {
			glEnd();
		}
		glEndList();

		initialized = 0;
		usedLighting = useLighting;
	}
	else {
		glCallList(checklist);
	}
}

void drawTube(GLfloat height, GLfloat outer_radius, GLfloat inner_radius, GLint slices, GLint stacks)
{
	glTranslatef(0.0, 0.0, -height / 2.0);
	GLUquadric* q = gluNewQuadric();
	gluQuadricOrientation(q, GLU_INSIDE);
	setColor(GRAY);
	gluCylinder(q, inner_radius, inner_radius, height, slices, stacks);

	gluQuadricOrientation(q, GLU_OUTSIDE);
	gluCylinder(q, outer_radius, outer_radius, height, slices, stacks);

	glPushMatrix();
	setColor(DARK_GRAY);
	glTranslatef(0, 0, height);
	gluDisk(q, inner_radius, outer_radius, slices, stacks);
	glPopMatrix();

	glPushMatrix();
	glRotatef(180, 1, 0, 0);
	gluDisk(q, inner_radius, outer_radius, slices, stacks);
	glPopMatrix();

	gluDeleteQuadric(q);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glTranslatef(0.0, 0, 0.0);
	glRotatef(-90.0, 1, 0, 0);
	glScalef(4.0, 4.0, 4.0);

	drawCheck(6, 6, WHITE, GRAY);  /* draw ground */
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 4, -4);
	glScalef(4.0, 4.0, 4.0);

	drawCheck(6, 6, WHITE, GRAY);  /* draw back */
	glPopMatrix();

	glPushMatrix();
	glTranslatef(4.0, 4, 0);
	glRotatef(-90.0, 0, 1, 0);
	glScalef(4.0, 4.0, 4.0);

	drawCheck(6, 6, WHITE, GRAY);  /* draw back */
	glPopMatrix();

	//draw body
	glPushMatrix();
	setColor(GREEN);
	glTranslatef(-1, 2, 1);
	glScaled(0.5, 0.3, 0.7);

	glutSolidCube(6);
	glPopMatrix();

	glPushMatrix();
	setColor(GREEN);
	glTranslatef(-1, 3.3, 1);
	glScaled(0.4, 0.3, 0.4);
	glutSolidCube(6);
	glPopMatrix();


	//draw wheels
	glPushMatrix();
	glTranslatef(-1, 1, 2.2);
	glScalef(6 * 0.27, 0.3, 0.3);
	glRotatef(-90, 0, 1, 0);
	drawTube(2, 2, 0, 16, 1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1, 1, -0.2);
	glScalef(6 * 0.27, 0.3, 0.3);
	glRotatef(-90, 0, 1, 0);
	drawTube(2, 2, 0, 16, 1);
	glPopMatrix();

	//draw windows
	glPushMatrix();
	setColor(BLUE);
	glTranslatef(-1, 3.3, 1);
	glScaled(0.43, 0.23, 0.33);
	glutSolidCube(6);
	glPopMatrix();

	glPushMatrix();
	setColor(BLUE);
	glTranslatef(-1, 3.3, 1);
	glScaled(0.33, 0.23, 0.43);
	glutSolidCube(6);
	glPopMatrix();

	//draw headlights
	glPushMatrix();
	setColor(YELLOW);
	glTranslatef(-2, 2.3, 3);
	glScaled(0.3, 0.3, 0.3);
	gluSphere(gluNewQuadric(), 0.7, 16, 16);
	glPopMatrix();

	glPushMatrix();
	setColor(YELLOW);
	glTranslatef(0, 2.3, 3);
	glScaled(0.3, 0.3, 0.3);
	gluSphere(gluNewQuadric(), 0.7, 16, 16);
	glPopMatrix();

	//draw bumper
	glPushMatrix();
	setColor(GRAY);
	glTranslatef(-1, 1.5, 1);
	glScaled(0.4, 0.05, 0.73);
	glutSolidCube(6);
	glPopMatrix();

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	glutSwapBuffers();

}


int main()
{
	int width = 500, height = 500;
	int i;
	char* name;

	glutInitWindowSize(width, height);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutCreateWindow("car");

	glutDisplayFunc(display);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 20.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(12, 1, 0, 0);
	glRotatef(75, 0, 1, 0);
	glTranslatef(6, -4.8, -3.8);

	//вид сбоку
	//glRotatef(45, 0, 1, 0);

	//вид спереди
	//glRotatef(-40, 0, 1, 0);

	//вид спереди
	//glRotatef(-55, 0, 1, 0);
	//glTranslatef(3, 2, 1);
	//glRotatef(15, 1, 0, 0);

	//вид сверху
	//glTranslatef(2, 3, 1);
	//glRotatef(-75, 0, 1, 0);
	//glRotatef(50, 1, 0, 0);

	glEnable(GL_DEPTH_TEST);

	if (useLighting) {
		glEnable(GL_LIGHTING);
	}
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpec);

	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glShadeModel(GL_SMOOTH);

	glClearColor(0.0, 0.0, 0.0, 1);
	glClearIndex(0);
	glClearDepth(1);

	glutMainLoop();
	return 0;
}