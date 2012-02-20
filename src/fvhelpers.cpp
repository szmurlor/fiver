#include "fvhelpers.h"
#include <QFileDialog>
#include <main.h>

FVHelpers::FVHelpers(QObject *parent) :
    QObject(parent)
{
}


QStringList FVHelpers::openFiles(const QMap< QString, FVOpener* > filters, QString &selectedFilter)
{
    QStringList files;

    QString startDir(fvsettings.value("/RSoft/FViewer/RecentDir","~").toString());
    QString lastFilter(fvsettings.value("/RSoft/FViewer/LastFilter",filters.keys().first()).toString());

    QFileDialog od;
    od.setModal(true);
    od.setDirectory( startDir );
    od.selectNameFilter(lastFilter);
    od.setFilters( filters.keys() );

    if (od.exec()) {
        files = od.selectedFiles();
        selectedFilter = od.selectedFilter();

        QString fname;
        foreach (fname,files) {
            fvsettings.setValue("/RSoft/FViewer/RecentDir",fname.section('/',0,-2));
            fvsettings.setValue("/RSoft/FViewer/LastFilter",lastFilter);
            fvsettings.sync();
        }
    }

    return files;
}
