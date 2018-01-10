// Lab 1-1.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-1.frag", "lab1-1.vert" are required.

// Should work as is on Linux and Mac. MS Windows needs GLEW or glee.
// See separate Visual Studio version of my demos.
#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include <math.h>
#include "loadobj.h"
#include "VectorUtils3.h"
// Globals

// vertex array object
unsigned int bunnyVertexArrayObjID;
unsigned int bunnyVertexBufferObjID;
unsigned int bunnyIndexBufferObjID;
unsigned int bunnyNormalBufferObjID;
unsigned int bunnyTexCoordBufferObjID;
GLuint program;
GLuint myTex;

#define near 1.0
#define far 30.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5
mat4 projectionMatrix;

void OnTimer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(20, &OnTimer, value);
}

Model *m, *m2, *m3,*m4,*m5,*m6,*m7;

void init(void)
{
	dumpInfo();

  // GL inits
	glClearColor(0.2,0.2,0.5,0);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	//glDisable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab3-1.vert", "lab3-1.frag");
	printError("init shader");
	m = LoadModelPlus("windmill/windmill-walls.obj");
	m2 = LoadModelPlus("windmill/windmill-roof.obj");
	m3 = LoadModelPlus("windmill/windmill-balcony.obj");
	m4 = LoadModelPlus("windmill/blade.obj");
	m5 = LoadModelPlus("windmill/blade.obj");
	m6 = LoadModelPlus("windmill/blade.obj");
	m7 = LoadModelPlus("windmill/blade.obj");

	// camera position
	vec3 pVec = {20,18,0};
	vec3 lVec = {0,9,0};
	vec3 vVec = {0,1,0};
	mat4 cameraMatrix = lookAtv(pVec, lVec, vVec);
	glUniform3f(glGetUniformLocation(program, "viewpos"), pVec.x, pVec.y, pVec.z);
	glUniformMatrix4fv(glGetUniformLocation(program, "cameraMatrix"), 1, GL_TRUE, cameraMatrix.m);

  // projection
	projectionMatrix = frustum( left,  right,  bottom,  top,  near,  far);
	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);

	// End of upload of geometry
	glutTimerFunc(20, &OnTimer, 0);

	printError("init arrays");
}

int oldx = 0;
int oldy = 0;
static float delta = 0;
static float heigh = 0;

void onDrag(int x, int y)
{
	// camera position
	delta += x - oldx;
	oldx = x;

	heigh += y - oldy;
	oldy = y;

	if(heigh > 1000)
		heigh = 1000;
	else if(heigh < -1000)
		heigh = -1000;

	vec3 pVec = {20 * cos(delta / 500), 18, 20 * sin(delta / 500)};
	vec3 lVec = {0,9 + heigh / 100,0};
	vec3 vVec = {0,1,0};
	mat4 cameraMatrix = lookAtv(pVec, lVec, vVec);
	glUniform3f(glGetUniformLocation(program, "viewpos"), pVec.x, pVec.y, pVec.z);
	glUniformMatrix4fv(glGetUniformLocation(program, "cameraMatrix"), 1, GL_TRUE, cameraMatrix.m);
}

void onMouse(int button, int state, int x, int y)
{
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		oldx = x;
		oldy = y;
	}
	else if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{

	}
}

void display(void)
{
	printError("pre display");

	glutMotionFunc(onDrag);
	glutMouseFunc(onMouse);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);  t /= 500;

	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);

	// texture
	/*
	LoadTGATextureSimple("SkyBox512.tga", &myTex); // load texture
	glBindTexture(GL_TEXTURE_2D, myTex);
	glUniform1i(glGetUniformLocation(program, "texUnit"), 0);
	glActiveTexture(GL_TEXTURE0);
  */
	mat4 rot, trans, scale ,total;
	mat4 windmillMdlMatrix;

	// windmill
	trans = T(0, 0, 0);
	rot = Ry(0);
	scale = S(1.f, 1.f ,1.f);
	total = Mult(Mult(trans, rot),scale);

	windmillMdlMatrix = total;

	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
  DrawModel(m, program, "in_Position", "in_Normal", "inTexCoord");
  DrawModel(m2, program, "in_Position", "in_Normal", "inTexCoord");
  DrawModel(m3, program, "in_Position", "in_Normal", "inTexCoord");

	// blade
	int i;
	for(i = 0; i < 4; i++)
	{
		trans = T(0, 0, 0);
		rot = Rx(i * 1.57);
		scale = S(0.8f, 0.8f ,0.8f);
		total = Mult(Mult(trans,rot),scale);

    trans = T(4.5f, 9.2f, 0);
		rot = Rx(t);
		scale = S(1.f, 1.f ,1.f);
	  total = Mult(Mult(Mult(trans,rot),scale) ,total);

    total = Mult(windmillMdlMatrix,total);

		glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
		switch(i)
		{
			case 0: DrawModel(m4, program, "in_Position", "in_Normal", "inTexCoord"); break;
			case 1: DrawModel(m5, program, "in_Position", "in_Normal", "inTexCoord"); break;
			case 2: DrawModel(m6, program, "in_Position", "in_Normal", "inTexCoord"); break;
			case 3: DrawModel(m7, program, "in_Position", "in_Normal", "inTexCoord"); break;
		}
	}
	printError("display");
	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("GL3 white triangle example");
	glutDisplayFunc(display);
	init ();
	glutMainLoop();
	return 0;
}
