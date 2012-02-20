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

#include <qwidgetplugin.h>

class QGLViewerPlugin : public QWidgetPlugin
{
public:
  QStringList keys() const;
  QWidget *create(const QString& key, QWidget *parent, const char *name);
  QString group(const QString& key) const;
  QIconSet iconSet(const QString&) const;
  QString includeFile(const QString& key) const;
  QString toolTip(const QString& key) const;
  QString whatsThis(const QString& key) const;
  bool isContainer(const QString&) const;
};
