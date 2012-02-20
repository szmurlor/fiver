#ifndef FVOPENERFIVERPROJ_H
#define FVOPENERFIVERPROJ_H

#include <fvopener.h>

class FVOpenerFiverProj : public FVOpener
{
public:
    FVOpenerFiverProj(bool saveMode = false);
    FVOpenerFiverProj(QObject * parent, bool saveMode = false);

    ~FVOpenerFiverProj();

    virtual FVObject* open( FVBoxMgr * bm, QString fname, int type );
private:
    bool saveMode;
};


#endif // FVOPENERFIVERPROJ_H
