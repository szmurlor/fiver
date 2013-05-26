#ifndef FVHELPERS_H
#define FVHELPERS_H

#include <QObject>
#include <QMap>
#include <fvopener.h>
#include <dolfin/mesh/Mesh.h>

class FVHelpers : public QObject
{
    Q_OBJECT
public:
    explicit FVHelpers(QObject *parent = 0);

    static QStringList openFiles(const QMap< QString, FVOpener* > filters, QString &selectedFilter);
    static void getBBox(dolfin::Mesh* m, double minP[3], double maxP[3]);
    static void getCenter( dolfin::Mesh* m, double P[3] );

signals:

public slots:

};

#endif // FVHELPERS_H
