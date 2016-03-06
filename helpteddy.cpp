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

//according to wiki's data
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

float radiusOfCCircle(vertex testvertex, vertex center)
{
    float radius = (testvertex.x-center.x)^2 + (testvertex.y-center.y)^2;
    radius = sqrt(radius);

    return radius;
}

bool isBadTriangle(vertex test_point, vertex center, float radius)
{
    if( insideTheCircle(test_point, center, radius) )
        return true;
    return false;
}

findSuperTriangle ( pointarray )
{
    vertex vertex_1 = { 0.0, 1.0, 0.0 };
    vertex vertex_2 = { 0.5, 0.0, 0.0};
    vertex vertex_3 = { -0.5, 0.0, 0.0};

    vertex test_vertex;

    for(i=0;i<sizeof pointarray;i++)
    {
        test_vertex = pointarray[i];

        if(outsideTheTriangle( test_vertex, vertex_1, vertex_2, vertex_3 ))

        vertex_1.x = vertex_1.x + vertex_1.x - test_vertex.x;
        vertex_2.x = vertex_2.x + vertex_2.x - test_vertex.x;
        vertex_3.x = vertex_3.x + vertex_3.x - test_vertex.x;

        vertex_1.y = vertex_1.y + vertex_1.y - test_vertex.y;
        vertex_2.y = vertex_2.y + vertex_2.y - test_vertex.y;
        vertex_3.y = vertex_3.y + vertex_3.y - test_vertex.y;
    }

    super1 = vertex_1;
    super2 = vertex_2;
    super3 = vertex_3;
}

//��}�Y
float addToTrianglePool()
{
    ;
}

//BowyerWatson
void generateDelaunayTruangle( theSetOfInputPoint, vertex super1, vertex super2, vertex super3 )
{
    vertex vertex_1;
    vertex vertex_2;
    vertex vertex_3;
    vertex center;
    float radius;

    findSuperTriangle( theSetOfInputPoint );
    for(i=0;i<sizeof theSetOfInputPoint;i++)
    {
        for(j=0;j<sizeof trianglePool;j++)
            {
                //push;
                center = centerOfCircumscribedCircle(vertex_1, vertex_2, vertex_3);
                radius = radiusOfCCircle( vertex_1, center );

                if( insideTheCircle(theSetOfInputPoint[i], center, radius) )
                    {
                        addToTrianglePool();
                    }
            }

        ;
    }
}


