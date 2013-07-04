#ifndef FVBOXEXPORTER_H
#define FVBOXEXPORTER_H
#include <QVector>
#include <QPair>
#include "xmlExporter.h"
#include "fvobject.h"
#include <list>

class FVBoxExporter
{
public:
    bool exportFromBoxMgr(FVBoxMgr *bm, QString fileName);

private:
    QString setAttributesToString(AttrModel *model);
    QString setBoxInfoToString(FVObject *obj);
    void exportFVBox(FVObject *obj, FVBoxMgr *bm, AttrModel *model, std::list< BoxObj* > &lst);

    XmlExporter exporter;
};

#endif // FVBOXEXPORTER_H
