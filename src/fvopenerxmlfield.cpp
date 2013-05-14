#include "fvopenerxmlfield.h"

#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <dolfin/function/Function.h>
#include <dolfin/function/FunctionSpace.h>
#include <TetraScalar.h>
#include <dolfin/mesh/Mesh.h>
#include <dolfin/fem/FiniteElement.h>
#include <dolfin/fem/DofMap.h>

FVOpenerXmlField::FVOpenerXmlField(QObject *parent) :
    FVOpener(parent)
{
}

FVOpenerXmlField::~FVOpenerXmlField()
{
}

QString FVOpenerXmlField::reload( FVObject * fvo )
{
//    QFile file(fname);
//    if (file.open(QFile::ReadOnly) == true) {
//        QTextStream ts(&file);
//        int n = 0;
//        int dim = 0;

//        // read from file the header
//        ts >> n >> dim;

//        qDebug() << "The field in file has n = " << n << " values";
//        qDebug() << "Each value has dim = " << dim << " components";

//        if (n > 0) {
//            if ((dim >= 1) && (dim <= 3)) {
//                // create field and add at once
//                // a new simple field
//                FVFieldInterface * fvInterface = (FVFieldInterface*) fvo->getInterface(QString("FVFieldInterface"));
//                if (fvInterface != 0) {
//                    Field * f = fvInterface->getField();
//                    /////////////////////////////
//                    loadData(n, dim, ts, f);
//                    /////////////////////////////
//                } else
//                    QMessageBox::information(0, tr("Reloading error"), tr("The object provided to reload the text field does not implement interface FVFieldInterface"));
//            }
//        }

//    }

    return QString::null;
}

FVObject* FVOpenerXmlField::open( FVBoxMgr * bm, QString fname, int type )
{
    FVBoxFieldXml * box = 0;

    // bm = bm;
    this->fname = fname;
    type = type;

    qDebug() << "Opening Xml Field: " << fname;



    dolfin::Mesh* mesh = new dolfin::Mesh();
    tetrascalar0* el0 = new tetrascalar0();
    dolfin::FiniteElement elem(boost::shared_ptr<ufc::finite_element>(el0) );
    tetrascalar_dof0* dof0 = new tetrascalar_dof0();
    dolfin::DofMap dof( boost::shared_ptr<ufc::dofmap>(dof0) , boost::shared_ptr<dolfin::Mesh>(mesh) );
    dolfin::FunctionSpace V(
                boost::shared_ptr<dolfin::Mesh>(mesh),
                boost::shared_ptr<dolfin::FiniteElement>(elem),
                boost::shared_ptr<dolfin::DofMap>(dof)
                );

//    dolfin::Function fun( boost::shared_ptr<dolfin::FunctionSpace>(V), fname.toStdString() );

    box = bm->addBoxFieldXml(NULL, fname, "nazwaFun");
    box->setOpener(this);


//    QFile file(fname);
//    if (file.open(QFile::ReadOnly) == true) {
//        QTextStream ts(&file);

//        int n = 0;
//        int dim = 0;

//        // read from file the header
//        ts >> n >> dim;

//        qDebug() << "The field in file has n = " << n << " values";
//        qDebug() << "Each value has dim = " << dim << " components";

//        if (n > 0) {
//            if ((dim >= 1) && (dim <= 3)) {
//                // create field and add at once
//                // a new simple field

//                Field * f = new Field();
//                SimpleField * sfield = new SimpleField();
//                f->add( sfield, 0);
//                f->setAttr("name", fname.toStdString());

//                /////////////////////////////
//                loadData(n, dim, ts, f);
//                /////////////////////////////

//                // jstar wstawil
//                QString name= fname;
//                QString reg( "^.*" );
//                reg.append(QDir::separator());
//                name.remove( QRegExp(reg) );
//                name.append( " : text field " );
//                box = bm->addBoxField(f, fname, name);
//                // dotad wstawil
//                // bylo box = bm->addBoxField(f, fname, tr("Text field"));
//                box->setOpener(this);
//            } else {
//                qWarning() << "The dimension of single value read from file " << dim << " should be 1,2 or 3 in (" << fname << ")";
//            }
//        } else {
//            qWarning() << "The number of field values should not be 0 in (" << fname << ")";
//        }
//    } else {
//        qWarning() << "Can not open file with text field: " << fname;
//    }

    return box;
}

void FVOpenerXmlField::loadData(int n, int dim, QTextStream & ts, dolfin::Function * f)
{
//    f->dim(dim);
//    f->size(n);
//    // quickly allocate memory
//    f->val( 0, n - 1, 0.0f );

//    for (int i = 0; i < n; i++) {
//        for (int j = 0; j < dim; j++) {
//            double v = 0;
//            if (ts.atEnd() == false) {
//                ts >> v;
//            } else {
//                qWarning() << "Unexpected end of file at value = " << i << " and dim = " << j;
//            }
//            f->val(j,i,v);
//        }
//    }
}
