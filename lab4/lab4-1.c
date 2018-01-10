// Lab 4, terrain generation

#ifdef __APPLE__
#include <OpenGL/gl3.h>
// Linking hint for Lightweight IDE
// uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"

#define CAM_POSITION_X 20
#define CAM_POSITION_Y 20
#define CAM_POSITION_Z 10

///////////////////////////////////////////// G L O B A L ///////////////////////////////////////////////
mat4 projectionMatrix;
///////////////////////////////////////////// G L O B A L ///////////////////////////////////////////////
GLfloat terrainScale = 1.f;
GLfloat terrainHeightRatio = 10.0f;
GLfloat terrainTexRatio = 10.0f;
///////////////////////////////////////////// G L O B A L ///////////////////////////////////////////////
TextureData ttex; // terrain
GLfloat *vertexArray;
///////////////////////////////////////////// G L O B A L ///////////////////////////////////////////////
GLfloat texHeightRamp[4] = {5,20,30,40};

vec3 getVertexforNormal(int x,int z)
{
	vec3 v;
	TextureData *myTex = &ttex;
	v.x = x * terrainScale;
	if(x < 0 || z < 0 || x >= myTex->width || z >= myTex->height)
		v.y = 0;
	else
		v.y = myTex->imageData[(x + z * myTex->width) * (myTex->bpp/8)] / 100 * terrainHeightRatio;
	v.z = z * terrainScale;
	return v;
}

vec3 getVertexNormalValue(int x,int z)
{
	vec3 resultv,v1,v2,v3,v4,v5,v6;
	//triangle no.1
	v1 = CalcNormalVector(getVertexforNormal(x,z),
	getVertexforNormal(x-1,z),
	getVertexforNormal(x-1,z+1));
	//triangle no.2
	v2 = CalcNormalVector(getVertexforNormal(x,z),
	getVertexforNormal(x-1,z+1),
	getVertexforNormal(x,z+1));
	//triangle no.3
	v3 = CalcNormalVector(getVertexforNormal(x,z),
	getVertexforNormal(x,z+1),
	getVertexforNormal(x+1,z));
	//triangle no.4
	v4 = CalcNormalVector(getVertexforNormal(x,z),
	getVertexforNormal(x+1,z),
	getVertexforNormal(x+1,z-1));
	//triangle no.5
	v5 = CalcNormalVector(getVertexforNormal(x,z),
	getVertexforNormal(x+1,z-1),
	getVertexforNormal(x,z-1));
	//triangle no.6
	v6 = CalcNormalVector(getVertexforNormal(x,z),
	getVertexforNormal(x,z-1),
	getVertexforNormal(x-1,z));

	//weighed
	resultv.x = (v1.x + v2.x + v3.x * 2 + v4.x + v5.x + v6.x * 2) / 8;
	resultv.y = (v1.y + v2.y + v3.y * 2 + v4.y + v5.y + v6.y * 2) / 8;
	resultv.z = (v1.z + v2.z + v3.z * 2 + v4.z + v5.z + v6.z * 2) / 8;
	return resultv;
}

Model* GenerateTerrain()
{
	TextureData *tex = &ttex;
	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width-1) * (tex->height-1) * 2;
	int x, z;

	vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount*3);

	printf("bpp %d\n", tex->bpp);
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
			// Vertex array. You need to scale this properly
			vertexArray[(x + z * tex->width)*3 + 0] = x * terrainScale;
			vertexArray[(x + z * tex->width)*3 + 1] = tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / 100.f * terrainHeightRatio;
			vertexArray[(x + z * tex->width)*3 + 2] = z * terrainScale;
			// Normal vectors. You need to calculate these.
			vec3 v = getVertexNormalValue(x,z);
			normalArray[(x + z * tex->width)*3 + 0] = v.x;
			normalArray[(x + z * tex->width)*3 + 1] = v.y;
			normalArray[(x + z * tex->width)*3 + 2] = v.z;
			/*normalArray[(x + z * tex->width)*3 + 0] = 0;
			normalArray[(x + z * tex->width)*3 + 1] = 1;
			normalArray[(x + z * tex->width)*3 + 2] = 0;*/
			// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * tex->width)*2 + 0] = (float)x / tex->width * terrainTexRatio;
			texCoordArray[(x + z * tex->width)*2 + 1] = (float)z / tex->height * terrainTexRatio;
		}
	for (x = 0; x < tex->width-1; x++)
		for (z = 0; z < tex->height-1; z++)
		{
			// Triangle 1
			indexArray[(x + z * (tex->width-1))*6 + 0] = x + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 1] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 2] = x+1 + z * tex->width;
			// Triangle 2
			indexArray[(x + z * (tex->width-1))*6 + 3] = x+1 + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 4] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 5] = x+1 + (z+1) * tex->width;
		}

	//
	Model* model = LoadDataToModel(
		vertexArray,
		normalArray,
		texCoordArray,
		NULL,
		indexArray,
		vertexCount,
		triangleCount*3);

	return model;
}

GLfloat getHeight(GLfloat xIn, GLfloat zIn)
{
	TextureData* tex = &ttex;
	GLfloat s = terrainScale;
	GLfloat d;
	GLfloat y = 0;
	vec3 A, B, C, n;
	xIn /= s;
	zIn /= s;
	int x, z;
	for (x = 0; x < tex->width - 1; x++)
	{
		for (z = 0; z < tex->height - 1; z++)
		{
			// 1 : quad
			if(z <= zIn && zIn < z + 1 && x <= xIn && xIn < x + 1)
			{
				// 2 : triangle
				if(zIn - z < -(xIn - x) + 1)
				{
					A = SetVector(x * s, vertexArray[(int)(x + z * tex->width)*3 + 1], z * s);
					B = SetVector((x + 1) * s, vertexArray[(int)(x + 1 + z * tex->width)*3 + 1], z * s);
					C = SetVector(x * s, vertexArray[(int)(x + (z + 1) * tex->width)*3 + 1], (z + 1) * s);
				}
				else
				{
					A = SetVector((x + 1) * s, vertexArray[(int)(x + 1 + (z + 1) * tex->width)*3 + 1], (z + 1) * s);
					B = SetVector((x + 1) * s, vertexArray[(int)(x + 1 + z * tex->width)*3 + 1], z * s);
					C = SetVector(x * s, vertexArray[(int)(x + (z + 1) * tex->width)*3 + 1], (z + 1) * s);
				}
				// 3 : height
				n = CalcNormalVector(A,B,C);
				d = -n.x * A.x - n.y * A.y - n.z * A.z ; // ax + by + cz + d = 0; a = n.x; n = (a,b,c)
				xIn *= s;
				zIn *= s;
				y = -(n.x * xIn + n.z * zIn + d) / n.y;
				break;
			}
		}
	}
	return y;
}

///////////////////////////////////////////// G L O B A L ///////////////////////////////////////////////

// vertex array object
Model *tm;
// Reference to shader program
GLuint program;
GLuint tex0, tex1, tex2;

///////////////////////////////////////////// G L O B A L ///////////////////////////////////////////////

vec3 myPosition;
vec3 myLookAt;

GLfloat walkSpeed = 5.f;

int oldx = -1;
int oldy = -1;

float pitchAngle = 0;
float yawAngle = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////

Model *model01;

void init(void)
{
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("terrain.vert", "terrain.frag");
	glUseProgram(program);
	printError("init shader");

  //projection
	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 500.0);
	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

	//texture
	LoadTGATextureSimple("grass.tga", &tex0);
	LoadTGATextureSimple("Cliff.tga", &tex1);
	LoadTGATextureSimple("Snow.tga", &tex2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex0);
	glUniform1i(glGetUniformLocation(program, "tex0"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex1);
	glUniform1i(glGetUniformLocation(program, "tex1"), 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, tex2);
	glUniform1i(glGetUniformLocation(program, "tex2"), 2);


	// Load terrain data
	LoadTGATextureData("fft-terrain.tga", &ttex);
	tm = GenerateTerrain(&ttex);
	printError("init terrain");

	// texture height ramp
	glUniform1fv(glGetUniformLocation(program, "heightRamp"),4, texHeightRamp);

	// load model
	model01 = LoadModelPlus("groundsphere.obj");

	//camera
	myPosition.x = CAM_POSITION_X;
	myPosition.y = CAM_POSITION_Y;
	myPosition.z = CAM_POSITION_Z;

	myLookAt.x = -1;
	myLookAt.y = 0;
	myLookAt.z = 0;
}

void keyboardMoving()
{
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
	else if(glutKeyIsDown(' '))
	{
		vec3 v = SetVector(0, walkSpeed, 0);
		myPosition = MultVec3(T(v.x, v.y, v.z), myPosition);
	}
	else if(glutKeyIsDown('v'))
	{
		vec3 v = SetVector(0, -walkSpeed, 0);
		myPosition = MultVec3(T(v.x, v.y, v.z), myPosition);
	}
}

void onMoveMouse(int x, int y)
{
	if(oldx != -1)
	{
		GLfloat deltax = x - oldx;
		GLfloat deltay = y - oldy;

		deltax *= 0.005f;
		deltay *= 0.005f;

		pitchAngle -= deltax;
		yawAngle += deltay;

		myLookAt = MultVec3(Ry(pitchAngle),MultVec3(Rz(yawAngle), SetVector(-1.f,0.f,0.f)));
	}
	oldx = x;
	oldy = y;
}

GLfloat t;

void display(void)
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	printError("pre display");
	glUseProgram(program);
	t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	t /= 500;


	///////////////////////////////////////////////////////////////////
  mat4 rot, trans, scale ,total, camMatrix;

	GLfloat mx, mz;
	//test zone
	mx = 30 + 20 * sin(t/5);
	mz = 30 + 20 * sin(t/5);

	//user moving
	keyboardMoving();
	glutPassiveMotionFunc(onMoveMouse);

  //camera
	vec3 cam = myPosition;
	glUniform3f(glGetUniformLocation(program, "viewpos"), cam.x, cam.y, cam.z);
	vec3 lookAtPoint = VectorAdd(myPosition, myLookAt);
	camMatrix = lookAt(cam.x, cam.y, cam.z,
		lookAtPoint.x, lookAtPoint.y, lookAtPoint.z,
		0.0, 1.0, 0.0);
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, camMatrix.m);

  //terrain
	//trans = T(-myTex->width * terrainScale / 2, 0, -myTex->height * terrainScale / 2);
	trans = T(0, 0, 0);
	rot = Ry(0);
	scale = S(1.f, 1.f ,1.f);
	total = Mult(Mult(trans, rot),scale);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");

	// model : sphere
	trans = T(mx, getHeight(mx,mz), mz);
	rot = Ry(0);
	scale = S(5.f, 5.f ,5.f);
	total = Mult(Mult(trans, rot),scale);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(model01, program, "inPosition", "inNormal", "inTexCoord");

	//light position
	glUniform3f(glGetUniformLocation(program, "lightpos"), 50 ,70, 70);

  ///////////////////////////////////////////////////////////////////////
	printError("display 2");
	glutSwapBuffers();
}

void timer(int i)
{
	glutTimerFunc(20, &timer, i);
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize (600, 600);
	glutCreateWindow ("TSBK07 Lab 4");
	glutDisplayFunc(display);
	init ();
	glutTimerFunc(20, &timer, 0);
	glutMainLoop();
	exit(0);
}
