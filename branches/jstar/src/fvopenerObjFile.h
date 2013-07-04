#ifndef FVOPENEROBJFILE_H
#define FVOPENEROBJFILE_H

#include <fvopener.h>

class FVOpenerObjFile : public FVOpener
{
    public:
        FVOpenerObjFile();
        FVOpenerObjFile(QObject * parent);

        ~FVOpenerObjFile();

        virtual FVObject* open( FVBoxMgr * bm, QString fname, int type );
};

#endif // FVOPENEROBJFILE_H
