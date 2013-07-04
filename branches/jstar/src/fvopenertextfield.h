#ifndef FVOPENERTEXTFIELD_H
#define FVOPENERTEXTFIELD_H

#include <fvopener.h>
#include <fvboxcblock.h>
#include <fvboxfield.h>

class FVOpenerTextField : public FVOpener
{
    Q_OBJECT
public:
    explicit FVOpenerTextField(QObject *parent = 0);

    ~FVOpenerTextField();

    virtual FVObject* open(FVBoxMgr * bm, QString fname, int type) ;
    virtual QString reload(FVObject * fvo);
    virtual void loadData(int n, int dim, QTextStream & ts, Field * f);

signals:

public slots:

private:
    QString fname;
};

#endif // FVOPENERTEXTFIELD_H
