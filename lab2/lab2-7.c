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

Model *m, *m2;

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
	program = loadShaders("lab2-7.vert", "lab2-7.frag");
	printError("init shader");
  m2 = LoadModelPlus("bilskiss.obj");
	m = LoadModelPlus("cubeplus.obj");

  // projection
	projectionMatrix = frustum( left,  right,  bottom,  top,  near,  far);
	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);

	// End of upload of geometry
	glutTimerFunc(20, &OnTimer, 0);

	printError("init arrays");
}

void display(void)
{
	printError("pre display");
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);  t /= 500;

	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);

	// camera position move
	vec3 pVec = {4 * cos(t * 0.2f),2, 4 * sin(t * 0.2f)};
	vec3 lVec = {0,0,0};
	vec3 vVec = {0,1,0};
	mat4 cameraMatrix = lookAtv(pVec, lVec, vVec);
	glUniform3f(glGetUniformLocation(program, "viewpos"), pVec.x, pVec.y, pVec.z);
	glUniformMatrix4fv(glGetUniformLocation(program, "cameraMatrix"), 1, GL_TRUE, cameraMatrix.m);

	// texture
	LoadTGATextureSimple("dirt.tga", &myTex); // load texture
	glBindTexture(GL_TEXTURE_2D, myTex);
	glUniform1i(glGetUniformLocation(program, "texUnit"), 0);
	glActiveTexture(GL_TEXTURE0);

	// model to view for model 1
	mat4 rot, trans, total, scale;
	trans = T(cos(t), 0, 1);
	rot = Ry(t * 0.5f);
	scale = S(1.0f, 1.0f ,1.0f);
	total = Mult(Mult(rot, trans),scale);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);

  // draw model 1
  DrawModel(m, program, "in_Position", "in_Normal", "inTexCoord");

	// texture
	LoadTGATextureSimple("dirt.tga", &myTex); // load texture
	glBindTexture(GL_TEXTURE_2D, myTex);
	glUniform1i(glGetUniformLocation(program, "texUnit"), 0);
	glActiveTexture(GL_TEXTURE0);

	// model to view for model 2
	trans = T(0, sin(t), -1);
	rot = Rx(t * 0.8f);
	total = Mult(rot, trans);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);

  // draw model 2
  DrawModel(m2, program, "in_Position", "in_Normal", "inTexCoord");

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
