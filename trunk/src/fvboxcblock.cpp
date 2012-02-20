/***************************************************************************
 *   Copyright (C) 2008 by Robert Szmurlo   *
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
#include "fvboxcblock.h"
#include <fstream>
#include <QtDebug>
#include <QList>
#include "fvfielddraw.h"
#include <fvfieldslice.h>

FVBoxCBlock::FVBoxCBlock(FVBoxMgr * manager, int x, int y)
 : FVBoxGrid(manager,new Grid(),x,y)
{
	currentFName = "no filename";

	bSelfUpdate = false;

     field = 0;
     fvFieldInterface = 0;
     newField();

	setupAttributes();
	setupMenu();
}

void FVBoxCBlock::newField()
{
     SimpleField *sf = new SimpleField();
     sf->dim(3);

     if (field != 0) delete field;
     field = new Field();
     field->add(sf,0);

     if (fvFieldInterface != 0) delete fvFieldInterface;
     fvFieldInterface = new FVFieldInterface( &field );

}


FVBoxCBlock::~FVBoxCBlock()
{
}

#define JR 2
#define JI 1
bool FVBoxCBlock::loadFromFile(QString fname)
{
	Q_ASSERT( grid != 0 );
        bool bOK = true;
	currentFName = fname;
	delete grid;
	grid = new Grid();

     newField();

	double boxHeightRation = getAttrValue(tr("Block height ratio")).toDouble();
	QString drawComponent = getAttrValue(tr("Vector factor"));
	int j_comp;
	j_comp = 0;
	if (drawComponent == tr("Jr")) j_comp = JR;
	if (drawComponent == tr("Ji")) j_comp = JI;

	grid->addSubdomain(1);
	grid->addBoundary(1);
        Node *n;
	Elem *e;
	std::ifstream file( fname.toAscii().data() );
	
        file.ignore( 256, '>');
	int number;
	file >> number;

        if (number <= 0) bOK = false;
        int i = 0;	
	while( i++ < number){
		double p[3];
		double p1[3];
		double x[3];
		double y[3];
		double z[3];
		double v[3];
		double l,jr,ji;
		file >> p[0] >> p[1] >> p[2];
		file >> x[0] >> x[1] >> x[2];
		file >> y[0] >> y[1] >> y[2];
		file >> l >> jr >> ji;

                if (l == 0) bOK = false;

		//wyznacz z z iloczynu wektorowego i dlugosci ll
		CROSS(z,x,y);
		VERSOR(z,z);
		if (j_comp == JR) {
			if (jr == 0) printf("Warning! Jr == 0 in cblock nr=%d in file %s.\n", i, fname.toAscii().data());
			MULT(v,z,jr);
		} else if (j_comp == JI) {
			if (ji == 0) printf("Warning! Ji == 0 in cblock nr=%d in file %s.\n", i, fname.toAscii().data());
			MULT(v,z,ji);
		} else {
			if (l == 0) printf("Warning! l == 0 in cblock nr=%d in file %s.\n", i, fname.toAscii().data());
			MULT(v,z,l);
		}
		MULT(z,z,l);

		// wymnoz przez dodatkowy wspolczynnik aby poszerzyc pojedynczy
		// prostokat
		MULT(z,z,boxHeightRation);

		// przesun srodek cblocka o 0.5 z
		MULT(p1,z,-0.5);
		ADD(p,p,p1);
		
		// dodaj 8 wierzcholkow
		n = grid->addNewNode(p); n->addBoundary(1);

		ADD(p1,p,x);
		n = grid->addNewNode(p1); n->addBoundary(1);

		ADD(p1,p,y);
		n = grid->addNewNode(p1); n->addBoundary(1);

		ADD(p1,p,x);
		ADD(p1,p1,y);
		n = grid->addNewNode(p1); n->addBoundary(1);


		ADD(p1,p,z);
		n = grid->addNewNode(p1); n->addBoundary(1);

		ADD(p1,p,x);
		ADD(p1,p1,z);
		n = grid->addNewNode(p1); n->addBoundary(1);

		ADD(p1,p,y);
		ADD(p1,p1,z);
		n = grid->addNewNode(p1); n->addBoundary(1);

		ADD(p1,p,x);
		ADD(p1,p1,y);
		ADD(p1,p1,z);
		n = grid->addNewNode(p1); n->addBoundary(1);


		// uwaga! puste w srodku!
		//cout << "i="<< i << "p(" << px << " " << py << " " << pz << endl;
                e = grid->addNewElemT4n3D(1,(i-1)*8+1,(i-1)*8+4,(i-1)*8+2,(i-1)*8+6); // P1, P2, P4, P6
                e = grid->addNewElemT4n3D(1,(i-1)*8+1,(i-1)*8+6,(i-1)*8+5,(i-1)*8+7); // P1, P5, P6, P7
                e = grid->addNewElemT4n3D(1,(i-1)*8+1,(i-1)*8+7,(i-1)*8+3,(i-1)*8+4); // P1, P7, P3, P4
                e = grid->addNewElemT4n3D(1,(i-1)*8+4,(i-1)*8+6,(i-1)*8+7,(i-1)*8+8); // P4, P6, P7, P8
                e = grid->addNewElemT4n3D(1,(i-1)*8+1,(i-1)*8+6,(i-1)*8+7,(i-1)*8+4); // P1, P6, P7, P4

                // ten sam wektor Z dla wszystkich pięciu elementów CBlocka.
                for (int j=0; j<5; j++)
			for (int k = 0; k < 3; k++)
                                field->val( k, (i-1) * 5 + j, v[k] );
	}
	file.close();

        return bOK;
}

void FVBoxCBlock::setupAttributes()
{
	Attr * a;

	am->clear();
	a = am->addAttr( tr("Type"), tr("Grid"), QString("text") );
	a->setEditable(false);
	a = am->addAttr( tr("Filename"), tr(""), QString("text") );
	a->setEditable(false);

	a = am->addSection( tr("CBlock options") );
	a = am->addAttr( tr("Block height ratio"), QString("1.0"), QString("text") );
	a = am->addAttr( tr("Vector factor"), QString(tr("l (depth)")), QString("textlist") );
	QStringList lst;
	lst.append(tr("l (depth)"));
	lst.append(tr("Jr"));
	lst.append(tr("Ji"));
	a->setList( lst );

}

void FVBoxCBlock::update()
{
	if (bSelfUpdate != true) {
		bSelfUpdate = true;	
		FVBoxGrid::update();
		loadFromFile( currentFName );
	
		QList<QString> cmd;
		cmd.append("update");
		manager->sendMessage(cmd,this,true);

		bSelfUpdate = false;
	}
}

void FVBoxCBlock::setupMenu()
{
    FVBoxGrid::setupMenu();

    contextMenuObj->addSeparator();
    contextMenuObj->addAction(tr("&Draw Vectors"), this, SLOT( slotDrawVectors() ) );
    contextMenuObj->addSeparator();
    contextMenuObj->addAction(tr("&Add Slice"), this, SLOT( slotAddSlice() ) );
    contextMenuObj->addSeparator();
    contextMenuObj->addAction(tr("&Reload"), this, SLOT( slotReload() ) );

}

void FVBoxCBlock::slotAddSlice()
{
        FVFieldSlice * fs = new FVFieldSlice(manager);
        addChild( fs );
        fs->slice();
        fs->update();
}

void FVBoxCBlock::slotDrawVectors()
{	
	FVFieldDraw * fd = new FVFieldDraw(manager, this );
	fd->setAttrValue( QString("View Style"), QString("vectors") );
	addChild( fd );
	fd->update();
}

void FVBoxCBlock::slotReload()
{    
    update();
}

FVInterface * FVBoxCBlock::getInterface(QString interfaceName)
{
	if (interfaceName == QString("FVFieldInterface"))
		return fvFieldInterface;

	return FVBoxGrid::getInterface( interfaceName );
}

