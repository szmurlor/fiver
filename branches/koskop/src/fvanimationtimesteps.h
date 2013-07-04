#ifndef FVANIMATIONTIMESTEPS_H
#define FVANIMATIONTIMESTEPS_H

#include <QObject>
#include <QTimer>
#include <QAction>
#include <fvobject.h>
#include <fvrequirefield.h>

class FVAnimationTimeSteps : public FVObject
{
    Q_OBJECT
public:
    explicit FVAnimationTimeSteps(FVBoxMgr * manager, int x = 0, int y = 0);
    ~FVAnimationTimeSteps();
    virtual bool removeChildren() { return false; }

    virtual QString classType() { return QString("FVAnimation"); };
    virtual void initAttributes();
    virtual void updateAttributes();

public slots:
    void slotUpdate();
    void slotTimer();

    void slotPlay();
    void slotPause();
    void slotStop();

    void slotConnectChild();

protected:
    FVRequireField reqField;
    QTimer * tmr;

    virtual void setupAttributes();
    virtual void setupMenu();

private:
    QString lastPlaySpeed;
    QAction * acPlay;
    QAction * acPause;
    QAction * acStop;

};

#endif // FVANIMATIONTIMESTEPS_H
