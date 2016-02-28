/*
    Jui Cheng, Sung. R.O.C.
    Lyre Mellark.
    2016.Feb.10
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

struct vertex
{
    float x;
    float y;
    float z;
};

bool onTheSameSide(vertex test_point, vertex line_start, vertex line_end, vertex compare_point)
{
    if(line_start.x);
}

bool outsideTheTriangle(vertex test_vertex, vertex vertex_1, vertex vertex_2, vertex vertex_3)
{
    if( onTheSameSide(test_vertex, vertex_1, vertex_2, vertex_3) &&
        onTheSameSide(test_vertex, vertex_3, vertex_1, vertex_2) &&
        onTheSameSide(test_vertex, vertex_2, vertex_3, vertex_1) )
        return false;

    return true;
}

bool insideTheCircle(vertex test_vertex, vertex center_of_circle, float radius)
{
    float distance2;
    distance2 = (test_vertex.x-center_of_circle.x)^2 + (test_vertex.y-center_of_circle.y)^2 ;
    if( sqrt(distance2) > radius )
        return false;

    return true;
}

centerOfCircumscribedCircle(vertex vertex_1, vertex vertex_2, vertex vertex_3)
{
    float xc =((vertex_1.x)^2+(vertex_1.y)^2)*vertex_2.y +
              ((vertex_2.x)^2+(vertex_2.y)^2)*vertex_3.y +
              ((vertex_3.x)^2+(vertex_3.y)^2)*vertex_1.y -
              ((vertex_1.x)^2+(vertex_1.y)^2)*vertex_3.y -
              ((vertex_2.x)^2+(vertex_2.y)^2)*vertex_1.y -
              ((vertex_3.x)^2+(vertex_3.y)^2)*vertex_2.y;
    float xm =vertex_1.x*vertex_2.y +
              vertex_2.x*vertex_3.y +
              vertex_3.x*vertex_1.y -
              vertex_1.x*vertex_3.y -
              vertex_2.x*vertex_1.y -
              vertex_3.x*vertex_2.y -;

    float yc =((vertex_1.x)^2+(vertex_1.y)^2)*vertex_3.x +
              ((vertex_2.x)^2+(vertex_2.y)^2)*vertex_1.x +
              ((vertex_3.x)^2+(vertex_3.y)^2)*vertex_2.x -
              ((vertex_1.x)^2+(vertex_1.y)^2)*vertex_2.x -
              ((vertex_2.x)^2+(vertex_2.y)^2)*vertex_3.x -
              ((vertex_3.x)^2+(vertex_3.y)^2)*vertex_1.x;
    float ym =vertex_1.x*vertex_2.y +
              vertex_2.x*vertex_3.y +
              vertex_3.x*vertex_1.y -
              vertex_1.x*vertex_3.y -
              vertex_2.x*vertex_1.y -
              vertex_3.x*vertex_2.y -;
    xm=2*xm;
    ym=2*ym;

    vertex center;
    cenetr.x =xc/xm;
    cenetr.y =yc/ym;
    cenetr.z =0.0;
}

void triangle()
{
    ;
}


#include <stdlib.h>

