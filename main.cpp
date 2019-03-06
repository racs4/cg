/*
 * GLUT Shapes Demo
 *
 * Written by Nigel Stewart November 2003
 *
 * This program is test harness for the sphere, cone
 * and torus shapes in GLUT.
 *
 * Spinning wireframe and smooth shaded shapes are
 * displayed until the ESC or q key is pressed.  The
 * number of geometry stacks and slices can be adjusted
 * using the + and - keys.
 */

 #include "lib/glm/glm.hpp"
#include "lib/glm/gtc/matrix_transform.hpp"
#include "lib/glm/gtc/type_ptr.hpp"
 #include <stdio.h>
 #include <math.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>

#define ESCAPE 27
#define W 119
#define A 97
#define S 115
#define D 100

class Camera{
    float x;
    float y;
    float z;
    float lookAtx;
    float lookAty;
    float lookAtz;
};

glm::vec3 positionCamera;
glm::vec3 direction, up, right;
GLfloat luzAmbiente[4] = {1.0,1.0,1.0,1.0}
,luzDifusa[4] = {0.7,0.7,0.7,1.0}
,luzEspecular[4] = {1.0, 1.0, 1.0, 1.0}
,posicaoLuz[4] = {0.0, 50, 0, 25.0}
,especularidade[4] = {1.0,1.0,1.0,1.0};

GLint especMaterial = 40;

float anglex = 0, angley = 0;
bool mouseStarted = true;
float mouseX, mouseY, height = 800, width = 600;
float rotationSpeed = 0.008;

void init(){
    glClearColor(0.0,0.0,0.0,1.0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    positionCamera = {0.0,10.0,0.0};

    up = {0.0,1.0,0.0};
    direction = {0.0,0.0,-1.0};
    right = glm::cross(direction,up);

    mouseX = width/2;
    mouseY = height/2;

    glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade);
    glMateriali(GL_FRONT,GL_SHININESS,especMaterial);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa );
    glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular );
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz );
}

void reshape(int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(100.0, 1.0, 2, 500);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void normalKeys(unsigned char key, int x, int y){
    glm::mat4 M = glm::mat4(1.0f);
    glm::vec4 aux = glm::vec4(positionCamera[0],positionCamera[1],positionCamera[2],1);
    if(key == W){
        M = glm::translate(M,direction);
        M = glm::transpose(M);
        aux = aux*M;
        positionCamera = glm::vec3(aux[0],aux[1], aux[2]);
    }
    else if(key == S){
        M = glm::translate(M,-direction);
        M = glm::transpose(M);
        aux = aux*M;
        positionCamera = glm::vec3(aux[0],aux[1], aux[2]);
    }
    else if(key == A){
        M = glm::translate(M,-right);
        M = glm::transpose(M);
        aux = aux*M;
        positionCamera = glm::vec3(aux[0],aux[1], aux[2]);
    }
    else if(key == D){
        M = glm::translate(M,right);
        M = glm::transpose(M);
        aux = aux*M;
        positionCamera = glm::vec3(aux[0],aux[1], aux[2]);
    }

    if(positionCamera[1] <= 0)
        positionCamera[1] = 1;
}

bool warping = false;
void mouse(int x, int y){
    if(!mouseStarted){
        glutWarpPointer(mouseX,mouseY);
        mouseStarted = true;
        return;
    } else{
        if(warping)
        {
            warping = false;
            return;
        }
        int dx = x - mouseX;
        int dy = mouseY - y;

        glm::vec4 aux = glm::vec4(direction[0],direction[1],direction[2],1);

        anglex = dx*rotationSpeed;
        angley = dy*rotationSpeed;

        glm::mat4 M = glm::mat4(1.0f);

        M = glm::rotate(M, anglex, glm::vec3(up[0],up[1],up[2]));
        M = glm::rotate(M, angley, glm::vec3(-right[0],-right[1],-right[2]));

        aux = aux*M;
        direction = glm::vec3(aux[0], aux[1], aux[2]);
        right = glm::cross(direction,up);

        if(mouseStarted)
        {
            warping = true;
            glutWarpPointer(mouseX,mouseY);
        }
        anglex = 0;
        angley = 0;

    }

}

void display(void){
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    ShowCursor(FALSE);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt (positionCamera[0], positionCamera[1], positionCamera[2], positionCamera[0]+direction[0], positionCamera[1]+direction[1], positionCamera[2]+direction[2], up[0], up[1], up[2]);

    static GLfloat vertices[] = {-100.0, 0.0, -100.0, -100.0, 100.0, -100.0, 100.0, 100.0, -100.0, 100.0, 0.0, -100.0};//, 100.0, 70.0, 100.0, 100.0, 70.0, -100.0, -100.0, 70, -100.0, -100.0, 70.0, 100.0};
    glEnableClientState(GL_VERTEX_ARRAY);

    glVertexPointer (3, GL_FLOAT, 0, vertices);

    int i, j, tam_array = (int) (sizeof(vertices)/sizeof(float));

    GLfloat aux[tam_array];
    for(i = 0; i < tam_array; i++){
        aux[i] = vertices[i];
    }

    glPushMatrix();
        glColor4f(0.3f,0.3f,0.3f,0.0f);
        glBegin(GL_QUADS);
            for (j = 0; j < 4; j++) {
                for (i = 0; i < 4; i++) {
                    glm::vec4 aux2 = glm::vec4(vertices[0 + 3*i],vertices[1 + 3*i],vertices[2 + 3*i],1);
                    glm::mat4 M = glm::mat4(1.0f);

                    M = glm::rotate(M, (float) M_PI/4, glm::vec3(0.0,1.0,0.0));

                    aux2 = aux2*M;

                    vertices[0 + 3*i] = aux2[0], vertices[1 + 3*i] = aux2[1], vertices[2 + 3*i] = aux2[2];

                    glArrayElement(i);
                }
            }
        glEnd();

        for(i = 0; i < tam_array; i++){
            vertices[i] = aux[i];
        }

        glBegin(GL_QUADS);
            glVertex3f(-100.0f, 0.0f, -100.0f);
            glVertex3f(-100.0f, 0.0f,  100.0f);
            glVertex3f( 100.0f, 0.0f,  100.0f);
            glVertex3f( 100.0f, 0.0f, -100.0f);
        glEnd();
    glPopMatrix();
    glPushMatrix();
        glColor3f(1.0,0.3,0.5);
        glTranslatef(0.0,2.8,25.0);
        glutSolidCube(9);
    glPopMatrix();

    glPushMatrix();
        glColor4f(0.3f,0.3f,0.9f, 1.0f);
        glTranslatef(0.0,9.0,25.0);
        glutSolidTeapot(3.0);
    glPopMatrix();

    glDisableClientState(GL_VERTEX_ARRAY);

	glutSwapBuffers();
}

int main(int argc, char **argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB| GLUT_DEPTH);
   glutInitWindowSize (height, width);
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);

   init ();

   glutDisplayFunc(display);
   glutKeyboardFunc(normalKeys);
   glutIdleFunc(display);
   glutReshapeFunc(reshape);
   glutPassiveMotionFunc(mouse);
   glutMainLoop();

   return 0;

return 1;
}
