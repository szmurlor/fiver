#ifndef FVGRIDTOSTLEXTRACTOR_H
#define FVGRIDTOSTLEXTRACTOR_H

#include <fvobject.h>
#include <fvboxmgr.h>

class FVGridToSTLExtractor : public FVObject
{
    Q_OBJECT
public:
    FVGridToSTLExtractor(FVBoxMgr * manager, int x, int y);
    virtual ~FVGridToSTLExtractor();

    virtual QString classType() { return QString("FVGridToSTLExtractor"); }

public slots:

protected:
    virtual void setupAttributes();
    virtual void updateAttributes();
    virtual void setupMenu();

};

#endif // FVGRIDTOSTLEXTRACTOR_H
