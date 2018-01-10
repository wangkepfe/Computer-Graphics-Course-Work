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
// Globals

// vertex array object
unsigned int bunnyVertexArrayObjID;
unsigned int bunnyVertexBufferObjID;
unsigned int bunnyIndexBufferObjID;
unsigned int bunnyNormalBufferObjID;
unsigned int bunnyTexCoordBufferObjID;
GLuint program;
GLuint myTex;

void OnTimer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(20, &OnTimer, value);
}

Model *m;

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
	program = loadShaders("lab2-2.vert", "lab2-2.frag");
	printError("init shader");

  m = LoadModel("bunnyplus.obj");

  glGenVertexArrays(1, &bunnyVertexArrayObjID);
  glGenBuffers(1, &bunnyVertexBufferObjID);
  glGenBuffers(1, &bunnyIndexBufferObjID);
  glGenBuffers(1, &bunnyNormalBufferObjID);

  glBindVertexArray(bunnyVertexArrayObjID);

  glBindBuffer(GL_ARRAY_BUFFER, bunnyVertexBufferObjID);
  glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->vertexArray, GL_STATIC_DRAW);
  glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));

  glBindBuffer(GL_ARRAY_BUFFER, bunnyNormalBufferObjID);
  glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->normalArray, GL_STATIC_DRAW);
  glVertexAttribPointer(glGetAttribLocation(program, "in_Normal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(glGetAttribLocation(program, "in_Normal"));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bunnyIndexBufferObjID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->numIndices*sizeof(GLuint), m->indexArray, GL_STATIC_DRAW);

  // texture
	LoadTGATextureSimple("maskros512.tga", &myTex); // load texture
	glGenBuffers(1, &bunnyTexCoordBufferObjID);
	glBindTexture(GL_TEXTURE_2D, myTex);
	glUniform1i(glGetUniformLocation(program, "texUnit"), 0);
	glActiveTexture(GL_TEXTURE0);

    if (m->texCoordArray != NULL)
    {
        glBindBuffer(GL_ARRAY_BUFFER, bunnyTexCoordBufferObjID);
        glBufferData(GL_ARRAY_BUFFER, m->numVertices*2*sizeof(GLfloat), m->texCoordArray, GL_STATIC_DRAW);
        glVertexAttribPointer(glGetAttribLocation(program, "inTexCoord"), 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(glGetAttribLocation(program, "inTexCoord"));
    }

	// End of upload of geometry
	glutTimerFunc(20, &OnTimer, 0);

	printError("init arrays");
}

void display(void)
{
	printError("pre display");



	// clear the screen
//	glClearColor(0.2f ,0.3f ,0.3f ,1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

	t /= 500;

	GLfloat myMatrix[] = {  cos(t), 0.0f,  sin(t), 0.0f,
                        0.0f,     1.0f,  0.0f, 0.0f,
                        -sin(t),   0.0f,    cos(t), 0.0f,
                        0.0f,    0.0f,    0.0f, 1.0f };

	//
  glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, myMatrix);

	printError("display");

  glBindVertexArray(bunnyVertexArrayObjID);
  glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L);

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
