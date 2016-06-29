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
#include <math.h>

int Width = 640;
int Height= 480;

using namespace std;

std::vector< vertex >   theSetOfMouse ;         //.stay                 ; ; Original input points
std::vector< vertex >	theSetOfInputPoint ;    //.clear() in the end   ; ; tmp input set, only used in Delaunay
//std::vector< vertex >	subInputPointSet ;		//.clear() in the end   ; ;
//std::vector< vertex >	tmp_PointSet ;			//.clear() in the end   ; ;
std::vector< vertex >	bone_vertex_pool ;      //.clear() in the end   ; ; warping
std::vector< vertex >	bone_prime_vPool ;		//.clear() in the end   ; ; warping
std::vector< vertex > 	bone_lower_vPool ;     	//.stay                 ; ; warping
std::vector< vertex > 	bone_uper_vPool ;       //.stay                 ; ; warping

std::vector< vertex > 	arch_pool_1 ;       	//.clear() in the end   ; ; warping
std::vector< vertex > 	arch_pool_2 ;       	//.clear() in the end   ; ; warping

std::vector< edge >		edgePool ;              //.clear() when started ; ; or in the end ;
std::vector< edge >		tmp_edgePool ;          //.clear() when started ; ; or in the end ;
std::vector< edge > 	bone_edgePool ;         //.stay                 ; ; 
//std::vector< edge > 	bone_lower_ePool ;     	//.stay                 ; ; warping
//std::vector< edge > 	bone_uper_ePool ;       //.stay                 ; ; warping

std::vector< triangle > trianglePool ;          //.stay                 ;;
std::vector< triangle > badTrianglePool ;       // ???   ;;
std::vector< triangle > subTrianglePool ;       //.clear() in the end   ;;

std::vector< triangle > meshPool ;       		//.stay                 ;;

triangle superDT;

bool meshBeenMade = true;

int test = 15;

int number;//for bone, isPathTriangles

GLfloat prev_mouse_X=0;
GLfloat prev_mouse_Y=0;

bool is_first_time = true;
bool drawn = true;

//BowyerWatson
bool isSameVertex( vertex A , vertex B )
{
	if(((A.x == B.x)&&(A.y == B.y))&&(A.z == B.z))
		return true;

	return false;
}

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
    vertex vertex_1 = { 0.0, 1.0, 0.0 };
    vertex vertex_2 = { 0.5, 0.0, 0.0};
    vertex vertex_3 = { -0.5, 0.0, 0.0};
/*
    vertex vertex_1 = {-10.0f, -4.1f, 0.0f};
    vertex vertex_2 = {4.5f, 13.7f, 0.0f};
    vertex vertex_3 = {24.0f,-6.3f, 0.0f};
*/
    triangle super;

    for(int i=0;i<theSetOfInputPoint.size();i++)
    {
        if(outsideTheTriangle( theSetOfInputPoint[i], vertex_1, vertex_2, vertex_3 ))
        {
            again:
            if(!onTheSameSide( theSetOfInputPoint[i], vertex_1, vertex_2, vertex_3 )){
                vertex_1.x = vertex_1.x + vertex_1.x - vertex_3.x;
                vertex_2.x = vertex_2.x + vertex_2.x - vertex_3.x;
                vertex_3.x = vertex_3.x + vertex_3.x - theSetOfInputPoint[i].x;

                vertex_1.y = vertex_1.y + vertex_1.y - vertex_3.y;
                vertex_2.y = vertex_2.y + vertex_2.y - vertex_3.y;
                vertex_3.y = vertex_3.y + vertex_3.y - theSetOfInputPoint[i].y;
            }
            if(!onTheSameSide( theSetOfInputPoint[i], vertex_2, vertex_3, vertex_1 )){
                vertex_2.x = vertex_2.x + vertex_2.x - vertex_1.x;
                vertex_3.x = vertex_3.x + vertex_3.x - vertex_1.x;

                vertex_2.y = vertex_2.y + vertex_2.y - vertex_1.y;
                vertex_3.y = vertex_3.y + vertex_3.y - vertex_1.y;
            }
            if(!onTheSameSide( theSetOfInputPoint[i], vertex_3, vertex_1, vertex_2 )){
                vertex_1.x = vertex_1.x + vertex_1.x - vertex_2.x;
                vertex_3.x = vertex_3.x + vertex_3.x - vertex_2.x;

                vertex_1.y = vertex_1.y + vertex_1.y - vertex_2.y;
                vertex_3.y = vertex_3.y + vertex_3.y - vertex_2.y;
            }
        }
        if (outsideTheTriangle( theSetOfInputPoint[i], vertex_1, vertex_2, vertex_3 ))
        goto again;
    }

    super.v1 = vertex_1;
    super.v2 = vertex_2;
    super.v3 = vertex_3;

    return super;
}

vertex findMidPoint( vertex v1, vertex v2 )
{
    vertex mid_point;
    mid_point.x = (v1.x + v2.x)*0.5;
    mid_point.y = (v1.y + v2.y)*0.5;
    mid_point.z = 0.0;//or transform to

    return mid_point;
}

vertex findCenterPoint( triangle cencer_triangle )
{
	vertex center_point;
    vertex mid_point;
    mid_point=findMidPoint(cencer_triangle.v2, cencer_triangle.v3);
    center_point.x = cencer_triangle.v1.x - (cencer_triangle.v1.x - mid_point.x)*0.66;
    center_point.y = cencer_triangle.v1.y - (cencer_triangle.v1.y - mid_point.y)*0.66;
    center_point.z = 0.0;//or transform to the plain you want

    return center_point;
}

//float might need to be changed
void addToEdgePool( vertex v1 , vertex v2 )
{
    edge edgetp;
    edgetp.v1 = v1;
    edgetp.v2 = v2;

    edgePool.push_back(edgetp);
}

void addToTmpEdgePool( vertex v1 , vertex v2 )
{
    edge edgetp;
    edgetp.v1 = v1;
    edgetp.v2 = v2;

    tmp_edgePool.push_back(edgetp);
}

void addToBoneEdgePool( vertex v1 , vertex v2 )
{
    edge edgetp;
    edgetp.v1 = v1;
    edgetp.v2 = v2;

    bone_edgePool.push_back(edgetp);
}

void addToTrianglePool( vertex v1 , vertex v2 , vertex v3 )
{
    triangle triangletp;
    triangletp.v1 = v1;
    triangletp.v2 = v2;
    triangletp.v3 = v3;

    trianglePool.push_back(triangletp);
}

void addToBadTrianglePool( vertex v1 , vertex v2 , vertex v3 )
{
    triangle triangletp;
    triangletp.v1 = v1;
    triangletp.v2 = v2;
    triangletp.v3 = v3;

    badTrianglePool.push_back(triangletp);
}

void addToMeshPool( vertex v1 , vertex v2 , vertex v3 )
{
    triangle triangletp;
    triangletp.v1 = v1;
    triangletp.v2 = v2;
    triangletp.v3 = v3;

    meshPool.push_back(triangletp);
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
/*
void cleanTheSubInputSet()
{
	int i;
    int j;
    int k;
    int h;

    start2:
    for(i=0;i<subInputPointSet.size();i++)
    {
        for(j=0; j < subInputPointSet.size(); j++)
        {
            if( isSameVertex(subInputPointSet[i],subInputPointSet[j]) && (i!=j) )
            {

                for(k=0; k<subInputPointSet.size(); k++){
                    if((k!=i)&&(k!=j))
                    tmp_PointSet.push_back(subInputPointSet[k]);
                }

                subInputPointSet.clear();
                for(h=0; h < tmp_PointSet.size(); h++){
                    subInputPointSet.push_back(tmp_PointSet[h]);
                }
                tmp_PointSet.clear();
                goto start2;
            }
        }
    }

    tmp_PointSet.clear();

    for(int i2=0; i2<theSetOfInputPoint.size(); i2++){
        for(int j2=0; j2<subInputPointSet.size(); j2++){
            if(isSameVertex(theSetOfInputPoint[i2],subInputPointSet[j2]))
                tmp_PointSet.push_back(subInputPointSet[j2]);
        }
    }

    subInputPointSet.clear();

    for(int m; m<tmp_PointSet.size(); m++){
    	subInputPointSet.push_back(tmp_PointSet[m]);
    }

    tmp_PointSet.clear();
}
*/
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

bool rotation_type( vertex v1, vertex v2, vertex v3 )//true:clockwise ;false:Counterclockwise
{
    vertex p;
    p.x= v1.x + v2.y - v1.y;
    p.y= v1.y + v1.x - v2.x;
    p.z=0.0;
    if( onTheSameSide( p, v1, v2, v3 ) )
        return true;	//clockwise
    return false;		//Counterclockwise
}

bool check_inputPonits_rotationtype()//true:clockwise ;false:Counterclockwise
{
	int size = theSetOfInputPoint.size();
	int n1,n2,n3;
	n1 = 0;
	n2 = size/3;
	n3 = (size*2)/3;
	return rotation_type( theSetOfInputPoint[n1], theSetOfInputPoint[n2], theSetOfInputPoint[n3] );
}

bool isOutEars( triangle testTri, edge testEdge, bool origins_Rotation_Type ) //true:clockwise ;false:Counterclockwise
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
            if( onTheSameSide( p, testTri.v1, testTri.v2, testTri.v3 ) )
            return true;
        }
        else if( areSameEdges( e2,testEdge ) ){
            p.x =testEdge.v1.x+testEdge.v2.y-testEdge.v1.y;
            p.y =testEdge.v1.y+testEdge.v1.x-testEdge.v2.x;
            p.z =0.0;
            if( onTheSameSide( p, testTri.v2, testTri.v3, testTri.v1 ) )
            return true;
        }
        else if( areSameEdges( e3,testEdge ) ){
            p.x =testEdge.v1.x+testEdge.v2.y-testEdge.v1.y;
            p.y =testEdge.v1.y+testEdge.v1.x-testEdge.v2.x;
            p.z =0.0;
            if( onTheSameSide( p, testTri.v3, testTri.v1, testTri.v2 ) )
            return true;
        }
    }
    }

    return false;
}

bool isPrimeEars( triangle testTri, bool origins_Rotation_Type )
{
	int t1,t2,t3;
	int n=0;
	for (int cot=0; cot<theSetOfInputPoint.size(); cot++)
	{
		if(n==0)
		{
			if(isSameVertex( testTri.v1 , theSetOfInputPoint[cot] ))  t1 = cot;
			if(isSameVertex( testTri.v2 , theSetOfInputPoint[cot] ))  t1 = cot;
			if(isSameVertex( testTri.v3 , theSetOfInputPoint[cot] ))  t1 = cot;
			n++;
		}

		if(n==1)
		{
			if(isSameVertex( testTri.v1 , theSetOfInputPoint[cot] ))  t2 = cot;
			if(isSameVertex( testTri.v2 , theSetOfInputPoint[cot] ))  t2 = cot;
			if(isSameVertex( testTri.v3 , theSetOfInputPoint[cot] ))  t2 = cot;
			n++;
		}

		if(n==2)
		{
			if(isSameVertex( testTri.v1 , theSetOfInputPoint[cot] ))  t3 = cot;
			if(isSameVertex( testTri.v2 , theSetOfInputPoint[cot] ))  t3 = cot;
			if(isSameVertex( testTri.v3 , theSetOfInputPoint[cot] ))  t3 = cot;
		}
	}
	//if( rotation_type( theSetOfInputPoint[t1], theSetOfInputPoint[t2], theSetOfInputPoint[t3] ) != origins_Rotation_Type )
    vertex p;
    p.x = theSetOfInputPoint[t1].x+theSetOfInputPoint[t2].y-theSetOfInputPoint[t1].y;
    p.y = theSetOfInputPoint[t1].y+theSetOfInputPoint[t1].x-theSetOfInputPoint[t2].x;
    p.z = 0.0;
	if( onTheSameSide( p, theSetOfInputPoint[t1], theSetOfInputPoint[t2], theSetOfInputPoint[t3] )&&( ( ((t2-t1)!=1)&&((t3-t2)!=1) ) &&((t1-t3)!=1) ) )//theSetOfInputPoint[t1+(t2-t1)/2]
		return true;
	return false;
}

void normalize( GLfloat vec[3])
{
	float lenth = vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2];
	lenth = sqrt(lenth);
	vec[0]= vec[0]/lenth;
	vec[1]= vec[1]/lenth;
	vec[2]= vec[2]/lenth;
}

bool isTooFlat( vertex mid_vertex, vertex long1_vertex, vertex long2_vertex )
{
	GLfloat vectorOne[] ={mid_vertex.x - long1_vertex.x, mid_vertex.y - long1_vertex.y, mid_vertex.z - long1_vertex.z};
	GLfloat vectorTwo[] ={long2_vertex.x - mid_vertex.x, long2_vertex.y - mid_vertex.y, long2_vertex.z - mid_vertex.z};
	GLfloat vectorBase[]={long2_vertex.x - long1_vertex.x, long2_vertex.y - long1_vertex.y, long2_vertex.z - long1_vertex.z};
	GLfloat oneDotTwo;

	normalize(vectorOne);
	normalize(vectorTwo);
	normalize(vectorBase);

	oneDotTwo = vectorOne[0]*vectorTwo[0] + vectorOne[1]*vectorTwo[1] + vectorOne[2]*vectorTwo[2];

	if(fabs(oneDotTwo-1)<0.005)
		return true;
	return false;
}

bool isNotTooSmall(triangle test_triangle)
{
	if(isTooFlat( test_triangle.v1, test_triangle.v2, test_triangle.v3 ))
		return false;
	if(isTooFlat( test_triangle.v2, test_triangle.v3, test_triangle.v1 ))
		return false;
	if(isTooFlat( test_triangle.v3, test_triangle.v1, test_triangle.v2 ))
		return false;
	return true;
}

void twoVertexIntoOneEdge(vertex vt1, vertex vt2, edge *edge_to_become)
{
    //edge_v1
    (*edge_to_become).v1.x = vt1.x;
    (*edge_to_become).v1.y = vt1.y;
    (*edge_to_become).v1.z = 0.0;

    //edge_v2
    (*edge_to_become).v2.x = vt2.x;
    (*edge_to_become).v2.y = vt2.y;
    (*edge_to_become).v2.z = 0.0;
}
/*
void substitudeTheInputSetWithSubInputSet()
{
	theSetOfInputPoint.clear();

    for(int m; m<subInputPointSet.size(); m++){
    	theSetOfInputPoint.push_back(subInputPointSet[m]);
    }
    subInputPointSet.clear();
/*----------------------------------/
    trianglePool.clear();

    for(int m; m<subTrianglePool.size(); m++){
    	trianglePool.push_back(subTrianglePool[m]);
    }
    subTrianglePool.clear();
/*----------------------------------/
    edgePool.clear();

    edge tmp_edge;
    int size;
    size = theSetOfInputPoint.size();
    for(int m; m<theSetOfInputPoint.size(); m++){
    	//twoVertexIntoOneEdge(theSetOfInputPoint[m],theSetOfInputPoint[(m+1)%size],tmp_edge);
    	tmp_edge.v1.x = theSetOfInputPoint[m].x;
        tmp_edge.v1.y = theSetOfInputPoint[m].y;
        tmp_edge.v1.z = 0.0;

        tmp_edge.v2.x = theSetOfInputPoint[(m+1)%size].x;
        tmp_edge.v2.y = theSetOfInputPoint[(m+1)%size].y;
        tmp_edge.v2.z = 0.0;
    	edgePool.push_back(tmp_edge);
    }

}
*/
bool isPathTriangle(triangle test_triangle)
{
    int count=0;
    edge e1, e2, e3;

    twoVertexIntoOneEdge(test_triangle.v1, test_triangle.v2, &e1);
    twoVertexIntoOneEdge(test_triangle.v2, test_triangle.v3, &e2);
    twoVertexIntoOneEdge(test_triangle.v3, test_triangle.v1, &e3);

    for(int i=0; i<tmp_edgePool.size(); i++)
    {
        if( areSameEdges(e1, tmp_edgePool[i]) )
            count++;
        if( areSameEdges(e2, tmp_edgePool[i]) )
            count++;
        if( areSameEdges(e3, tmp_edgePool[i]) )
            count++;
    	if(count==1)
    	{
        	number=i;
        	return true;
    	}
    }

    return false;
}

bool isCenterTriangle(triangle test_triangle)
{
	int count=0;
    edge e1, e2, e3;

    twoVertexIntoOneEdge(test_triangle.v1, test_triangle.v2, &e1);
    twoVertexIntoOneEdge(test_triangle.v2, test_triangle.v3, &e2);
    twoVertexIntoOneEdge(test_triangle.v3, test_triangle.v1, &e3);

    for(int i=0; i<tmp_edgePool.size(); i++)
    {
        if( areSameEdges(e1, tmp_edgePool[i]) )
            count++;
        if( areSameEdges(e2, tmp_edgePool[i]) )
            count++;
        if( areSameEdges(e3, tmp_edgePool[i]) )
            count++;
    }

    if(count==0)
        return true;

    return false;
}

void generateDelaunayTriangle()
{
    //triangle superDT;
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
        badTrianglePool.clear();
        for(int j=0; j<trianglePool.size(); j++)
            {
                test.v1 = trianglePool[j].v1;
                test.v2 = trianglePool[j].v2;
                test.v3 = trianglePool[j].v3;
                center = centerOfCircumscribedCircle( test.v1 , test.v2 , test.v3 );
                //center =
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

    /*--Trimming those (too small) triangles.--*/
/*
    for(int g=0; g<trianglePool.size(); g++)
    {
    	if( isNotTooSmall(trianglePool[g]) )
    	{
    		subInputPointSet.push_back(trianglePool[g].v1);
    		subInputPointSet.push_back(trianglePool[g].v2);
    		subInputPointSet.push_back(trianglePool[g].v3);

    		subTrianglePool.push_back(trianglePool[g]);
    	}
    	cleanTheSubInputSet();
    }

    substitudeTheInputSetWithSubInputSet();
*/
    /*--Trimming outside Triangles--*/
    //trim 01 : attached triangle
    edgePool.clear();
    for (int cot=0; cot<theSetOfInputPoint.size(); cot++){
        addToEdgePool( theSetOfInputPoint[cot], theSetOfInputPoint[(cot+1)%theSetOfInputPoint.size()] ); //(cot+1)%theSetOfInputPoint.size()
    }
    for (int cnt=0; cnt<edgePool.size(); cnt++) // done inserting points, now clean up
    {
        for (int ad=0; ad<trianglePool.size(); ad++){
            if( isOutEars( trianglePool[ad], edgePool[cnt] , false ) )
            {
                if(ad!=trianglePool.size()-1){
                    trianglePool[ad]=trianglePool[trianglePool.size()-1];
                    ad--;
                }
                trianglePool.pop_back();
            }
        }
    }

    //trim 02 : prime triangle
    /*
    bool originRtype = check_inputPonits_rotationtype();
    for (int ad=0; ad<trianglePool.size(); ad++)
    {
        if ( isPrimeEars(trianglePool[ad], originRtype ) )
        {
            if( ad!=trianglePool.size()-1 ){
                    trianglePool[ad]=trianglePool[trianglePool.size()-1];
                    ad--;
            }
            trianglePool.pop_back();
        }
    }
*/
}

void generateBoneLine()
{

		tmp_edgePool.clear();
    	for(int i=0; i<theSetOfInputPoint.size(); i++)
    	{
        	if(i!=theSetOfInputPoint.size()-1)
            	addToTmpEdgePool(theSetOfInputPoint[i],theSetOfInputPoint[i+1]);
        	else
            	addToTmpEdgePool(theSetOfInputPoint[i],theSetOfInputPoint[0]);
    	}

	    for(int i=0; i<trianglePool.size(); i++)
	    {
	    	if( isCenterTriangle(trianglePool[i]) )
	    	{
	    		vertex center = findCenterPoint(trianglePool[i]);
	    		vertex mid1 = findMidPoint(trianglePool[i].v1, trianglePool[i].v2);
	    		vertex mid2 = findMidPoint(trianglePool[i].v2, trianglePool[i].v3);
	    		vertex mid3 = findMidPoint(trianglePool[i].v3, trianglePool[i].v1);
	    		addToBoneEdgePool(center, mid1);
	    		addToBoneEdgePool(center, mid2);
	    		addToBoneEdgePool(center, mid3);
	    	}
	    	if( isPathTriangle(trianglePool[i]) )
	    	{
	    			edge tri_edge_1;
	    			edge tri_edge_2;
	    			edge tri_edge_3;

	    			tri_edge_1.v1 = trianglePool[i].v1;
	    			tri_edge_1.v2 = trianglePool[i].v2;

	    			tri_edge_2.v1 = trianglePool[i].v2;
	    			tri_edge_2.v2 = trianglePool[i].v3;

	    			tri_edge_3.v1 = trianglePool[i].v3;
	    			tri_edge_3.v2 = trianglePool[i].v1;

	    			vertex bone_edge_add_1;
	    			vertex bone_edge_add_2;

	    			if( areSameEdges(tri_edge_1, tmp_edgePool[number]) )
	    			{
	    				bone_edge_add_1 = findMidPoint(tri_edge_2.v1, tri_edge_2.v2);
	    				bone_edge_add_2 = findMidPoint(tri_edge_3.v1, tri_edge_3.v2);
	    				addToBoneEdgePool(bone_edge_add_1, bone_edge_add_2);
	    			}

	    			else if( areSameEdges(tri_edge_2, tmp_edgePool[number]) )
	    			{
	    				bone_edge_add_1 = findMidPoint(tri_edge_3.v1, tri_edge_3.v2);
	    				bone_edge_add_2 = findMidPoint(tri_edge_1.v1, tri_edge_1.v2);
	    				addToBoneEdgePool(bone_edge_add_1, bone_edge_add_2);
	    			}

	    			else if( areSameEdges(tri_edge_3, tmp_edgePool[number]) )
	    			{
	    				bone_edge_add_1 = findMidPoint(tri_edge_1.v1, tri_edge_1.v2);
	    				bone_edge_add_2 = findMidPoint(tri_edge_2.v1, tri_edge_2.v2);
	    				addToBoneEdgePool(bone_edge_add_1, bone_edge_add_2);
	    			}
	    		}

	    }
		cout<<"test"<<"\n";
        cout<<"Size of BoneEdge: "<<bone_edgePool.size()<<"\n";
        cout<<"triangle number: "<<trianglePool.size()<<"\n";
        cout<<"tmp_edge number: "<<tmp_edgePool.size()<<"\n";
        cout<<"\n";
        cout<<"\n";
	tmp_edgePool.clear();
	edgePool.clear();
}

bool is_in_triangle_but_not_on_the_edge(vertex test_vertex, triangle test_triangle)
{
	if(!outsideTheTriangle(test_vertex, test_triangle.v1, test_triangle.v2, test_triangle.v3)){
		if(onTheTriangleEdges(test_vertex, test_triangle.v1, test_triangle.v2, test_triangle.v3))
			return false;
		return true;
	}
	return false;
}

bool is_a_prime_bone_line_vertex(vertex candidate)
{
	for(int i=0; i<trianglePool.size(); i++){
		if( is_in_triangle_but_not_on_the_edge(candidate, trianglePool[i]) ){
			return true;
		}
	}
	return false;
}

void delete_double_bone_vertex() // Clean the bone vertex pool, delete the double vertex
{
	int i;
    int j;
    int k;
    int h;

    start780:
    for(i=0;i<bone_vertex_pool.size();i++)
    {
        for(j=0;j<bone_vertex_pool.size();j++)
        {
            if( isSameVertex(bone_vertex_pool[i], bone_vertex_pool[j]) && (i!=j) )
            {

                for(k=0; k<bone_vertex_pool.size(); k++){
                    if((k!=i)&&(k!=j))
                    bone_prime_vPool.push_back(bone_vertex_pool[k]);
                }

                bone_vertex_pool.clear();
                for(h=0; h<tmp_edgePool.size(); h++){
                    bone_vertex_pool.push_back(bone_prime_vPool[h]);
                }
                bone_prime_vPool.clear();
                goto start780;
            }
        }
    }
    for(int m=0; m<bone_vertex_pool.size(); m++)
    {
    	for(int n=0; n<edgePool.size(); n++)
    	{
    		if(onTheEdge(bone_vertex_pool[m], edgePool.v1, edgePool.v2)){
    			bone_vertex_pool[m] = bone_vertex_pool[bone_vertex_pool.size()-1];
    			m--;
    			bone_vertex_pool.pop_back();
    		}
    	}
    }
}

void in_prime_bone_vpool(vertex test_v)
{
	for(int i=0; i<bone_prime_vPool.size(); i++)
	{
		if(isSameVertex(teat_v, bone_prime_vPool[i]))
			return true;
	}
	return false;
}

void convert_bone_edge_to_vertex()
{
	for(int i=0; i<bone_edgePool.size(); i++)
	{
		bone_vertex_pool.push_back(bone_edgePool[i].v1);
		bone_vertex_pool.push_back(bone_edgePool[i].v2);
	}
	delete_double_bone_vertex();
	
	//Add prime vertex to bone_prime_vPool
	for (int i = 0; i < bone_vertex_pool.size(); i++)
	{
		if(is_a_prime_bone_line_vertex(bone_vertex_pool[i]))
			bone_prime_vPool.push_back(bone_vertex_pool[i]);
	}
}

void create_uper_vpool()
{
	for(int i=0; i<bone_vertex_pool.size(); i++){
		for(int j=0; j<trianglePool.size(); j++){
			if(onTheEdge(bone_vertex_pool[i],trianglePool[j].v1,trianglePool[j].v2))
			{
				bone_vertex_pool[i].z = distance(bone_vertex_pool[i], trianglePool[j].v1);
			}
			
			if(onTheEdge(bone_vertex_pool[i],trianglePool[j].v2,trianglePool[j].v3))
			{
				bone_vertex_pool[i].z = distance(bone_vertex_pool[i], trianglePool[j].v2);
			}
			
			if(onTheEdge(bone_vertex_pool[i],trianglePool[j].v3,trianglePool[j].v1))
			{
				bone_vertex_pool[i].z = distance(bone_vertex_pool[i], trianglePool[j].v1);
			}

			if(in_prime_bone_vpool(bone_vertex_pool[i])){
				bone_vertex_pool[i].z = distance(bone_vertex_pool[i], trianglePool[j].v1);
			}
		}
	}

	for(int i=0; i<bone_vertex_pool.size(); i++){
		bone_uper_vPool.push_back(bone_vertex_pool[i]);
	}
}

void create_lower_vpool()
{
	for(int i=0; i<bone_vertex_pool.size(); i++){
		for(int j=0; j<trianglePool.size(); j++){
			if(onTheEdge(bone_vertex_pool[i],trianglePool[j].v1,trianglePool[j].v2))
			{
				bone_vertex_pool[i].z = 0.0 - distance(bone_vertex_pool[i], trianglePool[j].v1);
			}
			
			if(onTheEdge(bone_vertex_pool[i],trianglePool[j].v2,trianglePool[j].v3))
			{
				bone_vertex_pool[i].z = 0.0 - distance(bone_vertex_pool[i], trianglePool[j].v2);
			}
			
			if(onTheEdge(bone_vertex_pool[i],trianglePool[j].v3,trianglePool[j].v1))
			{
				bone_vertex_pool[i].z = 0.0 - distance(bone_vertex_pool[i], trianglePool[j].v1);
			}

			if(in_prime_bone_vpool(bone_vertex_pool[i])){
				bone_vertex_pool[i].z = 0.0 - distance(bone_vertex_pool[i], trianglePool[j].v1);
			}
		}
	}

	for(int i=0; i<bone_vertex_pool.size(); i++){
		bone_lower_vPool.push_back(bone_vertex_pool[i]);
	}
}

void addArch_stitch_store(vertex top, vertex ground, vertex top_2, vertex ground_2)
{
	vertex a12, a13, a14, a22, a23, a24;

	arch_pool_1.push_back(top);
	arch_pool_1.push_back(a12);
	arch_pool_1.push_back(a13);
	arch_pool_1.push_back(a14);
	arch_pool_1.push_back(ground);

	arch_pool_2.push_back(top_2);
	arch_pool_2.push_back(a22);
	arch_pool_2.push_back(a23);
	arch_pool_2.push_back(a24);
	arch_pool_2.push_back(ground_2);


	if(isSameVertex(arch_pool_1[0], arch_pool_2[0])){
		for(int i=0; i<arch_pool_1.size()-1; i++)
		{
			if(i==0){
				addToMeshPool(arch_pool_1[i], arch_pool_2[i+1], arch_pool_1[i+1]);
			}
			else{
				addToMeshPool(arch_pool_1[i], arch_pool_2[i], arch_pool_2[i+1]);
				addToMeshPool(arch_pool_1[i], arch_pool_2[i+1], arch_pool_1[i+1]);
			}
		}
	}
	
	else if(isSameVertex(arch_pool_1[4], arch_pool_2[4])){
		for(int i=0; i<arch_pool_1.size()-1; i++)
		{
			if(i==arch_pool_1.size()-2){
				addToMeshPool(arch_pool_1[i], arch_pool_2[i], arch_pool_2[i+1]);
			}
			else{
				addToMeshPool(arch_pool_1[i], arch_pool_2[i], arch_pool_2[i+1]);
				addToMeshPool(arch_pool_1[i], arch_pool_2[i+1], arch_pool_1[i+1]);
			}
		}
	}

	else{
		for(int i=0; i<arch_pool_1.size()-1; i++)
		{
			addToMeshPool(arch_pool_1[i], arch_pool_2[i], arch_pool_2[i+1]);
			addToMeshPool(arch_pool_1[i], arch_pool_2[i+1], arch_pool_1[i+1]);
		}
	}

	arch_pool_1.clear();
	arch_pool_2.clear();
}

void generateMesh()
{
	//calculate the height of each bone line;
	convert_bone_edge_to_vertex();
	create_lower_vpool();
	create_uper_vpool();
	/*bone_vertex_pool.clear();*/
	/*bone_edgePool.clear();*/

	//draw arch uper
	for(int i=0; i<bone_edgePool.size(); i++)
	{
		int set=0;
		int let_i=0;
		int let_j=0;

		for(int j=0; j<bone_vertex_pool.size(); j++)
		{
			if(isSameVertex(bone_edgePool[i].v1, bone_vertex_pool[j])){ let_i=j; set++; }//set=1
			if(isSameVertex(bone_edgePool[i].v2, bone_vertex_pool[j])){ let_j=j; set++; }//set=2
		}
		
		for(int j=0; j<bone_vertex_pool.size(); j++)
		{
			//set=1 means End Edge
			if(set==1){
				for(int k=0; k<trianglePool.size(); j++)
				{
					if(onTheEdge(bone_vertex_pool[j], trianglePool.v1, trianglePool.v2)){
						addArch_stitch_store( bone_uper_vPool[j], trianglePool.v1, bone_uper_vPool[j], trianglePool.v3 );
						addArch_stitch_store( bone_uper_vPool[j], trianglePool.v2, bone_uper_vPool[j], trianglePool.v3 );
					}

					if(onTheEdge(bone_vertex_pool[j], trianglePool.v2, trianglePool.v3)){
						addArch_stitch_store( bone_uper_vPool[j], trianglePool.v2, bone_uper_vPool[j], trianglePool.v1 );
						addArch_stitch_store( bone_uper_vPool[j], trianglePool.v3, bone_uper_vPool[j], trianglePool.v1 );
					}

					if(onTheEdge(bone_vertex_pool[j], trianglePool.v3, trianglePool.v1)){
						addArch_stitch_store( bone_uper_vPool[j], trianglePool.v3, bone_uper_vPool[j], trianglePool.v2 );
						addArch_stitch_store( bone_uper_vPool[j], trianglePool.v1, bone_uper_vPool[j], trianglePool.v2 );
					}
				}
			}

			//set=2 means Path Edge or Prime-Path Edge
			if(set==2){
				short set_2 = 0;
				short set_v = 0;

				for(int p=0; p<bone_prime_vPool.size();p++)// Decide Bone_Edge is (Prime Edge / Prime-Path Edge)
				{
					if( isSameVertex(bone_edgePool[i].v1, bone_prime_vPool[p]) ){ 
						set_2 = 1;
						set_v = 1;
					}
					if( isSameVertex(bone_edgePool[i].v2, bone_prime_vPool[p]) ){ 
						set_2 = 1;
						set_v = 2;
					}
				}

				if( set_2 == 0 ){// Path Edge
					if(let_i==j)
					{
						for(int k=0; k<trianglePool.size(); j++)
						{
							if(onTheEdge(bone_vertex_pool[j], trianglePool.v1, trianglePool.v2)&&onTheEdge(bone_vertex_pool[let_j], trianglePool.v2, trianglePool.v3)){
							addArch_stitch_store( bone_uper_vPool[j], trianglePool.v2, bone_uper_vPool[let_j], trianglePool.v2 );//22
							addArch_stitch_store( bone_uper_vPool[j], trianglePool.v1, bone_uper_vPool[let_j], trianglePool.v3 );//13
							}

							if(onTheEdge(bone_vertex_pool[j], trianglePool.v2, trianglePool.v3)&&onTheEdge(bone_vertex_pool[let_j], trianglePool.v3, trianglePool.v1)){
							addArch_stitch_store( bone_uper_vPool[j], trianglePool.v3, bone_uper_vPool[let_j], trianglePool.v3 );//33
							addArch_stitch_store( bone_uper_vPool[j], trianglePool.v2, bone_uper_vPool[let_j], trianglePool.v1 );//21
							}

							if(onTheEdge(bone_vertex_pool[j], trianglePool.v3, trianglePool.v1)&&onTheEdge(bone_vertex_pool[let_j], trianglePool.v1, trianglePool.v2)){
							addArch_stitch_store( bone_uper_vPool[j], trianglePool.v1, bone_uper_vPool[let_j], trianglePool.v1 );//11
							addArch_stitch_store( bone_uper_vPool[j], trianglePool.v3, bone_uper_vPool[let_j], trianglePool.v2 );//32
							}

							if(onTheEdge(bone_vertex_pool[let_j], trianglePool.v1, trianglePool.v2)&&onTheEdge(bone_vertex_pool[j], trianglePool.v2, trianglePool.v3)){
							addArch_stitch_store( bone_uper_vPool[j], trianglePool.v2, bone_uper_vPool[let_j], trianglePool.v2 );//22
							addArch_stitch_store( bone_uper_vPool[j], trianglePool.v3, bone_uper_vPool[let_j], trianglePool.v1 );//31
							}

							if(onTheEdge(bone_vertex_pool[let_j], trianglePool.v2, trianglePool.v3)&&onTheEdge(bone_vertex_pool[j], trianglePool.v3, trianglePool.v1)){
							addArch_stitch_store( bone_uper_vPool[j], trianglePool.v3, bone_uper_vPool[let_j], trianglePool.v3 );//33
							addArch_stitch_store( bone_uper_vPool[j], trianglePool.v1, bone_uper_vPool[let_j], trianglePool.v2 );//12
							}

							if(onTheEdge(bone_vertex_pool[let_j], trianglePool.v3, trianglePool.v1)&&onTheEdge(bone_vertex_pool[j], trianglePool.v1, trianglePool.v2)){
							addArch_stitch_store( bone_uper_vPool[j], trianglePool.v1, bone_uper_vPool[let_j], trianglePool.v1 );//11
							addArch_stitch_store( bone_uper_vPool[j], trianglePool.v2, bone_uper_vPool[let_j], trianglePool.v3 );//23
							}
						}
					}
				}

				if( set_2 == 1 ){// Prime-Path Edge
					for(int k=0; k<trianglePool.size(); j++)
					{
						if(let_i==j && set_v==2)
						{
							if(onTheEdge(bone_vertex_pool[j], trianglePool.v1, trianglePool.v2)){
							addArch_stitch_store( bone_uper_vPool[j], trianglePool.v1, bone_uper_vPool[let_j], trianglePool.v1 );
							addArch_stitch_store( bone_uper_vPool[j], trianglePool.v2, bone_uper_vPool[let_j], trianglePool.v2 );
							}//Arch_stitch_store( edge_bone_v, trianglePool.v2, prime_bone_v, trianglePool.v2 );

							if(onTheEdge(bone_vertex_pool[j], trianglePool.v2, trianglePool.v3)){
							addArch_stitch_store( bone_uper_vPool[j], trianglePool.v2, bone_uper_vPool[let_j], trianglePool.v2 );
							addArch_stitch_store( bone_uper_vPool[j], trianglePool.v3, bone_uper_vPool[let_j], trianglePool.v3 );
							}//Arch_stitch_store( edge_bone_v, trianglePool.v2, prime_bone_v, trianglePool.v2 );

							if(onTheEdge(bone_vertex_pool[j], trianglePool.v3, trianglePool.v1)){
							addArch_stitch_store( bone_uper_vPool[j], trianglePool.v3, bone_uper_vPool[let_j], trianglePool.v3 );
							addArch_stitch_store( bone_uper_vPool[j], trianglePool.v1, bone_uper_vPool[let_j], trianglePool.v1 );
							}//Arch_stitch_store( edge_bone_v, trianglePool.v2, prime_bone_v, trianglePool.v2 );
						}
						if(let_j==j && set_v==1)
						{
							if(onTheEdge(bone_vertex_pool[j], trianglePool.v1, trianglePool.v2)){
							addArch_stitch_store( bone_uper_vPool[j], trianglePool.v1, bone_uper_vPool[let_i], trianglePool.v1 );
							addArch_stitch_store( bone_uper_vPool[j], trianglePool.v2, bone_uper_vPool[let_i], trianglePool.v2 );
							}//Arch_stitch_store( edge_bone_v, trianglePool.v2, prime_bone_v, trianglePool.v2 );

							if(onTheEdge(bone_vertex_pool[j], trianglePool.v2, trianglePool.v3)){
							addArch_stitch_store( bone_uper_vPool[j], trianglePool.v2, bone_uper_vPool[let_i], trianglePool.v2 );
							addArch_stitch_store( bone_uper_vPool[j], trianglePool.v3, bone_uper_vPool[let_i], trianglePool.v3 );
							}//Arch_stitch_store( edge_bone_v, trianglePool.v2, prime_bone_v, trianglePool.v2 );

							if(onTheEdge(bone_vertex_pool[j], trianglePool.v3, trianglePool.v1)){
							addArch_stitch_store( bone_uper_vPool[j], trianglePool.v3, bone_uper_vPool[let_i], trianglePool.v3 );
							addArch_stitch_store( bone_uper_vPool[j], trianglePool.v1, bone_uper_vPool[let_i], trianglePool.v1 );
							}//Arch_stitch_store( edge_bone_v, trianglePool.v2, prime_bone_v, trianglePool.v2 );
						}
					}
				}
			}

		}			
		
	}

	//draw arch lower

	/*
	for( each vertex in bone line ){
		for( each edge in edge Pool ){
			if( edge near bone line ){
				;
			}
			if( edge near bone line ){
				;
			}
			generate mesh;
			store mesh into mesh pool;
		}
	}*/

}

void printTrianglePool()
{
    for(int i=0; i<trianglePool.size(); i++)
    {
        glBegin(GL_LINE_LOOP);
            glVertex3f( trianglePool[i].v1.x, trianglePool[i].v1.y, trianglePool[i].v1.z );
            glVertex3f( trianglePool[i].v2.x, trianglePool[i].v2.y, trianglePool[i].v2.z );
            glVertex3f( trianglePool[i].v3.x, trianglePool[i].v3.y, trianglePool[i].v3.z );
        glEnd();
    }
}

void drawFlatTriangleBase()
{
    if(!meshBeenMade)
    {
        for(int i=0;i<theSetOfMouse.size();i=i+2){
            theSetOfInputPoint.push_back(theSetOfMouse[i]);
        }
        //cout<<"test"<<"\n";
        generateDelaunayTriangle();
        generateBoneLine();
        theSetOfInputPoint.clear();

        meshBeenMade=true;
    }
}

//Mouse draw
void mousebutton( int button, int state, int x, int y )
{
    if(button==GLUT_LEFT_BUTTON) {
        switch(state) {
			case GLUT_DOWN:
				drawn = false;
				break;
			case GLUT_UP:
				drawn = true;
				cout<<"Size of Stroke: "<<theSetOfMouse.size()<<"\n";

                cout<<"\n";
                meshBeenMade = false;
                drawFlatTriangleBase();
                // theSetOfMouse.clear();
				break;
		}
	}
	if(button==GLUT_RIGHT_BUTTON) {
        theSetOfMouse.clear();
        theSetOfInputPoint.clear();
        trianglePool.clear();
        edgePool.clear();
        bone_edgePool.clear();
        prev_mouse_X=0;
        prev_mouse_Y=0;
        is_first_time=true;
	}
}

void recordMousePos( int x, int y )
{
    vertex pre_pos;
    GLfloat xf,yf;

    xf = x;
    yf = y;
    xf = (xf-Width/2)/(Width/40);
    yf = (yf-Height/2)/(Height/30);
/*
    GLfloat deltaX = xf - prev_mouse_X;
    GLfloat deltaY = yf - prev_mouse_Y;

    xf = xf + deltaX/1.25;
    yf = yf + deltaY/1.25;
*/
    pre_pos.x=xf;
    pre_pos.y=-yf;
    pre_pos.z=0.0;
    theSetOfMouse.push_back(pre_pos);
/*
    prev_mouse_X = xf;
    prev_mouse_Y = yf;
*/
}

void printStroke()
{
    //if(!drawn)
    //{
    glBegin(GL_LINE_STRIP);
        for(int i=0;i<theSetOfMouse.size();i++){
            glVertex3f( theSetOfMouse[i].x, theSetOfMouse[i].y, theSetOfMouse[i].z );
        }
    glEnd();
    //}
}

void printBone()
{
	for(int i=0;i<bone_edgePool.size();i++){
		glBegin(GL_LINE_STRIP);
			glVertex3f( bone_edgePool[i].v1.x, bone_edgePool[i].v1.y, bone_edgePool[i].v1.z );
            glVertex3f( bone_edge Pool[i].v2.x, bone_edgePool[i].v2.y, bone_edgePool[i].v2.z );
    	glEnd();
	}
}

/* GLUT callback Handlers */
static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;
    Width  = width;
    Height = height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

static void display(void)
{
    /*const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    const double a = t*90.0;*/

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1,0,0);

    //glBegin(GL_TRIANGLES);

    glPushMatrix();

        glTranslated(0.0,0.0,-30);
        printTrianglePool();


    glPopMatrix();

    glPushMatrix();
        glTranslated(0.0,0.0,-30);
        printStroke();
    glPopMatrix();

    glPushMatrix();
    	glColor3d(0,0,1);
        glTranslated(0.0,0.0,-30);
        printBone();
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

        case 'r':
            meshBeenMade=false;
            test=test+1;
            test=test%16;
            if(test%16==0)
                test=5;
            drawFlatTriangleBase();
            break;

        case 'e':
            meshBeenMade=false;
            test=test-1;
            test=test%16;
            if(test%16<0)
                test=15;
            drawFlatTriangleBase();
            break;

        case 'a':
            meshBeenMade=false;
            test=15;
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
    drawFlatTriangleBase();

    glutInit(&argc, argv);
    glutInitWindowSize(Width,Height);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GLUT Shapes");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);

    glutMouseFunc( mousebutton );           // when mouse moves
    glutMotionFunc( recordMousePos );       // when mouse drags around

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
