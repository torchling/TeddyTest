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
    float x;
    float y;
    float z;
};

struct edge
{
    vertex v1;
    vertex v2;
    bool b=1;
};

struct triangle
{
    vertex v1;
    vertex v2;
    vertex v3;
};

std::vector< vertex >	theSetOfInputPoint ;
std::vector< edge >		edgePool ;
std::vector< triangle > trianglePool ;

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

float radiusOfCCircle(vertex testvertex, vertex center)
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

triangle findSuperTriangle( vector<vertex>& inputPoint )
{

    vertex vertex_1 = { 0.0, 1.0, 0.0 };
    vertex vertex_2 = { 0.5, 0.0, 0.0};
    vertex vertex_3 = { -0.5, 0.0, 0.0};

    triangle super;
    printf("test02\n");

    for(int i=0;i<inputPoint.size();i++)
    {
        if(outsideTheTriangle( inputPoint[i], vertex_1, vertex_2, vertex_3 ))
        {
        	vertex_1.x = vertex_1.x + vertex_1.x - inputPoint[i].x;
        	vertex_2.x = vertex_2.x + vertex_2.x - inputPoint[i].x;
        	vertex_3.x = vertex_3.x + vertex_3.x - inputPoint[i].x;

        	vertex_1.y = vertex_1.y + vertex_1.y - inputPoint[i].y;
        	vertex_2.y = vertex_2.y + vertex_2.y - inputPoint[i].y;
        	vertex_3.y = vertex_3.y + vertex_3.y - inputPoint[i].y;
        }

    }

    super.v1 = vertex_1;
    super.v2 = vertex_2;
    super.v3 = vertex_3;

    return super;
}

//float might need to be changed
void addToEdgelePool( vertex v1 , vertex v2 )
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

void deletDoubleEdge( vector<edge>& edgepool )
{
    int i;
    int j;
    int k=0;
    for(i=0;i<edgepool.size();i++)
    {
        for(j=0;j<edgepool.size();j++)
        {
            if( areSameEdges(edgepool[i],edgepool[j]) && (i!=j) )
            {
                if((i+j)>1)
                //delete edgePool[j] from edgePooL;
                edgepool[j].b=0;
            }
        }
    }
}

//BowyerWatson
void generateDelaunayTriangle( vector<vertex>& theSetofInputPoint )
{

    triangle superDT;
    edge edgeFT;
    vertex center;
    float radius;

    superDT = findSuperTriangle( theSetofInputPoint );
    printf("test03\n");
    addToTrianglePool(superDT.v1, superDT.v2, superDT.v3);


    for(int i=0; i<theSetofInputPoint.size(); i++)
    {
        for(int j=0; j<trianglePool.size(); j++)
            {
                superDT.v1 = trianglePool[j].v1;
                superDT.v2 = trianglePool[j].v2;
                superDT.v3 = trianglePool[j].v3;
                center = centerOfCircumscribedCircle( superDT.v1 , superDT.v2 , superDT.v3 );
                radius = radiusOfCCircle( superDT.v1 , center );

                if( insideTheCircle( theSetOfInputPoint[i] , center , radius ) )
                	{
                        addToEdgelePool( superDT.v1 , superDT.v2 );
						addToEdgelePool( superDT.v2 , superDT.v3 );
                  		addToEdgelePool( superDT.v3 , superDT.v1 );
                    }
            }
        //trianglepool := emptyset;
        trianglePool.clear();

        deletDoubleEdge(edgePool);

        for(int k=0;k<edgePool.size();k++)
            {
            	edgeFT = edgePool[k];
                addToTrianglePool( edgeFT.v1 , edgeFT.v2 , theSetOfInputPoint[i] );
            }
        //edgePool := emptyset;
        if((i+1)<theSetOfInputPoint.size())
        edgePool.clear();

    }
}


