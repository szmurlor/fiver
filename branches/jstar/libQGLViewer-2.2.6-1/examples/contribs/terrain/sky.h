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
//on ajoute du ciel!
#ifndef __SKYBOX_H__
#define __SKYBOX_H__

#include <QGLViewer/vec.h>
#include <qimage.h>

#define SKY_SIDES 6

#define SKY_FRONT  0
#define SKY_BACK   1
#define SKY_RIGHT  2
#define SKY_LEFT   3
#define SKY_TOP    4
#define SKY_BOTTOM 5


class SKY
{
private:
  QImage textures[SKY_SIDES];	//les cotes du ciel
  GLuint texIDs[SKY_SIDES];		//les IDs opengl pour chaque texture
  qglviewer::Vec vecMin, vecMax, vecCenter;
  bool iwantsky;

public:
  SKY() { iwantsky = false; }

  bool LoadTexture( int side, char* filename );

  void Render();

  //parametres du ciel
  void Set( float X, float Y, float Z, float size )
  {
    vecCenter.setValue( X, Y, Z );

    vecMin.setValue( 0, 0, 0 );
    vecMax.setValue( size, size, size );
  }

  void switchSky() { iwantsky = !iwantsky; }
  bool wantSky() { return iwantsky; }
};


#endif
