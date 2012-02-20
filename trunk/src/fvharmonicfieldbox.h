#ifndef FVHARMONICFIELDBOX_H
#define FVHARMONICFIELDBOX_H

#include <fvboxmgr.h>
#include <fvobject.h>
#include <fvfieldinterface.h>

class FVHarmonicFieldBox : public FVObject
{
    Q_OBJECT
public:
    explicit FVHarmonicFieldBox(FVBoxMgr * manager, FVObject * parent, int x = 0, int y = 0);

    virtual QString classType() { return QString("FVHarmonicField"); }
    virtual FVInterface * getInterface( QString interfaceName );

    virtual void update();

    void setT(double t);
    double getT();

    void setW(double w);
    double getW();

signals:

public slots:
    void slotLoadRealValues();
    void slotLoadImaginaryValues();
    void slotDrawVectors();
    void slotDrawColormap();
    void slotSlice();
    void slotAnimate();

protected:
    double t; // time [s]
    double w; // omega [Hz]

    // internally calculated field
    Field * field;
    FVFieldInterface * fvFieldInterface;

    Field * fieldReal;
    Field * fieldImag;

    virtual void setupAttributes();
    virtual void setupMenu();
    virtual bool message( const QList<QString> &argv );
    virtual void updateAttributes();

    bool recomputeField();
};

#endif // FVHARMONICFIELDBOX_H
