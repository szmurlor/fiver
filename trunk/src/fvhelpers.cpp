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

void FVHelpers::getBBox(dolfin::Mesh* m, double minP[3], double maxP[3])
{
        unsigned int in;
        minP[0]=minP[1]=minP[2]=1e20;
        maxP[0]=maxP[1]=maxP[2]=-1e20;

        for (in=0; in < m->num_vertices(); in++) {
                dolfin::Point p(m->coordinates()[3*in],m->coordinates()[3*in+1],m->coordinates()[3*in+2]);
                if (p.coordinates()[0] < minP[0]) minP[0] = p.coordinates()[0];
                if (p.coordinates()[1] < minP[1]) minP[1] = p.coordinates()[1];
                if (p.coordinates()[2] < minP[2]) minP[2] = p.coordinates()[2];
                if (p.coordinates()[0] > maxP[0]) maxP[0] = p.coordinates()[0];
                if (p.coordinates()[1] > maxP[1]) maxP[1] = p.coordinates()[1];
                if (p.coordinates()[2] > maxP[2]) maxP[2] = p.coordinates()[2];
        }
}

void FVHelpers::getCenter( dolfin::Mesh* m, double P[3] )
{
    unsigned int ie,ic;
    double cc[3];
    cc[0] = cc[1] = cc[2] = 0;

    for (ie = 0; ie < m->num_vertices(); ie++) {
        for (ic=0; ic < m->geometry().dim(); ic++)
            cc[ic] += m->coordinates()[m->geometry().dim()*ie+ic];
    }
    P[0] = cc[0] / (double) m->num_vertices();
    P[1] = cc[1] / (double) m->num_vertices();
    P[2] = cc[2] / (double) m->num_vertices();
}
