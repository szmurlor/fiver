//
// C++ Implementation: fvboundbox
//
// Description: 
//
//
// Author: Robert Szmurlo <robert@iem.pw.edu.pl>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "fvboundbox.h"
#include "fivermain.h"
#include <QtDebug>

FVBoundBox::FVBoundBox( FVBoxMgr * manager, int x, int y )
: FVObject(manager,x,y)
{
	initDrawable("BoundingBox","Draw");
}


FVBoundBox::~FVBoundBox()
{
}

void FVBoundBox::paintGL( )
{
	readAttribs();
	
	fvlist->start();
	glColor3f(0.0,0.0,0.0);
	glLineWidth( lineThickness );
	
	if (showScale) DrawScale(10,  p1[0],p2[0],   p1[0],p1[1],p1[2],   p2[0],p1[1],p1[2]);
	if (showScale) DrawScale(10,  p1[1],p2[1],   p1[0],p1[1],p1[2],   p1[0],p2[1],p1[2]);
	if (showScale) DrawScale(10,  p1[2],p2[2],   p1[0],p1[1],p1[2],   p1[0],p1[1],p2[2]);
	if (showScale) DrawScale(10,  p1[0],p2[0],   p1[0],p2[1],p2[2],   p2[0],p2[1],p2[2]);
	if (showScale) DrawScale(10,  p1[1],p2[1],   p2[0],p1[1],p2[2],   p2[0],p2[1],p2[2]);
	if (showScale) DrawScale(10,  p1[2],p2[2],   p2[0],p2[1],p1[2],   p2[0],p2[1],p2[2]);
	if (showScale) DrawScale(10,  p1[2],p2[2],   p2[0],p1[1],p1[2],   p2[0],p1[1],p2[2]);
	if (showScale) DrawScale(10,  p1[0],p2[0],   p1[0],p1[1],p2[2],   p2[0],p1[1],p2[2]);
	if (showScale) DrawScale(10,  p1[1],p2[1],   p1[0],p1[1],p2[2],   p1[0],p2[1],p2[2]);
	if (showScale) DrawScale(10,  p1[1],p2[1],   p2[0],p1[1],p1[2],   p2[0],p2[1],p1[2]);
	if (showScale) DrawScale(10,  p1[0],p2[0],   p1[0],p2[1],p1[2],   p2[0],p2[1],p1[2]);
	if (showScale) DrawScale(10,  p1[2],p2[2],   p1[0],p2[1],p1[2],   p1[0],p2[1],p2[2]);
	
	if (showGrid) {
		drawGrid();
	}
	
	glBegin(GL_LINES);
	glVertex3f(p1[0],p1[1],p1[2]);//
	glVertex3f(p2[0],p1[1],p1[2]);
	
	glVertex3f(p1[0],p1[1],p1[2]);//
	glVertex3f(p1[0],p2[1],p1[2]);
	
	glVertex3f(p1[0],p1[1],p1[2]);//
	glVertex3f(p1[0],p1[1],p2[2]);
	
	glVertex3f(p2[0],p2[1],p2[2]);//
	glVertex3f(p1[0],p2[1],p2[2]);
	
	glVertex3f(p2[0],p2[1],p2[2]);//
	glVertex3f(p2[0],p1[1],p2[2]);
	
	glVertex3f(p2[0],p2[1],p2[2]);//
	glVertex3f(p2[0],p2[1],p1[2]);
	
	glVertex3f(p2[0],p1[1],p1[2]); //
	glVertex3f(p2[0],p1[1],p2[2]);
	
	glVertex3f(p1[0],p1[1],p2[2]);//
	glVertex3f(p2[0],p1[1],p2[2]);
	
	glVertex3f(p1[0],p1[1],p2[2]);//
	glVertex3f(p1[0],p2[1],p2[2]);
	
	glVertex3f(p2[0],p1[1],p1[2]); //
	glVertex3f(p2[0],p2[1],p1[2]);
	
	glVertex3f(p1[0],p2[1],p1[2]); //
	glVertex3f(p2[0],p2[1],p1[2]);
	
	glVertex3f(p1[0],p2[1],p1[2]);
	glVertex3f(p1[0],p2[1],p2[2]);
	glEnd();
	
	
	DrawCube(p1[0],p1[1],p1[2]);
	DrawCube(p2[0],p2[1],p2[2]);
	
	DrawCube(p1[0],p2[1],p1[2]);
	DrawCube(p1[0],p1[1],p2[2]);
	
	DrawCube(p2[0],p1[1],p1[2]);
	DrawCube(p1[0],p2[1],p2[2]);
	
	DrawCube(p2[0],p2[1],p1[2]);
	DrawCube(p2[0],p1[1],p2[2]);
	
	fvlist->end();
}

void FVBoundBox::drawGrid()
{
	glBegin(GL_LINES);
		drawLines(10, p1[0], p1[1], p1[2], p2[0], p1[1], p1[2], p1[0], p1[1], p2[2] );
		drawLines(10, p1[0], p1[1], p1[2], p2[0], p1[1], p1[2], p1[0], p2[1], p1[2] );
		
		drawLines(10, p1[0], p1[1], p1[2], p1[0], p2[1], p1[2], p2[0], p1[1], p1[2] );
		drawLines(10, p1[0], p1[1], p1[2], p1[0], p2[1], p1[2], p1[0], p1[1], p2[2] );
		
		drawLines(10, p1[0], p1[1], p1[2], p1[0], p1[1], p2[2], p2[0], p1[1], p1[2] );
		drawLines(10, p1[0], p1[1], p1[2], p1[0], p1[1], p2[2], p1[0], p2[1], p1[2] );
		
		drawLines(10, p2[0], p2[1], p2[2], p1[0], p2[1], p2[2], p2[0], p2[1], p1[2] );
		drawLines(10, p2[0], p2[1], p2[2], p1[0], p2[1], p2[2], p2[0], p1[1], p2[2] );
		
		drawLines(10, p2[0], p2[1], p2[2], p2[0], p1[1], p2[2], p1[0], p2[1], p2[2] );
		drawLines(10, p2[0], p2[1], p2[2], p2[0], p1[1], p2[2], p2[0], p2[1], p1[2] );
		
		drawLines(10, p2[0], p2[1], p2[2], p2[0], p2[1], p1[2], p1[0], p2[1], p2[2] );
		drawLines(10, p2[0], p2[1], p2[2], p2[0], p2[1], p1[2], p2[0], p1[1], p2[2] );
	glEnd();
	
}

void FVBoundBox::drawLines(int n, double p1x , double p1y , double p1z , double p2x , double p2y , double p2z , double p3x , double p3y , double p3z )
{
	int i;
	double mp1[3],mp2[3], mp3[3] ,v[3];
	SET3v(mp1,p1x,p1y,p1z);
	SET3v(mp2,p2x,p2y,p2z);
	SET3v(mp3,p3x,p3y,p3z);
	SUB(v,mp3, mp1);
	MULT(v,v,1.0 / (double) n);
	
	
	for ( i = 0; i < n; i++) {
		/* printf("Line %lf, %lf, %lf - %lf, %lf, %lf\n",
			p1x + i * v[0], p1y + i * v[1], p1z + i * v[2],
			p2x + i * v[0], p2y + i * v[1], p2z + i * v[2] );  */
		glVertex3f( p1x + i * v[0], p1y + i * v[1], p1z + i * v[2] ); 
		glVertex3f( p2x + i * v[0], p2y + i * v[1], p2z + i * v[2] ); 
	}
	
}

void FVBoundBox::update( )
{
	paintGL();
	getCurrentViewer()->updateGL();
}

void FVBoundBox::setBBox( double _p1[], double _p2[] )
{
	SET(p1,_p1);
	SET(p2,_p2);
	dx = p2[0] - p1[0];
	dy = p2[1] - p1[1];
	dz = p2[2] - p1[2];
	ADD(c,p1,p2);
	MULT(c,c,0.5);
	//qDebug("c = %f,%f,%f",c[0],c[1],c[2]);
}

void FVBoundBox::DrawCube(float xPos, float yPos, float zPos)
{
	glPushMatrix();	
	glTranslatef(xPos,yPos,zPos);
	glScalef(boxSize*dx/40, boxSize*dy/40, boxSize*dz/40);
	glTranslatef(0.5, 0.5, 0.5);
	
	glBegin(GL_POLYGON);

	/*      This is the top face*/
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, -1.0f);
	glVertex3f(-1.0f, 0.0f, -1.0f);
	glVertex3f(-1.0f, 0.0f, 0.0f);

	/*      This is the front face*/
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);

	/*      This is the right face*/
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, -1.0f);
	glVertex3f(0.0f, 0.0f, -1.0f);

	/*      This is the left face*/
	glVertex3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-1.0f, 0.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);

	/*      This is the bottom face*/
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);

	/*      This is the back face*/
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-1.0f, 0.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(0.0f, -1.0f, -1.0f);
	
	glEnd();
	
	glScalef(1.0,1.0,1.0);
	glPopMatrix();
}


void FVBoundBox::DrawScale( int n, float vp, float vk, float x1, float y1, float z1, float x2, float y2, float z2 )
{
	QString s;
	float vx,vy,vz, dv;
	vx = x2 - x1;
	vy = y2 - y1;
	vz = z2 - z1;
	dv = vk - vp;
	
	float pp1[3],pp2[3],v1[3],v2[3],v[3];
	pp1[0] = x1;
	pp1[1] = y1;
	pp1[2] = z1;
	pp2[0] = x2;
	pp2[1] = y2;
	pp2[2] = z2;
	SUB(v1,pp1,c);
	SUB(v2,pp2,c);
	ADD(v,v1,v2);
	MULT(v,v,0.1);
	VERSOR(v,v);
	MULT(v,v,((vx+vy+vz)/(5*n)));
	
	for (float i = 1; i < n; i += 1.0) {
		getCurrentViewer()->fvRenderText(v[0] + x1 + vx / n * i,
		                               v[1] + y1 + vy / n * i,
		                               v[2] + z1 + vz / n * i, 
						s.sprintf("%.2g",vp + dv / n * i)
						);
		
		glBegin(GL_LINES);
		glVertex3f(v[0] + x1 + vx / n * i, v[1] + y1 + vy / n * i, v[2] + z1 + vz / n * i);
		glVertex3f(x1 + vx / n * i, y1 + vy / n * i, z1 + vz / n * i);
		glEnd();
	}
}

void FVBoundBox::readAttribs( )
{
	lineThickness = getAttr("Line Width")->toDouble();
	boxSize = getAttr("Box Size")->toDouble();
	if (getAttrValue(tr("Show Scale")) == "Yes")
		showScale = true;
	else
		showScale = false;
	if (getAttrValue(tr("Show Grid")) == "Yes")
		showGrid = true;
	else
		showGrid = false;
	
}

void FVBoundBox::slotApply( )
{
	update();
}

void FVBoundBox::setupAttributes( )
{
	Attr * a;
	a = am->addAttr( tr("Line Width"), QString("1.0"), "text" );
	a = am->addAttr( tr("Box Size"), QString("1.0"), "text" );
	a = am->addAttr( tr("Show Scale"), QString("No"), "boolean" );
	a = am->addAttr( tr("Show Grid"), QString("No"), "boolean" );
}





