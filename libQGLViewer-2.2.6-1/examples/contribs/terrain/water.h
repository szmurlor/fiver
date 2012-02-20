/****************************************************************************

 Copyright (C) 2002-2006 Gilles Debunne (Gilles.Debunne@imag.fr)

This file is part of the QGLViewer library.
 Version 2.2.6-1, released on July 4, 2007.

 http://artis.imag.fr/Members/Gilles.Debunne/QGLViewer

 libQGLViewer is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 libQGLViewer is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with libQGLViewer; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*****************************************************************************/

// TP OpenGL: Joerg Liebelt, Serigne Sow
// generation d'eau: calcul vitesses,normales,reflections
#ifndef __WATER_H__
#define __WATER_H__

#include <QGLViewer/qglviewer.h>
#include <math.h>
#include <qcolor.h>

#define WATER_RESOLUTION 60
#define SQR( number )		( number*number )

using namespace qglviewer;		//classe VEC

class WATER
{
private:
  Vec vertArray[SQR( WATER_RESOLUTION )];		//champ de vertex
  Vec normalArray[SQR( WATER_RESOLUTION )];	//champ des normales de vertex
  float forceArray[SQR( WATER_RESOLUTION )];	//champ des forces influencant un vertex d'eau
  float velArray[SQR( WATER_RESOLUTION )];	//champ de vitesses des vagues
  int polyIndexArray[SQR( ( WATER_RESOLUTION-1 ) )*6];	//champ des indices des polygones (pour updates..)

  float worldSize;

  bool iwantwater;

  int numIndices;
  int numVertices;

  QColor color;

  unsigned int refmapID;

public:
  WATER()
  {
    SetColor(QColor("white"));
    iwantwater = false;
  }

  void Init(float myWorldSize, float scaleHeight);

  void Update(float delta);
  void CalcNormals();
  void Render();

  void switchWater() { iwantwater = !iwantwater; }

  bool wantWater() { return iwantwater; }

  void LoadReflectionMap(char* filename);

  void SetColor(const QColor& col) { color = col; }
};


#endif	//__WATER_H__
