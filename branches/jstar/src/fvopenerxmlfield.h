#ifndef FVOPENERXMLFIELD_H
#define FVOPENERXMLFIELD_H

#include <fvopener.h>
#include <fvboxcblock.h>
#include <fvboxfieldxml.h>
#include <fvboxfield.h>

class FVOpenerXmlField : public FVOpener
{
    Q_OBJECT
public:
    explicit FVOpenerXmlField(QObject *parent = 0);

    ~FVOpenerXmlField();

    virtual FVObject* open(FVBoxMgr * bm, QString fname, int type) ;
    virtual QString reload(FVObject * fvo);
    virtual void loadData(int n, int dim, QTextStream & ts, dolfin::Function * f);

signals:

public slots:

private:
    QString fname;
};

#endif // FVOPENERXMLFIELD_H
