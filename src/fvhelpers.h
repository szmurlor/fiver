#ifndef FVHELPERS_H
#define FVHELPERS_H

#include <QObject>
#include <QMap>
#include <fvopener.h>

class FVHelpers : public QObject
{
    Q_OBJECT
public:
    explicit FVHelpers(QObject *parent = 0);

    static QStringList openFiles(const QMap< QString, FVOpener* > filters, QString &selectedFilter);

signals:

public slots:

};

#endif // FVHELPERS_H
