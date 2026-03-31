#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define MAX_HOUSES 20

const float RED[3] = {1.0f, 0.41f, 0.41f};
const float BROWN[3] = {0.55f, 0.27f, 0.07f};
const float GOLD[3] = {1.0f, 0.84f, 0.0f};
const float BLACK_60[4] = {0.0f, 0.0f, 0.0f, 0.6f};

// House Base Colours
const float WHITE[3] = {1.0f, 1.0f, 1.0f}; // Selected
const float GREEN[3] = {0.41f, 1.0f, 0.41f};
const float BLUE[3] = {0.41f, 0.41f, 1.0f};
const float YELLOW[3] = {1.0f, 1.0f, 0.41f};
const float PURPLE[3] = {1.0f, 0.41f, 1.0f};
const float CYAN[3] = {0.41f, 1.0f, 1.0f};

const float* HOUSE_COLOURS[] = {GREEN, BLUE, YELLOW, PURPLE, CYAN};
const int NUM_HOUSE_COLORS = 5;

int selectedHouse = 0;
int numHouses = 1;

GLfloat angle, fAspect = 1.0f;
int winW = 400, winH = 400;

typedef struct {
	float x, y, z;
} Point3D;

typedef struct {
	float rotX, rotY, rotZ;
	float posX, posZ;
	int colour;
} HouseState;

HouseState houses[MAX_HOUSES] = {
	{-30.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0},
};

void EspecificaParametrosVisualizacao (void){
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	int activeCols = (numHouses < 5) ? numHouses : 5;
	int activeRows = ((numHouses - 1)/5) + 1;

	float maxDimension = (activeCols > activeRows) ? activeCols : activeRows;
	float windowSize = 60.0f + (activeCols * 40.0f) + (activeRows * 45.0f);

	if (fAspect <= 1)
		glOrtho(-windowSize, windowSize, -windowSize / fAspect, windowSize / fAspect, -2000.0, 2000.0);
	else
		glOrtho(-windowSize * fAspect, windowSize * fAspect, -windowSize, windowSize, -2000.0, 2000.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float centerFocusX = ((activeCols - 1) * 120.0f) / 2.0f;
	float centerFocusZ = -((activeRows - 1) * 250.0f) / 2.0f;

	gluLookAt(centerFocusX, 300.0f, centerFocusZ + 200.0f, centerFocusX, 0, centerFocusZ, 0, 1, 0);
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

void resetRotation (int n){
	houses[n].rotX = -30.0;
	houses[n].rotY = 0.0;
	houses[n].rotZ = 0.0;	
}

int getColour (int n){
	int id, prev_id = -1, bottom_id = -1;

	if (n > 0 && (n % 5 != 0))
		prev_id = houses[n-1].colour;

	if (n >= 5){
		bottom_id = houses[n-5].colour;
	}

	while (1){
		id = rand() % NUM_HOUSE_COLORS;

		if (id != prev_id && id != bottom_id)
			break;
	}
	
	return id;
}

void DrawHouse (int isSelected, int colour){
	
	// House Base
	if (isSelected)
		glColor3fv(WHITE);
	else
		glColor3fv(HOUSE_COLOURS[colour]);
		
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
	glBegin(GL_QUADS);
		glColor3fv(BROWN);
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(10.0, 5.0, 50.1);
		glVertex3f(-10.0, 5.0, 50.1);
		glVertex3f(-10.0, -25.0, 50.1);
		glVertex3f(10.0, -25.0, 50.1);
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

	glBegin(GL_QUADS);
		glVertex2i(10, 10);
		glVertex2i(240, 10);
		glVertex2i(240, 150);
		glVertex2i(10, 150);
    glEnd();
    glDisable(GL_BLEND);

	glColor3fv(WHITE);

	int startY = 130, spacing = 16;

	RenderText(20, startY, "CONTROLS:");
    RenderText(20, startY - spacing * 1, "+ / - : Add or Remove House");
    RenderText(20, startY - spacing * 2, "Arrows : Change Selection");
    RenderText(20, startY - spacing * 3, "X / Y / Z : Rotate Selected");
	RenderText(20, startY - spacing * 4, "SHIFT + X / Y / Z : Rotate Selected");
    RenderText(20, startY - spacing * 5, "R : Reset Selected Rotation");
    RenderText(20, startY - spacing * 6, "SHIFT + R : Reset All Rotations");
    RenderText(20, startY - spacing * 7, "ESC : Exit Program");

	glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void Desenha (void){
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	EspecificaParametrosVisualizacao();

	for (int i = 0; i < numHouses; i++){
		glPushMatrix();
			glTranslatef(houses[i].posX, 0.0f, houses[i].posZ);

			glRotatef(houses[i].rotX, 1.0f, 0.0f, 0.0f);
			glRotatef(houses[i].rotY, 0.0f, 1.0f, 0.0f);
			glRotatef(houses[i].rotZ, 0.0f, 0.0f, 1.0f);

			DrawHouse(i == selectedHouse, houses[i].colour);
		glPopMatrix();
	}

	DrawHUD();
	
	glutSwapBuffers();
}

void Inicializa (void){ 
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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
}

void GerenciaTeclado (unsigned char key, int x, int y){
	switch (key) {
		case 27:
			exit(0);
			break;
		case 'x':
			houses[selectedHouse].rotX += 15.0f;
			break;
		case 'X':
			houses[selectedHouse].rotX -= 15.0f;
			break;
		case 'y':
			houses[selectedHouse].rotY += 15.0f;
			break;
		case 'Y':
			houses[selectedHouse].rotY -= 15.0f;
			break;
		case 'z':
			houses[selectedHouse].rotZ += 15.0f;
			break;
		case 'Z':
			houses[selectedHouse].rotZ -= 15.0f;
			break;
		case 'r':
			resetRotation(selectedHouse);
			break;
		case 'R':
			for (int i = 0; i < numHouses; i++)
				resetRotation(i);
			break;
		case '+':
		case '=':
			if (numHouses < MAX_HOUSES){

				int col = numHouses % 5;
				int row = numHouses/5;

				houses[numHouses].rotX = -30.0f;
				houses[numHouses].rotY = 0.0f;
				houses[numHouses].rotZ = 0.0f;

				houses[numHouses].posX = col * 120.0f;
				houses[numHouses].posZ = -(row * 250.0f);

				houses[numHouses].colour = getColour(numHouses);

				numHouses++;
				selectedHouse = numHouses - 1;
			}
			break;
		case '-':
			if (numHouses > 1) {
				numHouses--;
				if (selectedHouse >= numHouses) {
					selectedHouse = numHouses - 1;
				}
			}
	}
	
	glutPostRedisplay();
}

void TeclasEspeciais(int key, int x, int y) {
	
	int col = selectedHouse % 5;
	int row = selectedHouse/5;
	int maxRows = (numHouses - 1)/5;

	int startOfRow = row * 5;
	int housesInRow = 5;

	if (startOfRow + 5 > numHouses)
		housesInRow = numHouses - startOfRow;
	
	switch (key) {
		case GLUT_KEY_LEFT:
			selectedHouse = startOfRow + ((col - 1 + housesInRow) % housesInRow);
			break;
		case GLUT_KEY_RIGHT:
			selectedHouse = startOfRow + ((col + 1) % housesInRow);
			break;
		case GLUT_KEY_UP:
			if (selectedHouse + 5 < numHouses)
				selectedHouse += 5;
			else
				selectedHouse = col;
			break;
		case GLUT_KEY_DOWN:
			if (selectedHouse - 5 >= 0)
				selectedHouse -= 5;
			else {
				int target = col + (maxRows * 5);
				if (target >= numHouses)
					target -= 5;
				selectedHouse = target;
			}
			break;
	}
	
	glutPostRedisplay();
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);

	srand(time(NULL));
	
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