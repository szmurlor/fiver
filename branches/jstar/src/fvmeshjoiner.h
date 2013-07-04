#ifndef FVBOXMESHJOINER_H
#define FVBOXMESHJOINER_H

#include <fvobject.h>
#include <fvboxmgr.h>
#include <grid.h>
//#include <fvrequirefield.h>

#include <vector>
#include <meshjoiner.h>

class FVMeshJoiner : public FVObject
{
    Q_OBJECT
public:
    FVMeshJoiner(FVBoxMgr * manager, int x = 0, int y = 0);
    ~FVMeshJoiner();
    virtual bool removeChildren() { return false; }

    virtual QString classType() { return QString("FVMeshJoiner"); };

    MeshJoiner* mj;
    QString sFlags;

public slots:
    void slotJoin();
    void slotExternal();
    void slotHole();
    void slotDeleteChildren();

protected:
    bool existExternal;

    virtual void setupAttributes();
    virtual void updateAttributes();
    virtual void setupMenu();

private:

};

#endif // FVBOXMESHJOINER_H
