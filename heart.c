#include <GL/glut.h>
#include <math.h>

// Função callback chamada para fazer o desenho

void DrawFilledCircle(float cx, float cy, float r, int segments)
{
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, cy);
        for (int i = 0; i <= segments; i++)
        {
            float theta = 2.0f * 3.1415926f * i / segments;
            float x = r * cosf(theta);
            float y = r * sinf(theta);

            glVertex2f(cx + x, cy + y);
        }
    glEnd();
}

void DrawStrokeText(float x, float y, float scale, const char* text)
{
    glPushMatrix();
        glTranslatef(x, y, 0.0f);
        glScalef(scale, scale, scale);
        while (*text)
        {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, *text);
            text++;
        }
    glPopMatrix();
}

void Desenha(void)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
                
    // Limpa a janela de visualização com a cor de fundo especificada
    glClear(GL_COLOR_BUFFER_BIT);

    // Make a border

    glColor3f(0.5f, 0.0f, 0.0f); // Red
    
    // Circle Params
    float r = 32;
    int segments = 128;
    DrawFilledCircle(95.0f, 155.0f, r, segments);
    DrawFilledCircle(135.0f, 155.0f, r, segments);

    // Triangle
    glBegin(GL_TRIANGLES);
        glVertex2i(66,139);
        glVertex2i(164,139);
        glVertex2i(115,91);
    glEnd();

    // Foreground

    glColor3f(0.65f, 0.25f, 0.25f); // Dark Red
    
    // Circle Params
    r = 30;
    segments = 128;
    DrawFilledCircle(95.0f, 155.0f, r, segments);
    DrawFilledCircle(135.0f, 155.0f, r, segments);

    // Triangle
    glBegin(GL_TRIANGLES);
        glVertex2i(69,139);
        glVertex2i(161,139);
        glVertex2i(115,94);
    glEnd();

    // Text
    // glLineWidth(3.0f);
    // glColor3f(1.0f, 0.6f, 0.6f); // light pink / white
    // DrawStrokeText(88.0f, 135.0f, 0.18f, "MARI");
    // glLineWidth(1.0f);
    
    // Executa os comandos OpenGL
    glFlush();
}

// Inicializa parâmetros de rendering
void Inicializa (void)
{   
    // Define a cor de fundo da janela de visualização como preta
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
}

// Função callback chamada quando o tamanho da janela é alterado 
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
                   // Evita a divisao por zero
                   if(h == 0) h = 1;
                           
                   // Especifica as dimensões da Viewport
                   glViewport(0, 0, w, h);

                   // Inicializa o sistema de coordenadas
                   glMatrixMode(GL_PROJECTION);
                   glLoadIdentity();

                   // Estabelece a janela de seleção (left, right, bottom, top)
                   if (w <= h) 
                           gluOrtho2D (0.0f, 250.0f, 0.0f, 250.0f*h/w);
                   else 
                           gluOrtho2D (0.0f, 250.0f*w/h, 0.0f, 250.0f);
}

// Programa Principal 
int main(int argc, char **argv)
{
     glutInit(&argc, argv);

     glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
     glutInitWindowSize(400,400);
     glutInitWindowPosition(10,10);
     glutCreateWindow("Heart");
     glutDisplayFunc(Desenha);
     glutReshapeFunc(AlteraTamanhoJanela);
     Inicializa();
     glutMainLoop();
}
