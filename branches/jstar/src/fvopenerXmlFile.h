#ifndef FVOPENERXMLFILE_H
#define FVOPENERXMLFILE_H

#include <fvopener.h>

class FVOpenerXmlFile : public FVOpener
{
    public:
        FVOpenerXmlFile();
        FVOpenerXmlFile(QObject * parent);

        ~FVOpenerXmlFile();

        virtual FVObject* open( FVBoxMgr * bm, QString fname, int type );
};

#endif // FVOPENERXMLFILE_H
