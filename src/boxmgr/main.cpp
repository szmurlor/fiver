#include "boxmgr.h"
#include "boxbox.h"

#include <qobject.h>
#include <qapplication.h>
#include <qpushbutton.h>




int main( int argc, char ** argv)
{
	QApplication qapp(argc,argv);
	BoxMgr *pb = new BoxMgr(0,"przycisk");

//	pb->setText("Napis.");
    pb->setGeometry(100,100,200,200);
	pb->show();

    BoxObj * b, *b1, *b2, *b3;
    b = pb->addBox("FVField","Obiekt 1", QColor(255,255,0) );
    b1 = pb->addBox("FVGrid", "Obiekt 2", QColor(0,255,0) );
    b2 = pb->addBox("FVSlice", "Obiekt 3", QColor(255,0,0) );
    b = pb->addBox("FVSlice", "Obiekt 3", QColor(0,0,255) );
    b3 = pb->addBox("FVSlice", "Obiekt 3", QColor(0,255,255) );
    b = pb->addBox("FVSlice", "Obiekt 3", QColor(0,0,255) );
    b = pb->addBox("FVSlice", "Obiekt 3", QColor(0,0,255) );
    
    pb->addCon(b1,b2,"napis","nazwa");
    pb->addCon(b,b2,"napis","nazwa");
    pb->addCon(b1,b,"napis","nazwa");
    
    BoxBox * bb;
    bb = new BoxBox(pb);
    bb->sType = "Utworzony w main";
    bb->sName = "A ja sie nazywam";
    bb->rRect.setRect(150,150,190,45);
    bb->cColor = QColor(156,156,0);
    pb->addObj( bb );
    
    pb->addCon( bb, b3,"napis","nazwa" );
    pb->addCon(b,b3,"napis","nazwa");

    qapp.setMainWidget(pb);

    return qapp.exec();
}
