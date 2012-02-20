#include "fvharmonicfieldbox.h"
#include <main.h>
#include <math.h>
#include <QMessageBox>
#include <fvopener.h>
#include <fvopenertextfield.h>
#include <fvhelpers.h>
#include <fvfielddraw.h>
#include <fvfieldslice.h>
#include <fvanimationtimesteps.h>
#include <QDir>

FVHarmonicFieldBox::FVHarmonicFieldBox(FVBoxMgr * manager, FVObject * , int x, int y)
 : FVObject(manager, x,y)
{
    sType = tr("Harmonic Field");
    sName = tr("Dynamiczne pole harmoniczne");
    cColor = fvsettings.value( classType() + "_DefaultColor", FV_DEFAULT_BOX_COLOR_FIELD ).value<QColor>();

    setupAttributes();
    setupMenu();

    rRect.setWidth( 90 );

    setAttrValue("Name", sName);
    setAttrValue("Type", sType );

    fieldReal = 0;
    fieldImag = 0;
    t = 0;
    w = 1;

    // creating internally updated field from imaginary and real components
    field = new Field();
    SimpleField *sf = new SimpleField();
    field->add(sf);
    fvFieldInterface = new FVFieldInterface( &field );
}

void FVHarmonicFieldBox::setupMenu( )
{
    contextMenuObj->clear();

    contextMenuObj->addAction(tr("Load &real values"), this, SLOT( slotLoadRealValues() ));
    contextMenuObj->addAction(tr("Load &imaginary values"), this, SLOT( slotLoadImaginaryValues() ));
    contextMenuObj->addSeparator();


    contextMenuObj->addAction(tr("Draw &Vectors"), this, SLOT( slotDrawVectors() ) );
    contextMenuObj->addAction(tr("Draw &Colormap"), this, SLOT( slotDrawColormap() ) );
    contextMenuObj->addSeparator();

    contextMenuObj->addAction(tr("&Slice field"), this, SLOT( slotSlice() ) );
    contextMenuObj->addAction(tr("&Animate"), this, SLOT( slotAnimate() ) );
    contextMenuObj->addSeparator();

    contextMenuObj->addAction(tr("De&lete"),(QWidget*) manager, SLOT(slotDelete()) );
}

void FVHarmonicFieldBox::setupAttributes( )
{
    Attr * a;
    a = am->addAttr( tr("Type"), tr("FVHarmonicField"), QString("text") );
    a->setEditable(false);
    a = am->addAttr( tr("Name"), tr(""), QString("text") );
    a->setEditable(false);

    a = am->addAttr( tr("Real values"), tr("(use context menu to select)"), QString("text") );
    a->setEditable(false);
    a = am->addAttr( tr("Imaginary values"), tr("(use context menu to select)"), QString("text") );
    a->setEditable(false);

    am->addSection("Time parameters:");
    a = am->addAttr( tr("t [s]"), QString("0.0"), QString("text") );
    a->setEditable(true);
    a = am->addAttr( tr("frequency [Hz]"), QString("50.0"), QString("text") );
    a->setEditable(true);
}

FVInterface * FVHarmonicFieldBox::getInterface( QString interfaceName )
{
    if (interfaceName == QString("FVFieldInterface"))
        return fvFieldInterface;

    return parentInterface(interfaceName);
}

// this is to update view in OpenGL
void FVHarmonicFieldBox::update( )
{
    t = getAttrValue("t [s]").toDouble();
    w = 2 * M_PI * getAttrValue("frequency [Hz]").toDouble();

    //////////////////////
    recomputeField();
    //////////////////////
}


void FVHarmonicFieldBox::updateAttributes( )
{
    FVObject::update();
    manager->sendMessage(QString("update"), this, true );
}

bool FVHarmonicFieldBox::message( const QList< QString > & argv )
{
    if (argv.size() > 0) {
        if (argv[0] == QString("setTime") ) {
            double tv = argv[1].toDouble();
            setAttrValue("t [s]", QString::number(tv));

            /////////////////
            recomputeField();
            /////////////////

            return true;
        }
    }

    return false;
}

bool FVHarmonicFieldBox::recomputeField()
{
    qDebug() << "FVHarmonicFieldBox::recomputeField";
    if ((fieldReal == 0) && (fieldImag == 0)) {
        qDebug() << tr("Please use context menu to read real and imaginary fields first.");
        return false;
    }

    int n = fieldReal != 0 ? fieldReal->size() : fieldImag->size();
    int dim = fieldReal != 0 ? fieldReal->dim() : fieldImag->dim();

    if (fieldReal != 0 && fieldImag != 0 && n != fieldImag->size()) {
        qDebug() <<  tr("Number of values in real and imaginary fields does not match (size(re) = %1, size(im) = %1)").arg(n).arg(fieldImag->size());
        return false;
    }
    if (fieldReal != 0 && fieldImag != 0 && dim  != fieldImag->dim()) {
        qDebug() <<  tr("Dimensions of values of real and imaginary fields does not match (dim(re) = %1, dim(im) = %1)").arg(dim).arg(fieldImag->dim());
        return false;
    }

    // resize the time domain field
    field->size(n);
    field->setAttr("name", "Automatically computed harmonic field.");
    field->dim(dim);
    // quickly allocate memory
    field->val( 0, n - 1, 0.0f );

    for (int i = 0; i < n; i++) {
        for (int d = 0; d < dim; d++) {
            double re = fieldReal == 0 ? 0 : fieldReal->val(d,i);
            double im = fieldImag == 0 ? 0 : fieldImag->val(d,i);

            double Vm = sqrt(re*re+im*im);
            double phi = atan2(im,re);

            double v = Vm * sin(w*t+phi);

            field->val(d,i,v);
        }
    }

    return true;
}

void FVHarmonicFieldBox::setT(double t)
{
    this->t = t;

    recomputeField();
}

double FVHarmonicFieldBox::getT()
{
    return this->t;
}

void FVHarmonicFieldBox::setW(double w)
{
    this->w = w;

    recomputeField();
}

double FVHarmonicFieldBox::getW()
{
    return this->w;
}



void FVHarmonicFieldBox::slotLoadRealValues()
{
    QStringList files;
    QString fname;
    QString selectedFilter;

    QMap< QString, FVOpener* > filters;
    filters[tr("1 - Text field file (*.*)")] =  new FVOpenerTextField();

    //////////////////////////////////////////////////////
    // Show Dialog to Open File
    files = FVHelpers::openFiles(filters, selectedFilter);
    ///////////////////////////////////////////////////////


    foreach (fname, files) {
        FVOpener *opener = filters[selectedFilter];
        if (opener != 0) {
            FVObject* box = opener->open(manager, fname, 0);
            if (box != 0) {
                FVFieldInterface * fvint = (FVFieldInterface *) ((FVObject *) box)->getInterface("FVFieldInterface");
                if (fvint != 0) {
                    fieldReal = fvint->getField();
                }
                // jstar wstawil
                QString name= fname;
                QString reg( "^.*" );
                reg.append(QDir::separator());
                name.remove( QRegExp(reg) );
                name.append( " : real field " );
                box->setType( name);
                // dotad wstawil
                // tak bylo box->setType("Real Field");
                box->setName(fname);

                manager->addCon(this, box, tr(""), tr(""));
                manager->autoArrangeChildren(this);
            } else {
                QMessageBox::warning(manager,"Loading text field", tr("I have encountered an error processing text field from file: %1. See diagnostic messages to verify the problem.").arg(fname));
            }
        } else {
            QMessageBox::warning(manager,"Loading text field", tr("You must selected proper filter to point the expected file format."));
        }
    }

    /////////////////////////////////////////
    if ((fieldReal != 0) || (fieldImag != 0))
        recomputeField();
    /////////////////////////////////////////
}

void FVHarmonicFieldBox::slotLoadImaginaryValues()
{
    QStringList files;
    QString fname;
    QString selectedFilter;

    QMap< QString, FVOpener* > filters;
    filters[tr("1 - Text field file (*.*)")] =  new FVOpenerTextField();

    //////////////////////////////////////////////////////
    // Show Dialog to Open File
    files = FVHelpers::openFiles(filters, selectedFilter);
    ///////////////////////////////////////////////////////


    foreach (fname, files) {
        FVOpener *opener = filters[selectedFilter];
        if (opener != 0) {
            FVObject* box = opener->open(manager, fname, 0);
            if (box != 0) {
                FVFieldInterface * fvint = (FVFieldInterface *) ((FVObject *) box)->getInterface("FVFieldInterface");
                if (fvint != 0) {
                    fieldImag = fvint->getField();
                }
                // jstar wstawil
                QString name= fname;
                QString reg( "^.*" );
                reg.append(QDir::separator());
                name.remove( QRegExp(reg) );
                name.append( " : imag field " );
                box->setType( name);
                // dotad wstawil
                // bylo box->setType("Imag Field");
                box->setName(fname);

                manager->addCon(this, box, tr(""), tr(""));
                manager->autoArrangeChildren(this);
            } else {
                QMessageBox::warning(manager,"Loading text field", tr("I have encountered an error processing text field from file: %1. See diagnostic messages to verify the problem.").arg(fname));
            }
        } else {
            QMessageBox::warning(manager,"Loading text field", tr("You must selected proper filter to point the expected file format."));
        }
    }
    /////////////////////////////////////////
    if ((fieldReal != 0) || (fieldImag != 0))
        recomputeField();
    /////////////////////////////////////////
}

void FVHarmonicFieldBox::slotDrawColormap()
{
    if ( (fieldImag == 0) && (fieldReal == 0) )
        QMessageBox::warning(manager, tr("Draw vectors"), tr("Please use context menu and read real and imaginary fields from files first."));
    else {
        FVFieldDraw * fd = new FVFieldDraw( manager, this );
        addChild(fd);
        fd->update();
    }
}

void FVHarmonicFieldBox::slotDrawVectors()
{
    if ( (fieldImag == 0) && (fieldReal == 0) )
        QMessageBox::warning(manager, tr("Draw vectors"), tr("Please use context menu and read real and imaginary fields from files first."));
    else {
        FVFieldDraw * fd = new FVFieldDraw(manager, this );
        fd->setAttrValue( QString("View Style"), QString("vectors") );
        addChild( fd );
        fd->update();
        qDebug("FVFieldDraw created");
    }
}

void FVHarmonicFieldBox::slotSlice( )
{
    if( fieldReal != 0 || fieldImag != 0 ) {
        FVFieldSlice * fs = new FVFieldSlice(manager);
        addChild( fs );
        fs->slice();
        fs->update();
} else {
        QMessageBox::warning(manager, tr("Slice"), tr("Please use context menu and read real or imaginary fields from files first."));
    }
}

void FVHarmonicFieldBox::slotAnimate( )
{
    FVAnimationTimeSteps * fa = new FVAnimationTimeSteps(manager, childSuggestedX(), childSuggestedY());
    fa->addParent(this);
    fa->initAttributes();
    fa->update();
}

