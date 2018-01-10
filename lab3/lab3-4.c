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
#define far 1000.0
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

Model *m, *m2, *m3,*m4,*m5,*m6,*m7, *m8, *m9;

vec3 myPosition;
vec3 myLookAt;

Point3D lightSourcesColorsArr[] = { {1.0f, 0.0f, 0.0f}, // Red light
                                 {0.0f, 1.0f, 0.0f}, // Green light
                                 {0.0f, 0.0f, 1.0f}, // Blue light
                                 {1.0f, 1.0f, 1.0f} }; // White light

GLfloat specularExponent[] = {10.0, 20.0, 60.0, 5.0};
GLint isDirectional[] = {0,0,1,1};

Point3D lightSourcesDirectionsPositions[] = { {10.0f, 5.0f, 0.0f}, // Red light, positional
                                       {0.0f, 5.0f, 10.0f}, // Green light, positional
                                       {-1.0f, 0.0f, 0.0f}, // Blue light along X
                                       {0.0f, 0.0f, -1.0f} }; // White light along Z

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
	program = loadShaders("lab3-4.vert", "lab3-4.frag");
	printError("init shader");
	m = LoadModelPlus("windmill/windmill-walls.obj");
	m2 = LoadModelPlus("windmill/windmill-roof.obj");
	m3 = LoadModelPlus("windmill/windmill-balcony.obj");
	m4 = LoadModelPlus("windmill/blade.obj");
	m5 = LoadModelPlus("windmill/blade.obj");
	m6 = LoadModelPlus("windmill/blade.obj");
	m7 = LoadModelPlus("windmill/blade.obj");
	m8 = LoadModelPlus("plane.obj");
  m9 = LoadModelPlus("skybox.obj");

  // projection
	projectionMatrix = frustum( left,  right,  bottom,  top,  near,  far);
	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);

	//camera
	myPosition.x = 20;
	myPosition.y = 5;
	myPosition.z = 0;

  myLookAt.x = -1;
	myLookAt.y = 0;
	myLookAt.z = 0;

	//multi light
	glUniform3fv(glGetUniformLocation(program, "lightSourcesDirPosArr"), 4, &lightSourcesDirectionsPositions[0].x);
	glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), 4, &lightSourcesColorsArr[0].x);
	glUniform1fv(glGetUniformLocation(program, "specularExponent"), 4, specularExponent);
	glUniform1iv(glGetUniformLocation(program, "isDirectional"), 4, isDirectional);

	// End of upload of geometry
	glutTimerFunc(20, &OnTimer, 0);

	printError("init arrays");
}

int oldx = -1;
int oldy = -1;

float pitchAngle = 0;
float yawAngle = 0;

void onMoveMouse(int x, int y)
{
	if(oldx != -1)
	{
			GLfloat deltax = x - oldx;
			GLfloat deltay = y - oldy;

			deltax *= 0.002f;
			deltay *= 0.002f;

			pitchAngle -= deltax;
			//yawAngle += deltay;

			myLookAt = MultVec3(Ry(pitchAngle),MultVec3(Rz(yawAngle), SetVector(-1.f,0.f,0.f)));
	}
	oldx = x;
	oldy = y;
}


void onMouse(int button, int state, int x, int y)
{
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{

	}
	else if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
	}
}

GLfloat walkSpeed = 0.5f;

void display(void)
{
	printError("pre display");
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);  t /= 500;
	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);

	if(glutKeyIsDown('w'))
	{
			vec3 v = ScalarMult(myLookAt, walkSpeed);
			myPosition = MultVec3(T(v.x, v.y, v.z), myPosition);
	}
	else if(glutKeyIsDown('s'))
	{
		vec3 v = ScalarMult(myLookAt, -walkSpeed);
		myPosition = MultVec3(T(v.x, v.y, v.z), myPosition);
	}
	else if(glutKeyIsDown('a'))
	{
		vec3 v = MultVec3(Ry(1.57f),ScalarMult(myLookAt, walkSpeed));
		myPosition = MultVec3(T(v.x, v.y, v.z), myPosition);
	}
	else if(glutKeyIsDown('d'))
	{
		vec3 v = MultVec3(Ry(-1.57f),ScalarMult(myLookAt, walkSpeed));
		myPosition = MultVec3(T(v.x, v.y, v.z), myPosition);
	}

  mat4 rot, trans, scale ,total;

	glutPassiveMotionFunc(onMoveMouse);
	glutMouseFunc(onMouse);

  //camera
  mat4 cameraMatrix;
	vec3 pVec = myPosition;
	vec3 lVec = VectorAdd(myPosition, myLookAt);
	vec3 vVec = {0,1,0};
	cameraMatrix = lookAtv(pVec, lVec, vVec);
	glUniform3f(glGetUniformLocation(program, "viewpos"), pVec.x, pVec.y, pVec.z);
	glUniformMatrix4fv(glGetUniformLocation(program, "cameraMatrix"), 1, GL_TRUE, cameraMatrix.m);

  //skybox
	glDisable(GL_DEPTH_TEST);
  glUniform1i(glGetUniformLocation(program, "needTexture"), 1);
	glUniform1i(glGetUniformLocation(program, "isSkybox"), 1);
	LoadTGATextureSimple("SkyBox512.tga", &myTex); // load texture
	glBindTexture(GL_TEXTURE_2D, myTex);
	glUniform1i(glGetUniformLocation(program, "texUnit"), 0);
	glActiveTexture(GL_TEXTURE0);

	mat4 skyboxCameraMatrix = cameraMatrix;
	skyboxCameraMatrix.m[3] = 0;
	skyboxCameraMatrix.m[7] = 0;
	skyboxCameraMatrix.m[11] = 0;
	//skyboxCameraMatrix.m[12] = 0;
	//skyboxCameraMatrix.m[13] = 0;
	//skyboxCameraMatrix.m[14] = 0;

	glUniformMatrix4fv(glGetUniformLocation(program, "skyboxCameraMatrix"), 1, GL_TRUE, skyboxCameraMatrix.m);
	trans = T(0,-15,0);
	rot = Ry(0);
	scale = S(20.f, 20.f ,20.f);
	total = Mult(Mult(trans, rot),scale);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
  DrawModel(m9, program, "in_Position", "in_Normal", "inTexCoord");

	//ground plane
	glUniform1i(glGetUniformLocation(program, "isSkybox"), 0);
	glEnable(GL_DEPTH_TEST);
	LoadTGATextureSimple("grass.tga", &myTex); // load texture
	glBindTexture(GL_TEXTURE_2D, myTex);
	glUniform1i(glGetUniformLocation(program, "texUnit"), 0);
	glActiveTexture(GL_TEXTURE0);

	trans = T(0, 0, 0);
	rot = Ry(0);
	scale = S(50.f, 50.f ,50.f);
	total = Mult(Mult(trans, rot),scale);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
  DrawModel(m8, program, "in_Position", "in_Normal", "inTexCoord");

	// windmill
	glUniform1i(glGetUniformLocation(program, "needTexture"), 0);
	mat4 windmillMdlMatrix;
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
