#ifndef FVOPENERCOIL_H
#define FVOPENERCOIL_H

#include <fvopener.h>

/**
        @author jstar <jstar@iem.pw.edu.pl>
*/
class FVOpenerCoil : public FVOpener
{
public:
        FVOpenerCoil();
        FVOpenerCoil(QObject * parent);

    ~FVOpenerCoil();

    virtual FVObject* open( FVBoxMgr * bm, QString fname, int type ) ;
};
#endif // FVOPENERCOIL_H
