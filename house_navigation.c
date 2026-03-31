#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif

// Structs
typedef struct {
	float x, y, z;
} Point3D;

typedef struct {
	float rotX, rotY, rotZ;
	float posX;
} HouseState;

// Global Variables
const float RED[3] = {1.0f, 0.41f, 0.41f};
const float GREEN[3] = {0.41f, 1.0f, 0.41f};
const float BROWN[3] = {0.55f, 0.27f, 0.07f};
const float GOLD[3] = {1.0f, 0.84f, 0.0f};
const float PURPLE[3] = {1.0f, 0.41f, 1.0f};
const float WHITE[3] = {1.0f, 1.0f, 1.0f};
const float BLACK_60[4] = {0.0f, 0.0f, 0.0f, 0.6f};
const float GREY[3] = {0.5f, 0.5f, 0.5f};
const float YELLOW[3] = {1.0f, 1.0f, 0.41f};

GLfloat angle, fAspect;
int winW = 400, winH = 400;
int selectedHouse = 0;

float camX = 0.0f, camZ = 200.0f;
float camYaw = -90.0f;
float camSpeed = 5.0f;
float rotSpeed = 3.0f;

HouseState houses[2] = {
	{0.0f, 0.0f, 0.0f, -60.0f},
	{0.0f, 0.0f, 0.0f,  60.0f}
};

void EspecificaParametrosVisualizacao (void){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, fAspect, 1.0f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float lx = cos(camYaw * M_PI/180.0f);
	float lz = sin(camYaw * M_PI/180.0f);
	float eyeLevel = -5.0f;

	gluLookAt(camX, eyeLevel, camZ, camX + lx, eyeLevel, camZ + lz, 0.0f, 1.0f, 0.0f);
}

void ApplyNormal(Point3D v1, Point3D v2, Point3D v3) {
	float U[3], V[3], N[3];
	float length;

	U[0] = v2.x - v1.x;
	U[1] = v2.y - v1.y;
	U[2] = v2.z - v1.z;

	V[0] = v3.x - v1.x;
	V[1] = v3.y - v1.y;
	V[2] = v3.z - v1.z;

	N[0] = (U[1] * V[2]) - (U[2] * V[1]);
	N[1] = (U[2] * V[0]) - (U[0] * V[2]);
	N[2] = (U[0] * V[1]) - (U[1] * V[0]);

	length = sqrt(N[0]*N[0] + N[1]*N[1] + N[2]*N[2]);
	if (length == 0.0f) length = 1.0f;

	N[0] /= length;
	N[1] /= length;
	N[2] /= length;

	glNormal3f(N[0], N[1], N[2]);
}

void DrawFloor (void){
	
	glColor3fv(GREY);

	glBegin(GL_QUADS);
		glNormal3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-10000.0f, -25.1f, -10000.0f);
        glVertex3f(-10000.0f, -25.1f,  10000.0f);
        glVertex3f( 10000.0f, -25.1f,  10000.0f);
        glVertex3f( 10000.0f, -25.1f, -10000.0f);
    glEnd();
}

void DrawHouse (const float * baseColour){
	
	// House Base
	glColor3fv(baseColour);
		
	glBegin(GL_QUADS);	// Front
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(25.0, 25.0, 50.0);
		glVertex3f(-25.0, 25.0, 50.0);
		glVertex3f(-25.0, -25.0, 50.0);
		glVertex3f(25.0, -25.0, 50.0);
	glEnd();

	glBegin(GL_QUADS);	// Back
		glNormal3f(0.0, 0.0, -1.0);
		glVertex3f(25.0, 25.0, -50.0);
		glVertex3f(25.0, -25.0, -50.0);
		glVertex3f(-25.0, -25.0, -50.0);
		glVertex3f(-25.0, 25.0, -50.0);
	glEnd();
	
	glBegin(GL_QUADS);	// Left
		glNormal3f(-1.0, 0.0, 0.0);
		glVertex3f(-25.0, 25.0, 50.0);
		glVertex3f(-25.0, 25.0, -50.0);
		glVertex3f(-25.0, -25.0, -50.0);
		glVertex3f(-25.0, -25.0, 50.0);
	glEnd();

	glBegin(GL_QUADS);	// Right
		glNormal3f(1.0, 0.0, 0.0);
		glVertex3f(25.0, 25.0, 50.0);
		glVertex3f(25.0, -25.0, 50.0);
		glVertex3f(25.0, -25.0, -50.0);
		glVertex3f(25.0, 25.0, -50.0);
	glEnd();

	glBegin(GL_QUADS);	// Top
		glNormal3f(0.0, 1.0, 0.0);
		glVertex3f(-25.0, 25.0, -50.0);
		glVertex3f(-25.0, 25.0, 50.0);
		glVertex3f(25.0, 25.0, 50.0);
		glVertex3f(25.0, 25.0, -50.0);
	glEnd();

	glBegin(GL_QUADS);	// Bottom
		glColor3fv(YELLOW);
		glNormal3f(0.0, -1.0, 0.0);
		glVertex3f(-25.0, -25.0, -50.0);
		glVertex3f(25.0, -25.0, -50.0);
		glVertex3f(25.0, -25.0, 50.0);
		glVertex3f(-25.0, -25.0, 50.0);
	glEnd();

	// Roof
	glColor3fv(RED);

	glBegin(GL_TRIANGLES); // Back
		glNormal3f(0.0, 0.0, -1.0);
		glVertex3f(30.0f, 25.0f, -52.0f);
		glVertex3f(-30.0f, 25.0f, -52.0f);
		glVertex3f(0.0f, 60.0f, -52.0f);
	glEnd();

	glBegin(GL_TRIANGLES); // Front
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(30.0f, 25.0f, 52.0f);
		glVertex3f(-30.0f, 25.0f, 52.0f);
		glVertex3f(0.0f, 60.0f, 52.0f);
	glEnd();

	glBegin(GL_QUADS);	// Left
		Point3D p1 = {0.0f, 60.0f, -52.0f};
		Point3D p2 = {-30.0f, 25.0f, -52.0f};
		Point3D p3 = {-30.0f, 25.0f, 52.0f};
		Point3D p4 = {0.0f, 60.0f, 52.0f};
		
		ApplyNormal(p1, p2, p3);
		glVertex3f(p1.x, p1.y, p1.z);
		glVertex3f(p2.x, p2.y, p2.z);
		glVertex3f(p3.x, p3.y, p3.z);
		glVertex3f(p4.x, p4.y, p4.z);
	glEnd();

	glBegin(GL_QUADS);	// Right
		Point3D p5 = {0.0f, 60.0f, -52.0f};
		Point3D p6 = {30.0f, 25.0f, -52.0f};
		Point3D p7 = {30.0f, 25.0f, 52.0f};
		Point3D p8 = {0.0f, 60.0f, 52.0f};
		
		ApplyNormal(p7, p6, p5);
		glVertex3f(p5.x, p5.y, p5.z);
		glVertex3f(p6.x, p6.y, p6.z);
		glVertex3f(p7.x, p7.y, p7.z);
		glVertex3f(p8.x, p8.y, p8.z);
	glEnd();

	// Door
	glColor3fv(BROWN);
	
	glBegin(GL_QUADS);
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(10.0, 5.0, 50.1);
		glVertex3f(-10.0, 5.0, 50.1);
		glVertex3f(-10.0, -25.0, 50.1);
		glVertex3f(10.0, -25.0, 50.1);
	glEnd();

	glBegin(GL_QUADS);
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(10.0, 5.0, 49.9);
		glVertex3f(-10.0, 5.0, 49.9);
		glVertex3f(-10.0, -25.0, 49.9);
		glVertex3f(10.0, -25.0, 49.9);
	glEnd();

	// Door Knob
	glColor3fv(GOLD);
	glPushMatrix();
		glTranslatef(-5.0, -10.0, 50.0);
		glutSolidSphere(2.0, 16, 16); 
	glPopMatrix();
}

void RenderText (float x, float y, const char *string){
	glRasterPos2f(x, y);
	while (*string){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *string++);
	}
}

void DrawHUD (void){
	
	char yawStr[32];
	sprintf(yawStr, "Yaw: %.2f", camYaw);
	const char* lines[] = {
		yawStr,
		" ",
		"CONTROLS:",
		"<- / -> : Move Camera",
		"W : Move Forwards",
		"A : Strafe Left",
		"S : Mode Backwards",
		"D : Strafe Right",
		"ESC : Exit Program"
	};

	int numLines = sizeof(lines)/sizeof(lines[0]);
	int maxW = 0;

	void* font = GLUT_BITMAP_HELVETICA_12;
    
    for (int i = 0; i < numLines; i++) {
        int w = glutBitmapLength(font, (const unsigned char*)lines[i]);
        if (w > maxW)
			maxW = w;
    }

	int padding = 15;
	int lineH = 16;
	int boxW = maxW + (padding * 2);
	int boxH = (numLines * lineH) + (padding * 2);

	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	gluOrtho2D(0, winW, 0, winH);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4fv(BLACK_60);

	int x1 = 10, y1 = 10;
    int x2 = x1 + boxW;
    int y2 = y1 + boxH;

	glBegin(GL_QUADS);
		glVertex2i(x1, y1);
		glVertex2i(x2, y1);
		glVertex2i(x2, y2);
		glVertex2i(x1, y2);
	glEnd();
	glDisable(GL_BLEND);

	glColor3fv(WHITE);
	for (int i = 0; i < numLines; i++){
		int textY = y2 - padding - (i * lineH) - 10;
		RenderText(x1 + padding, textY, lines[i]);
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void Desenha (void){
	
	EspecificaParametrosVisualizacao();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawFloor();

	for (int i = 0; i < 2; i++){
		glPushMatrix();
			glTranslatef(houses[i].posX, 0.0f, 0.0f);
			DrawHouse(i ? GREEN : PURPLE);
		glPopMatrix();
	}

	DrawHUD();
	glutSwapBuffers();
}

void Inicializa (void){ 
	
	glClearColor(0.41f, 0.41f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	angle=45;

	glEnable(GL_COLOR_MATERIAL); 
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);

	GLfloat ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	GLfloat diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat specularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	GLfloat lightPos[] = { 50.0f, 100.0f, 100.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
}

void AlteraTamanhoJanela (GLsizei w, GLsizei h){
	if ( h == 0 ) h = 1;
	glViewport(0, 0, w, h); 
	fAspect = (GLfloat)w/(GLfloat)h;

	winW = w;
	winH = h;

	EspecificaParametrosVisualizacao();
}

void GerenciaTeclado (unsigned char key, int x, int y){
	
	float lx = cos(camYaw * M_PI/180.0f);
	float lz = sin(camYaw * M_PI/180.0f);

	float sx = -lz;
	float sz = lx;

	switch (key) {
		case 27:
			exit(0);
			break;
		case 'w': case 'W':
			camX += lx * camSpeed;
			camZ += lz * camSpeed;
			break;
		case 's': case 'S':
			camX -= lx * camSpeed;
			camZ -= lz * camSpeed;
			break;
		case 'a': case 'A':
			camX -= sx * camSpeed;
			camZ -= sz * camSpeed;
			break;
		case 'd': case 'D':
			camX += sx * camSpeed;
			camZ += sz * camSpeed;
			break;
	}
	
	glutPostRedisplay();
}

void TeclasEspeciais(int key, int x, int y) {
	
	float lx = cos(camYaw * M_PI/180.0f);
	float lz = sin(camYaw * M_PI/180.0f);

	switch (key) {
		case GLUT_KEY_UP:
			break;
		case GLUT_KEY_DOWN:
			break;
		case GLUT_KEY_LEFT:
			camYaw -= rotSpeed;
			break;
		case GLUT_KEY_RIGHT:
			camYaw += rotSpeed;
			break;
	}
	
	EspecificaParametrosVisualizacao();
	glutPostRedisplay();
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	
	int scrW = glutGet(GLUT_SCREEN_WIDTH);
	int scrH = glutGet(GLUT_SCREEN_HEIGHT);

	glutInitWindowPosition(0, 0);
	glutInitWindowSize(scrW, scrH);
	
	glutCreateWindow("House 3D");
	glutDisplayFunc(Desenha);
	glutReshapeFunc(AlteraTamanhoJanela);
	glutKeyboardFunc(GerenciaTeclado);
	glutSpecialFunc(TeclasEspeciais);
	Inicializa();
	glutMainLoop();
}