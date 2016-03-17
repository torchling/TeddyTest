/*
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

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <algorithm>
#include <vector>

using namespace std;

struct vertex
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
};

struct edge
{
    vertex v1;
    vertex v2;
};

struct triangle
{
    vertex v1;
    vertex v2;
    vertex v3;
};


bool onTheSameSide(vertex test_point, vertex line_start, vertex line_end, vertex compare_point)
{
    if(line_start.x!=line_end.x)
    {
        float vx = line_start.x-line_end.x;
        float vy = line_start.y-line_end.y;

        if( (test_point.y - (line_start.y + vy*(test_point.x-line_start.x)/vx))*
           (compare_point.y - (line_start.y + vy*(compare_point.x-line_start.x)/vx)) <= 0 )
            return false;


        return true;
    }

    if(line_start.x==line_end.x)
    {
        if( (test_point.x-line_start.x)*(compare_point.x-line_start.x) <= 0 )
            return false;

        return true;
    }

}

bool outsideTheTriangle(vertex testvertex, vertex vertex1, vertex vertex2, vertex vertex3)
{
    if( onTheSameSide(testvertex, vertex1, vertex2, vertex3) &&
        onTheSameSide(testvertex, vertex3, vertex1, vertex2) &&
        onTheSameSide(testvertex, vertex2, vertex3, vertex1) )
        return false;

    return true;
}

bool insideTheCircle(vertex test_vertex, vertex center_of_circle, float radius)
{
    float distance2;
    distance2 = pow(test_vertex.x-center_of_circle.x, 2.0) + pow(test_vertex.y-center_of_circle.y, 2.0) ;
    if( sqrt(distance2) > radius )
        return false;

    return true;
}

//according to wiki's data
vertex centerOfCircumscribedCircle(vertex vertex_1, vertex vertex_2, vertex vertex_3)
{


    float xc =(pow(vertex_1.x, 2.0)+pow(vertex_1.y, 2.0))*vertex_2.y +
              (pow(vertex_2.x, 2.0)+pow(vertex_2.y, 2.0))*vertex_3.y +
              (pow(vertex_3.x, 2.0)+pow(vertex_3.y, 2.0))*vertex_1.y -
              (pow(vertex_1.x, 2.0)+pow(vertex_1.y, 2.0))*vertex_3.y -
              (pow(vertex_2.x, 2.0)+pow(vertex_2.y, 2.0))*vertex_1.y -
              (pow(vertex_3.x, 2.0)+pow(vertex_3.y, 2.0))*vertex_2.y;
    float xm =vertex_1.x*vertex_2.y +
              vertex_2.x*vertex_3.y +
              vertex_3.x*vertex_1.y -
              vertex_1.x*vertex_3.y -
              vertex_2.x*vertex_1.y -
              vertex_3.x*vertex_2.y;

    float yc =(pow(vertex_1.x, 2.0)+pow(vertex_1.y, 2.0))*vertex_3.x +
              (pow(vertex_2.x, 2.0)+pow(vertex_2.y, 2.0))*vertex_1.x +
              (pow(vertex_3.x, 2.0)+pow(vertex_3.y, 2.0))*vertex_2.x -
              (pow(vertex_1.x, 2.0)+pow(vertex_1.y, 2.0))*vertex_2.x -
              (pow(vertex_2.x, 2.0)+pow(vertex_2.y, 2.0))*vertex_3.x -
              (pow(vertex_3.x, 2.0)+pow(vertex_3.y, 2.0))*vertex_1.x;
    float ym =vertex_1.x*vertex_2.y +
              vertex_2.x*vertex_3.y +
              vertex_3.x*vertex_1.y -
              vertex_1.x*vertex_3.y -
              vertex_2.x*vertex_1.y -
              vertex_3.x*vertex_2.y;
    xm=2*xm;
    ym=2*ym;

    vertex center;
    center.x =xc/xm;
    center.y =yc/ym;
    center.z =0.0;

    return center;
}

GLfloat radiusOfCCircle(vertex testvertex, vertex center)
{
    float radius = pow(testvertex.x-center.x, 2.0) + pow(testvertex.y-center.y, 2.0);
    radius = sqrt(radius);

    return radius;
}

//might be ok to delete  bool isBadTriangle
bool isBadTriangle(vertex test_point, vertex center, float radius)
{
    if( insideTheCircle(test_point, center, radius) ){return true;}
    return false;
}


bool areSameEdges( edge edge1 , edge edge2 )
{
    bool v1122 =(((edge1.v1.x == edge2.v1.x)&&(edge1.v1.y == edge2.v1.y))&&(edge1.v1.z == edge2.v1.z))&&
                (((edge1.v2.x == edge2.v2.x)&&(edge1.v2.y == edge2.v2.y))&&(edge1.v2.z == edge2.v2.z));
    bool v1221 =(((edge1.v1.x == edge2.v2.x)&&(edge1.v1.y == edge2.v2.y))&&(edge1.v1.z == edge2.v2.z))&&
                (((edge1.v2.x == edge2.v1.x)&&(edge1.v2.y == edge2.v1.y))&&(edge1.v2.z == edge2.v1.z));

	if(v1122){
		return true;}
	else if(v1221){
		return true;}

	return false;
}

