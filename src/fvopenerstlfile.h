#ifndef FVOPENERSTLFILE_H
#define FVOPENERSTLFILE_H

#include <fvopener.h>

class FVOpenerSTLFile : public FVOpener
{
    public:
        FVOpenerSTLFile();
        FVOpenerSTLFile(QObject * parent);

        ~FVOpenerSTLFile();

        virtual FVObject* open( FVBoxMgr * bm, QString fname, int type );
};

#endif // FVOPENERSTLFILE_H
