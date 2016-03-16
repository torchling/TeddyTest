/*
    GLUT Teddy
    Jui-Cheng,Sung. R.O.C.
    Lyre Mellark.
    Started from 2016.Feb.10
*/

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <windows.h>
#include <GL/glut.h>
#endif

#include <vector>
#include "helpteddy.h"
#include <stdlib.h>

using namespace std;

vector<float> vect;

bool meshBeenMade = false;

struct vertex
{
    float x;
    float y;
    float z;
};

static int slices = 16;
static int stacks = 16;

GLfloat vertices[15][3]=
{
    { 0.0f, 0.0f, 0.0f },
    { 3.5f, -0.5f, 0.0f },
    { 3.7f, 1.6f, 0.0f },
    { 0.6f, 3.2f, 0.0f },
    { 0.5f, 6.8f, 0.0f },
    { 3.7f, 9.2f, 0.0f },
    { 7.5f, 8.9f, 0.0f },
    { 10.1f, 6.5f, 0.0f },
    { 9.6f, 3.3f, 0.0f },
    { 6.7f, 1.5f, 0.0f },
    { 6.2f, -1.2f, 0.0f },
    { 8.0f, -4.2f, 0.0f },
    { 4.5f, -4.1f, 0.0f },
    { 3.4f, -1.8f, 0.0f },
    { 1.2f, -1.5f, 0.0f }
};

/* GLUT callback Handlers */

static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

static void display(void)
{
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    const double a = t*90.0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1,0,0);

    glPushMatrix();
        glTranslated(-2.4,1.2,-6);
        glBegin(GL_TRIANGLES);
            glVertex3f( 0.2f, 0.2f, 0.2f);    // Top Right Of The Quad (Front)
            glVertex3f(-0.2f, 0.2f, 0.2f);    // Top Left Of The Quad (Front)
            glVertex3f(-0.2f,-0.2f, 0.2f);    // Bottom Left Of The Quad (Front)
        glEnd();
    glPopMatrix();

    glPushMatrix();
        glTranslated(-5.0,-3.0,-30);
        glBegin(GL_LINE_LOOP);
            glVertex3f( -10.0f, -4.1f, 0.0f);
            glVertex3f(4.5f, 13.7f, 0.0f);
            glVertex3f(24.0f,-6.3f, 0.0f);
        glEnd();
        glBegin(GL_LINE_LOOP);
            glVertex3f( 0.0f, 0.0f, 0.0f);
            glVertex3f( 3.5f, -0.5f, 0.0f);
            glVertex3f( 3.7f, 1.6f, 0.0f);
            glVertex3f( 0.6f, 3.2f, 0.0f);
            glVertex3f( 0.5f, 6.8f, 0.0f);
            glVertex3f( 3.7f, 9.2f, 0.0f);
            glVertex3f( 7.5f, 8.9f, 0.0f);
            glVertex3f( 10.1f, 6.5f, 0.0f);
            glVertex3f( 9.6f, 3.3f, 0.0f);
            glVertex3f( 6.7f, 1.5f, 0.0f);
            glVertex3f( 6.2f, -1.2f, 0.0f);
            glVertex3f( 8.0f, -4.2f, 0.0f);
            glVertex3f( 4.5f, -4.1f, 0.0f);
            glVertex3f( 3.4f, -1.8f, 0.0f);
            glVertex3f( 1.2f, -1.5f, 0.0f);
        glEnd();
    glPopMatrix();

/*
    glPushMatrix();
        glTranslated(2.4,1.2,-6);
        glRotated(60,1,0,0);
        glRotated(a,0,0,1);
        glutSolidTorus(0.2,0.8,slices,stacks);
    glPopMatrix();

    glPushMatrix();
        glTranslated(2.4,-1.2,-6);
        glRotated(60,1,0,0);
        glRotated(a,0,0,1);
        glutWireTorus(0.2,0.8,slices,stacks);
    glPopMatrix();
*/
    glutSwapBuffers();
}


static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;

        case '+':
            slices++;
            stacks++;
            break;

        case '-':
            if (slices>3 && stacks>3)
            {
                slices--;
                stacks--;
            }
            break;
    }

    glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

/* Program entry point */

int main(int argc, char *argv[])
{
    if(!meshBeenMade)
    {
        generateDelaunayTruangle( vertices );
        //makeMesh();
    }

    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GLUT Shapes");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    glClearColor(1,1,1,1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glutMainLoop();

    return EXIT_SUCCESS;
}
