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

#ifndef PIECE_H
#define PIECE_H

#include <qgl.h>
#include <math.h>
#include <iostream>

/*
 * Classe decrivant une piece
 */
class Piece {
private:
  int id;
  bool selected;
  bool fenetre;
  bool couleur;
  bool taille;
  bool forme;
  bool trou;
  double x_pos, y_pos;
  GLfloat amb_diff[3];
  GLfloat specular[3];
  GLfloat shininess;
  GLuint texture;

  void drawRectangle();
  void drawCylinder();
  void drawBoite();
  void drawBoule();

public:
  Piece(int, bool, bool, bool, bool, double, double);
  ~Piece() {}
  void paint(bool);
  void setSelected(bool s) { selected=s; }
  void setFenetre(bool f) { fenetre=f; }
  void setPos(double x, double y) { x_pos=x; y_pos=y; }
  void setTexture( GLuint t) { texture=t; }
  bool getTaille() { return taille; }
  bool getCouleur() { return couleur; }
  bool getForme() { return forme; }
  bool getTrou() { return trou; }
};

/*
 * Ensemble des pieces du jeu
 */
class SetOfPiece {
private:
  Piece* tab[16];
  int selected;

public:
  SetOfPiece();
  ~SetOfPiece() {
    for(int i=0; i<16; i++)
      delete(tab[i]);
  }
  void init();
  void paint(bool);
  void setSelected(int);
  void setTexture(GLuint);
  void placeSelectedPiece(int);
  Piece* getPiece() { return tab[selected]; }
};

#endif // PIECE_H
