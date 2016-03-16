#ifndef __helpteddy_h__
#define __helpteddy_h__


#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

//help teddy
bool onTheSameSide(float test_point, float line_start, float line_end, float );
bool outsideTheTriangle(float test_vertex, float vertex_1, float vertex_2, float vertex_3);
bool insideTheCircle(float test_vertex, float center_of_circle, float radius);
vertex centerOfCircumscribedCircle(vertex vertex_1, vertex vertex_2, vertex vertex_3);
float radiusOfCCircle(vertex testvertex, vertex center);
bool isBadTriangle(vertex test_point, vertex center, float radius);
triangle findSuperTriangle ( pointarray );
float addToEdgelePool( vertex v1 , vertex v2 );
float addToTrianglePool( vertex v1 , vertex v2 , vertex v3 );
bool areSameEdges( edge edge1 , edge edge2 );
void deletDoubleEdge( edgePool );
void generateDelaunayTruangle( theSetOfInputPoint );

#endif
