#include "fvopenertextfield.h"

#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDir>

FVOpenerTextField::FVOpenerTextField(QObject *parent) :
    FVOpener(parent)
{
}

FVOpenerTextField::~FVOpenerTextField()
{
}

QString FVOpenerTextField::reload( FVObject * fvo )
{
    QFile file(fname);
    if (file.open(QFile::ReadOnly) == true) {
        QTextStream ts(&file);
        int n = 0;
        int dim = 0;

        // read from file the header
        ts >> n >> dim;

        qDebug() << "The field in file has n = " << n << " values";
        qDebug() << "Each value has dim = " << dim << " components";

        if (n > 0) {
            if ((dim >= 1) && (dim <= 3)) {
                // create field and add at once
                // a new simple field
                FVFieldInterface * fvInterface = (FVFieldInterface*) fvo->getInterface(QString("FVFieldInterface"));
                if (fvInterface != 0) {
                    Field * f = fvInterface->getField();
                    /////////////////////////////
                    loadData(n, dim, ts, f);
                    /////////////////////////////
                } else
                    QMessageBox::information(0, tr("Reloading error"), tr("The object provided to reload the text field does not implement interface FVFieldInterface"));
            }
        }

    }

    return QString::null;
}

FVObject* FVOpenerTextField::open( FVBoxMgr * bm, QString fname, int type )
{
    FVBoxField * box = 0;

    // bm = bm;
    this->fname = fname;
    type = type;

    qDebug() << "Opening Text Field: " << fname;

    QFile file(fname);
    if (file.open(QFile::ReadOnly) == true) {
        QTextStream ts(&file);

        int n = 0;
        int dim = 0;

        // read from file the header
        ts >> n >> dim;

        qDebug() << "The field in file has n = " << n << " values";
        qDebug() << "Each value has dim = " << dim << " components";

        if (n > 0) {
            if ((dim >= 1) && (dim <= 3)) {
                // create field and add at once
                // a new simple field

                Field * f = new Field();
                SimpleField * sfield = new SimpleField();
                f->add( sfield, 0);
                f->setAttr("name", fname.toStdString());

                /////////////////////////////
                loadData(n, dim, ts, f);
                /////////////////////////////

                // jstar wstawil
                QString name= fname;
                QString reg( "^.*" );
                reg.append(QDir::separator());
                name.remove( QRegExp(reg) );
                name.append( " : text field " );
                box = bm->addBoxField(f, fname, name);
                // dotad wstawil
                // bylo box = bm->addBoxField(f, fname, tr("Text field"));
                box->setOpener(this);
            } else {
                qWarning() << "The dimension of single value read from file " << dim << " should be 1,2 or 3 in (" << fname << ")";
            }
        } else {
            qWarning() << "The number of field values should not be 0 in (" << fname << ")";
        }
    } else {
        qWarning() << "Can not open file with text field: " << fname;
    }

    return box;
}

void FVOpenerTextField::loadData(int n, int dim, QTextStream & ts, Field * f)
{
    f->dim(dim);
    f->size(n);
    // quickly allocate memory
    f->val( 0, n - 1, 0.0f );

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < dim; j++) {
            double v = 0;
            if (ts.atEnd() == false) {
                ts >> v;
            } else {
                qWarning() << "Unexpected end of file at value = " << i << " and dim = " << j;
            }
            f->val(j,i,v);
        }
    }
}
