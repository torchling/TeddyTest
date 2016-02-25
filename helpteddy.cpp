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

bool insideTheCircle(vertex test_vertex, vertex center_of_circle, float radius)
{
    float distance2;
    distance2 = (test_vertex.x-center_of_circle.x)^2 + (test_vertex.y-center_of_circle.y)^2 ;
    if( sqrt(distance2) > radius )
        return false;

    return true;
}

vertex centerOftheCircle(vertex vertex_1, vertex vertex_2, vertex vertex_3)
{
    vertex center;
    return ;
}

bool isBadTriangle(vertex test_point, vertex center, float radius)
{
    if( insideTheCircle(test_point, center, radius) )
        return true;
    return false;
}

void triangle()
{
    ;
}


#include <stdlib.h>

