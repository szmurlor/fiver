/***************************************************************************
 *   Copyright (C) 2011 by Robert Szmurlo   *
 *   robert@iem.pw.edu.pl   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "fvgridtostlextractor.h"
#include <main.h>
#include <QMessageBox>

FVGridToSTLExtractor::FVGridToSTLExtractor(FVBoxMgr * manager, int x = 10, int y = 10) :
    FVObject(manager,false)
{
    rRect.setRect(x,y,150,20);
    cColor = fvsettings.value( classType() + "_DefaultColor", FV_DEFAULT_BOX_COLOR_OPERATOR ).value<QColor>();

    setupAttributes();
    setupMenu();

    rRect.setWidth( 150 );
}


FVGridToSTLExtractor::~FVGridToSTLExtractor()
{
}

void FVGridToSTLExtractor::setupAttributes( )
{
        Attr * a;
        a = am->addAttr( tr("Type"), tr("Operator - Extract STL"), QString("text") );
        a->setEditable(false);
        a = am->addAttr( tr("Subdomains to extract"), tr("all"), QString("text") );
        a->setEditable(true);
}

void FVGridToSTLExtractor::setupMenu( )
{
    FVObject *fvo = manager->getParentOf(this);
}

void FVGridToSTLExtractor::updateAttributes( )
{
    QMessageBox::information(0,tr("Under construction"), QString("Tutaj bêdzie dzia³aæ w tym momencie wyci¹ganie STLa z grida."));
}

