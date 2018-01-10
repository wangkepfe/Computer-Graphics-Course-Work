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

// Globals

//front side 1
GLfloat vertices[] = {-0.5f, 0.5f, 0.5f,
	                    -0.5f, -0.5f, 0.5f,
										  0.5f, -0.5f, 0.5f,
//front side 2
 											-0.5f, 0.5f, 0.5f,
	                    0.5f, 0.5f, 0.5f,
										  0.5f, -0.5f, 0.5f,
//back side 1
 											-0.5f, 0.5f, -0.5f,
	                    -0.5f, -0.5f, -0.5f,
										  0.5f, -0.5f, -0.5f,
//back side 2
 											-0.5f, 0.5f, -0.5f,
	                    0.5f, 0.5f, -0.5f,
										  0.5f, -0.5f, -0.5f,
//left side 1
                      -0.5f, 0.5f, -0.5f,
	                    -0.5f, -0.5f, 0.5f,
										  -0.5f, -0.5f, -0.5f,
//left side 2
                      -0.5f, 0.5f, -0.5f,
	                    -0.5f, -0.5f, 0.5f,
										  -0.5f, 0.5f, 0.5f,
//right side 1
                      0.5f, 0.5f, -0.5f,
	                    0.5f, -0.5f, 0.5f,
										  0.5f, -0.5f, -0.5f,
//right side 2
                      0.5f, 0.5f, -0.5f,
	                    0.5f, -0.5f, 0.5f,
										  0.5f, 0.5f, 0.5f,
//up side 1
                      -0.5f, 0.5f, -0.5f,
	                    -0.5f, 0.5f, 0.5f,
										  0.5f, 0.5f, -0.5f,
//up side 2
                      0.5f, 0.5f, 0.5f,
	                    -0.5f, 0.5f, 0.5f,
										  0.5f, 0.5f, -0.5f,
//down side 1
											-0.5f, -0.5f, -0.5f,
	                    -0.5f, -0.5f, 0.5f,
										  0.5f, -0.5f, -0.5f,
//down side 2
                      0.5f, -0.5f, 0.5f,
	                    -0.5f, -0.5f, 0.5f,
										  0.5f, -0.5f, -0.5f};

											//front side 1
											GLfloat colorMatrix[] = {1.0f, 0.0f, 0.0f,
												                    1.0f, 0.0f, 0.0f,
																					  1.0f, 0.0f, 0.0f,
											//front side 2
																						1.0f, 0.0f, 0.0f,
																						1.0f, 0.0f, 0.0f,
																						1.0f, 0.0f, 0.0f,
											//back side 1
																						0.0f, 0.0f, 1.0f,
																						0.0f, 0.0f, 1.0f,
																						0.0f, 0.0f, 1.0f,
											//back side 2
											0.0f, 0.0f, 1.0f,
											0.0f, 0.0f, 1.0f,
											0.0f, 0.0f, 1.0f,
											//left side 1
											1.0f, 1.0f, 0.0f,
											1.0f, 1.0f, 0.0f,
											1.0f, 1.0f, 0.0f,
											//left side 2
											1.0f, 1.0f, 0.0f,
											1.0f, 1.0f, 0.0f,
											1.0f, 1.0f, 0.0f,
											//right side 1
											1.0f, 1.0f, 0.0f,
											1.0f, 0.0f, 1.0f,
											0.0f, 1.0f, 1.0f,
											//right side 2
											1.0f, 1.0f, 0.0f,
											1.0f, 0.0f, 1.0f,
											0.0f, 1.0f, 1.0f,
											//up side 1
											1.0f, 1.0f, 0.0f,
											1.0f, 0.0f, 1.0f,
											0.0f, 1.0f, 1.0f,
											//up side 2
											1.0f, 1.0f, 0.0f,
											1.0f, 0.0f, 1.0f,
											0.0f, 1.0f, 1.0f,
											//down side 1
											1.0f, 1.0f, 0.0f,
											1.0f, 0.0f, 1.0f,
											0.0f, 1.0f, 1.0f,
											//down side 2
											1.0f, 1.0f, 0.0f,
											1.0f, 0.0f, 1.0f,
											0.0f, 1.0f, 1.0f};

// vertex array object
unsigned int vertexArrayObjID;
GLuint program;

void OnTimer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(20, &OnTimer, value);
}

void init(void)
{
	// vertex buffer object, used for uploading the geometry
  //	unsigned int vertexBufferObjID;
	// Reference to shader program

	unsigned int vertexBufferObjID;
	unsigned int colorBufferObjID;

	dumpInfo();

	// GL inits
	glClearColor(0.2,0.2,0.5,0);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	//glDisable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab1-4.vert", "lab1-4.frag");
	printError("init shader");



	// Upload geometry to the GPU:

	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &vertexArrayObjID); // VAO
	glBindVertexArray(vertexArrayObjID);
	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &vertexBufferObjID);

	// VBO for vertex dataglUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, myMatrix);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 108*sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));

// color
	glGenBuffers(1, &colorBufferObjID);
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 108*sizeof(GLfloat), colorMatrix, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Color"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Color"));

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

	GLfloat myMatrix[] = {  cos(t), cos(t),  sin(t), 0.0f,
                        cos(t),     1.0f,  0.0f, 0.0f,
                        -sin(t),   0.0f,    cos(t), 0.0f,
                        0.0f,    0.0f,    0.0f, 1.0f };

	//
  glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, myMatrix);

	glBindVertexArray(vertexArrayObjID);
	glDrawArrays(GL_TRIANGLES, 0, 36);	// draw object

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
