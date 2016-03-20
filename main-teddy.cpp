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
#include <stdio.h>
#include <iostream>

using namespace std;

std::vector< vertex >	theSetOfInputPoint ;
std::vector< edge >		edgePool ;
std::vector< edge >		tmp_edgePool ;
std::vector< triangle > trianglePool ;
std::vector< triangle > badTrianglePool ;

bool meshBeenMade = false;

/*------------*/
static int slices = 16;
static int stacks = 16;
/*-------------------*/

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


//BowyerWatson
bool areSameEdges( edge edgeA , edge edgeB )
{
    bool vA1B1 =(((edgeA.v1.x == edgeB.v1.x)&&(edgeA.v1.y == edgeB.v1.y))&&(edgeA.v1.z == edgeB.v1.z))&&
                (((edgeA.v2.x == edgeB.v2.x)&&(edgeA.v2.y == edgeB.v2.y))&&(edgeA.v2.z == edgeB.v2.z));
    bool vA1B2 =(((edgeA.v1.x == edgeB.v2.x)&&(edgeA.v1.y == edgeB.v2.y))&&(edgeA.v1.z == edgeB.v2.z))&&
                (((edgeA.v2.x == edgeB.v1.x)&&(edgeA.v2.y == edgeB.v1.y))&&(edgeA.v2.z == edgeB.v1.z));

	if(vA1B1||vA1B2)
		return true;

	return false;
}

triangle findSuperTriangle()
{
/*
    vertex vertex_1 = { 0.0, 1.0, 0.0 };
    vertex vertex_2 = { 0.5, 0.0, 0.0};
    vertex vertex_3 = { -0.5, 0.0, 0.0};
*/
    vertex vertex_1 = {-10.0f, -4.1f, 0.0f};
    vertex vertex_2 = {4.5f, 13.7f, 0.0f};
    vertex vertex_3 = {24.0f,-6.3f, 0.0f};

    triangle super;
/*
    for(int i=0;i<theSetOfInputPoint.size();i++)
    {
        if(outsideTheTriangle( theSetOfInputPoint[i], vertex_1, vertex_2, vertex_3 ))
        {
        	vertex_1.x = vertex_1.x + vertex_1.x - theSetOfInputPoint[i].x;
        	vertex_2.x = vertex_2.x + vertex_2.x - theSetOfInputPoint[i].x;
        	vertex_3.x = vertex_3.x + vertex_3.x - theSetOfInputPoint[i].x;

        	vertex_1.y = vertex_1.y + vertex_1.y - theSetOfInputPoint[i].y;
        	vertex_2.y = vertex_2.y + vertex_2.y - theSetOfInputPoint[i].y;
        	vertex_3.y = vertex_3.y + vertex_3.y - theSetOfInputPoint[i].y;
        }

    }
*/
    super.v1 = vertex_1;
    super.v2 = vertex_2;
    super.v3 = vertex_3;

    return super;
}

//float might need to be changed
void addToEdgePool( vertex v1 , vertex v2 )
{
    edge edgetp;
    edgetp.v1 = v1;
    edgetp.v2 = v2;

    edgePool.push_back(edgetp);
}

void addToTrianglePool( vertex v1 , vertex v2 , vertex v3 )
{
    triangle triangletp;
    triangletp.v1 = v1;
    triangletp.v2 = v2;
    triangletp.v3 = v3;

    trianglePool.push_back(triangletp);
    cout<<"testforadd"<<"\n";
}

void addToBadTrianglePool( vertex v1 , vertex v2 , vertex v3 )
{
    triangle triangletp;
    triangletp.v1 = v1;
    triangletp.v2 = v2;
    triangletp.v3 = v3;

    badTrianglePool.push_back(triangletp);
}

void deletDoubleEdge()
{
    int i;
    int j;
    int k;
    int h;

    start:
    for(i=0;i<edgePool.size();i++)
    {
        for(j=0;j<edgePool.size();j++)
        {
            if( areSameEdges(edgePool[i],edgePool[j]) && (i!=j) )
            {

                for(k=0; k<edgePool.size(); k++){
                    if((k!=i)&&(k!=j))
                    tmp_edgePool.push_back(edgePool[k]);
                }

                edgePool.clear();
                for(h=0; h<tmp_edgePool.size(); h++){
                    edgePool.push_back(tmp_edgePool[h]);
                }
                tmp_edgePool.clear();
                goto start;
            }
        }
    }
}

bool triangulationContainSuperDT( triangle test, triangle super )
{
    vertex s1 = super.v1;
    vertex s2 = super.v2;
    vertex s3 = super.v3;

    if(
    (((test.v1.x == s1.x)&&(test.v1.y == s1.y))&&(test.v1.z == s1.z))||
    (((test.v1.x == s2.x)&&(test.v1.y == s2.y))&&(test.v1.z == s2.z))||
    (((test.v1.x == s3.x)&&(test.v1.y == s3.y))&&(test.v1.z == s3.z))||
    (((test.v2.x == s1.x)&&(test.v2.y == s1.y))&&(test.v2.z == s1.z))||
    (((test.v2.x == s2.x)&&(test.v2.y == s2.y))&&(test.v2.z == s2.z))||
    (((test.v2.x == s3.x)&&(test.v2.y == s3.y))&&(test.v2.z == s3.z))||
    (((test.v3.x == s1.x)&&(test.v3.y == s1.y))&&(test.v3.z == s1.z))||
    (((test.v3.x == s2.x)&&(test.v3.y == s2.y))&&(test.v3.z == s2.z))||
    (((test.v3.x == s3.x)&&(test.v3.y == s3.y))&&(test.v3.z == s3.z))
       )
       return true;

    return false;
}

bool triangulationContainPoint( triangle test, vertex s1 )
{
    if(
    (((test.v1.x == s1.x)&&(test.v1.y == s1.y))&&(test.v1.z == s1.z))||
    (((test.v2.x == s1.x)&&(test.v2.y == s1.y))&&(test.v2.z == s1.z))||
    (((test.v3.x == s1.x)&&(test.v3.y == s1.y))&&(test.v3.z == s1.z))
       )
       return true;

    return false;
}

bool isAnOutsideTriangle( triangle testTri, edge testEdge )
{
    if( triangulationContainPoint( testTri , testEdge.v1 ) ) {
    if( triangulationContainPoint( testTri , testEdge.v2 ) ){
        edge e1,e2,e3;
        e1.v1=testTri.v1; e1.v2=testTri.v2;
        e2.v1=testTri.v2; e2.v2=testTri.v3;
        e3.v1=testTri.v3; e3.v2=testTri.v1;
        vertex p;

        if( areSameEdges( e1,testEdge ) ){
            p.x =testEdge.v1.x+testEdge.v2.y-testEdge.v1.y;
            p.y =testEdge.v1.y+testEdge.v1.x-testEdge.v2.x;
            p.z =0.0;
            if( !onTheSameSide( p, testTri.v1, testTri.v2, testTri.v3 ) )
            return true;
        }
        else if( areSameEdges( e2,testEdge ) ){
            p.x =testEdge.v1.x+testEdge.v2.y-testEdge.v1.y;
            p.y =testEdge.v1.y+testEdge.v1.x-testEdge.v2.x;
            p.z =0.0;
            if( !onTheSameSide( p, testTri.v2, testTri.v3, testTri.v1 ) )
            return true;
        }
        else if( areSameEdges( e3,testEdge ) ){
            p.x =testEdge.v1.x+testEdge.v2.y-testEdge.v1.y;
            p.y =testEdge.v1.y+testEdge.v1.x-testEdge.v2.x;
            p.z =0.0;
            if( !onTheSameSide( p, testTri.v3, testTri.v1, testTri.v2 ) )
            return true;
        }
    }
    }

    return false;
}

void generateDelaunayTriangle()
{
    triangle superDT;
    triangle test;
    edge edgeFT;
    vertex center;
    float radius;
    bool contains_superDT;

    trianglePool.clear();

    superDT = findSuperTriangle();
    addToTrianglePool(superDT.v1, superDT.v2, superDT.v3);


    for(int i=0; i<theSetOfInputPoint.size(); i++)
    {
        cout<<"\n";
        cout<<"Round "<<i<<"\n";
        cout<<trianglePool.size()<<" TrianglePool_Size"<<"\n";
        badTrianglePool.clear();
        for(int j=0; j<trianglePool.size(); j++)
            {
                test.v1 = trianglePool[j].v1;
                test.v2 = trianglePool[j].v2;
                test.v3 = trianglePool[j].v3;
                center = centerOfCircumscribedCircle( test.v1 , test.v2 , test.v3 );
                radius = radiusOfCCircle( test.v1 , center );

                if( insideTheCircle( theSetOfInputPoint[i] , center , radius ) ){
                        addToBadTrianglePool( test.v1 , test.v2 , test.v3 );

                        //Delete badTri. from tri.Pool
                        if(j!=trianglePool.size()-1){
                            trianglePool[j]=trianglePool[trianglePool.size()-1];
                            j--;
                        }
                        trianglePool.pop_back();
                }

            }

        edgePool.clear();
        for(int q=0; q<badTrianglePool.size(); q++)
            {
                addToEdgePool( badTrianglePool[q].v1 , badTrianglePool[q].v2 );
                addToEdgePool( badTrianglePool[q].v2 , badTrianglePool[q].v3 );
                addToEdgePool( badTrianglePool[q].v3 , badTrianglePool[q].v1 );
            }

        deletDoubleEdge();
/*
        //Part of pseudo code. Have been writen in Line 199 "Delete badTri. from tri.Pool"

        for each triangle in badTriangles do // remove them from the data structure
            remove triangle from triangulation
*/
        for(int k=0;k<edgePool.size();k++)
            {
            	edgeFT = edgePool[k];
                addToTrianglePool( edgeFT.v1 , edgeFT.v2 , theSetOfInputPoint[i] );
            }

    }

    for (int ct=0; ct<trianglePool.size(); ct++) // done inserting points, now clean up
    {
        if( triangulationContainSuperDT( trianglePool[ct], superDT ) )
        {
            if(ct!=trianglePool.size()-1){
                trianglePool[ct]=trianglePool[trianglePool.size()-1];
                ct--;
            }
            trianglePool.pop_back();
        }
    }

    //Trimming outside Triangles
    edgePool.clear();
    for (int cot=0; cot<theSetOfInputPoint.size()-1; cot++){
        addToEdgePool( theSetOfInputPoint[cot], theSetOfInputPoint[cot+1] );
    }
    for (int cnt=0; cnt<edgePool.size(); cnt++) // done inserting points, now clean up
    {
        for (int ad=0; ad<trianglePool.size(); ad++){
            if( isAnOutsideTriangle( trianglePool[ad], edgePool[cnt] ) )
            {
                if(ad!=trianglePool.size()-1){
                    trianglePool[ad]=trianglePool[trianglePool.size()-1];
                    ad--;
                }
                trianglePool.pop_back();
            }
        }
    }

}

void printTrianglePool()
{
    //cout<<trianglePool.size()<<"\n";

    for(int i=0; i<trianglePool.size(); i++)
    {
        glBegin(GL_LINE_LOOP);
            glVertex3f( trianglePool[i].v1.x, trianglePool[i].v1.y, trianglePool[i].v1.z );
            glVertex3f( trianglePool[i].v2.x, trianglePool[i].v2.y, trianglePool[i].v2.z );
            glVertex3f( trianglePool[i].v3.x, trianglePool[i].v3.y, trianglePool[i].v3.z );
        glEnd();
    }

}


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
/*
    glPushMatrix();
        glTranslated(-2.4,1.2,-6);
        glBegin(GL_TRIANGLES);
            glVertex3f( 0.2f, 0.2f, 0.2f);    // Top Right Of The Quad (Front)
            glVertex3f(-0.2f, 0.2f, 0.2f);    // Top Left Of The Quad (Front)
            glVertex3f(-0.2f,-0.2f, 0.2f);    // Bottom Left Of The Quad (Front)
        glEnd();
    glPopMatrix();
*/
    glPushMatrix();
        glTranslated(-5.0,-3.0,-30);
        /*glBegin(GL_LINE_LOOP);
            glVertex3f( -10.0f, -4.1f, 0.0f);
            glVertex3f(4.5f, 13.7f, 0.0f);
            glVertex3f(24.0f,-6.3f, 0.0f);
        glEnd();*/
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

        printTrianglePool();
    glPopMatrix();

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
        vertex vertextp;
        for(int i=0;i<15;i++)
        {
            vertextp.x = vertices[i][0];
            vertextp.y = vertices[i][1];
            vertextp.z = vertices[i][2];
            theSetOfInputPoint.push_back(vertextp);
        }
        printf("test\n");
        generateDelaunayTriangle();

        theSetOfInputPoint.clear();

        //makeMesh();

        meshBeenMade=true;
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
