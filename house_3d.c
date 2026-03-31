#include <GL/glut.h>
#include <math.h>

typedef struct {
    float x, y, z;
} Point3D;

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

const float RED[3] = {1.0f, 0.41f, 0.41f};
const float DARK_RED[3] = {0.6f, 0.2f, 0.2f};
const float GREEN[3] = {0.41f, 1.0f, 0.41f};
const float DARK_GREEN[3] = {0.2f, 0.6f, 0.2f};
const float BROWN[3] = {0.55f, 0.27f, 0.07f};
const float GOLD[3] = {1.0f, 0.84f, 0.0f};
const float LIGHT_BLUE[3] = {1.0f, 0.41f, 0.41f};
const float GREY[3] = {0.5f, 0.5f, 0.5f};
const float DARK_BROWN[3] = {0.33f, 0.16f, 0.04f};

GLfloat rotX = 0.0f;
GLfloat rotY = 0.0f;
GLfloat rotZ = 0.0f;
GLfloat angle, fAspect;

void resetRotation (){
	rotX = 0.0;
	rotY = 0.0;
	rotZ = 0.0;	
}

// Função callback chamada para fazer o desenho
void Desenha(void)
{
	// glMatrixMode(GL_MODELVIEW);
	// glLoadIdentity();
				
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glRotatef(rotX, 1.0f, 0.0f, 0.0f);
	glRotatef(rotY, 0.0f, 1.0f, 0.0f);
	glRotatef(rotZ, 0.0f, 0.0f, 1.0f);

	// House Base
	glColor3fv(GREEN);
		
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

	glutSwapBuffers();
}

void Inicializa (void)
{ 
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

void EspecificaParametrosVisualizacao(void)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(angle,fAspect,0.5,500);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0,80,200, 0,0,0, 0,1,0);
}

void AlteraTamanhoJanela(GLsizei w, GLsizei h) {
	if ( h == 0 ) h = 1;
	glViewport(0, 0, w, h); 
	fAspect = (GLfloat)w/(GLfloat)h;
	EspecificaParametrosVisualizacao();
}

void GerenciaTeclado(unsigned char key, int x, int y) {
	switch (key) {
		case 'x':
			rotX += 15.0f;
			break;
		case 'X':
			rotX -= 15.0f;
			break;
		case 'y':
			rotY += 15.0f;
			break;
		case 'Y':
			rotY -= 15.0f;
			break;
		case 'z':
			rotZ += 15.0f;
			break;
		case 'Z':
			rotZ -= 15.0f;
			break;
		case 'r':
			resetRotation();
			break;
	}
	
	EspecificaParametrosVisualizacao();
	glutPostRedisplay();
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400,400);
	glutCreateWindow("House 3D");
	glutDisplayFunc(Desenha);
	glutReshapeFunc(AlteraTamanhoJanela);
	glutKeyboardFunc(GerenciaTeclado);
	Inicializa();
	glutMainLoop();
}