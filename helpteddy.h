#ifndef __helpteddy_h__
#define __helpteddy_h__


#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

//help teddy
bool onTheSameSide(float test_point, float line_start, float line_end, float );
bool insideTheTriangle(float test_vertex, float vertex_1, float vertex_2, float vertex_3);
bool insideTheCircle(float test_vertex, float center_of_circle, float radius);
void triangle();


#endif
