#ifndef FVBOXIMPORTER_H
#define FVBOXIMPORTER_H
#include "fvobject.h"
#include <qdom.h>
#include <QVector>
#include <QPair>

class FVBoxImporter
{
public:
    bool importToBoxMgr(FVBoxMgr *bm, QString fileName);

private:
    QVector<QPair<QString, QString> > getAttributesFromString(QString text);
};

#endif // FVBOXIMPORTER_H
