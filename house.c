#include <GL/glut.h>

const float RED[3] = {1.0f, 0.41f, 0.41f};
const float DARK_RED[3] = {0.6f, 0.2f, 0.2f};
const float GREEN[3] = {0.41f, 1.0f, 0.41f};
const float DARK_GREEN[3] = {0.2f, 0.6f, 0.2f};
const float BROWN[3] = {0.55f, 0.27f, 0.07f};
const float GOLD[3] = {1.0f, 0.84f, 0.0f};
const float LIGHT_BLUE[3] = {1.0f, 0.41f, 0.41f};
const float GREY[3] = {0.5f, 0.5f, 0.5f};

void Desenha(void)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
                
    glClear(GL_COLOR_BUFFER_BIT);

    // Roof Front
    glBegin(GL_TRIANGLES);
        glColor3fv(RED);
        glVertex2f(-0.2f, 0.4f);
        glVertex2f(-0.4f, 0.0f);
        glVertex2f(0.0f, 0.0f);
    glEnd();

    // Roof Side
    glBegin(GL_QUADS);
        glColor3fv(DARK_RED);
        glVertex2f(-0.2f, 0.4f);
        glVertex2f(0.4f, 0.4f);
        glVertex2f(0.6f, 0.0f);
        glVertex2f(0.0f, 0.0f);
    glEnd();
     
    // House Base Front
    glBegin(GL_QUADS);
        glColor3fv(GREEN);
        glVertex2f(-0.4f, 0.0f);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(0.0f, -0.4f);
        glVertex2f(-0.4f, -0.4f);
    glEnd();

    // House Base Side
    glBegin(GL_QUADS);
        glColor3fv(DARK_GREEN);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(0.6f, 0.0f);
        glVertex2f(0.6f, -0.4f);
        glVertex2f(0.0f, -0.4f);
    glEnd();

    // Door
    glBegin(GL_QUADS);
        glColor3fv(BROWN);
        glVertex2f(-0.28f, -0.4f);
        glVertex2f(-0.28f, -0.16f);
        glVertex2f(-0.12f, -0.16f);
        glVertex2f(-0.12f, -0.4f);
    glEnd();

    // Door Knob
    glColor3fv(GOLD);
    glPointSize(3.0f);
    glBegin(GL_POINTS);
        glVertex2f(-0.256f, -0.28f);
    glEnd();

    // Floor
    glBegin(GL_QUADS);
        glColor3fv(GREY);
        glVertex2f(-1.0f, -0.4f);
        glVertex2f(1.0f, -0.4f);
        glVertex2f(1.0f, -1.0f);
        glVertex2f(-1.0f, -1.0f);
    glEnd();

     glFlush();
}

void Inicializa (void)
{   
    // Sets the background colour to light blue
    glClearColor(0.529f, 0.808f, 1.0f, 1.0f);
}

void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
    if(h == 0) h = 1;
            
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (w <= h) 
            gluOrtho2D (-1.0f, 1.0f, -1.0f*h/w, 1.0f*h/w);
    else 
            gluOrtho2D (-1.0f*w/h, 1.0f*w/h, -1.0f, 1.0f);
}

int main(int argc, char **argv)
{
     glutInit(&argc, argv);

     glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
     glutInitWindowSize(400,400);
     glutInitWindowPosition(10,10);
     glutCreateWindow("House");
     glutDisplayFunc(Desenha);
     glutReshapeFunc(AlteraTamanhoJanela);
     Inicializa();
     glutMainLoop();
}