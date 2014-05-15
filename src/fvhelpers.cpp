#include "fvhelpers.h"
#include <QFileDialog>
#include <main.h>
#include <math.h>

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


void
FVHelpers::normVec(double * w1, double * w2, double * w3, double wynik[3])
{
    double a1, a2, a3, b1, b2, b3, nomin, t;

    a1 = (w1)[0] - (w2)[0];
    a2 = (w1)[1] - (w2)[1];
    a3 = (w1)[2] - (w2)[2];

    b1 = (w1)[0] - (w3)[0];
    b2 = (w1)[1] - (w3)[1];
    b3 = (w1)[2] - (w3)[2];

    double A, B, C, D;
    A = a2 * b3 - a3 * b2;
    B = a3 * b1 - a1 * b3;
    C = a1 * b2 - a2 * b1;
    // from belonging of Node 1 to surface
    D = -(A * (w1)[0] + B * (w1)[1] + C * (w1)[2]);
    nomin = A*A + B*B + C*C;

    wynik[0] = A/sqrt(nomin);
    wynik[1] = B/sqrt(nomin);
    wynik[2] = C/sqrt(nomin);
}

double
FVHelpers::normalny4p(double * w1, double * w2, double * w3, double * w4,
           double wynik[3])
{
    double a1, a2, a3, b1, b2, b3, nomin, t;

    a1 = (w1)[0] - (w2)[0];
    a2 = (w1)[1] - (w2)[1];
    a3 = (w1)[2] - (w2)[2];

    b1 = (w1)[0] - (w3)[0];
    b2 = (w1)[1] - (w3)[1];
    b3 = (w1)[2] - (w3)[2];

    double A, B, C, D;
    A = a2 * b3 - a3 * b2;
    B = a3 * b1 - a1 * b3;
    C = a1 * b2 - a2 * b1;
    // from belonging of Node 1 to surface
    D = -(A * (w1)[0] + B * (w1)[1] + C * (w1)[2]);
    nomin = A*A + B*B + C*C;
    // sprawdzam wezly po kolei:
    t = -(A * (w4)[0] + B * (w4)[1] + C * (w4)[2] + D) / nomin;

    if (t >= 0) {
        wynik[0] = A/sqrt(nomin);
        wynik[1] = B/sqrt(nomin);
        wynik[2] = C/sqrt(nomin);
    } else {
        wynik[0] = -A/sqrt(nomin);
        wynik[1] = -B/sqrt(nomin);
        wynik[2] = -C/sqrt(nomin);
    }
        return(t);
}

