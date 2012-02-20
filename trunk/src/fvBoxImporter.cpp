#include "fvBoxImporter.h"
#include "fvobject.h"
#include <qdom.h>
#include <QVector>
#include <QPair>
#include <QFile>

QVector< QPair<QString, QString> > FVBoxImporter::getAttributesFromString(QString text)
{
    QStringList lst = text.split("'''", QString::SkipEmptyParts);

    QVector< QPair<QString, QString> > out( lst.size() );

    for( int i=0; i < lst.size(); i++ )
    {
        if( lst.at(i).size() == 0 )
            continue;

        QStringList attr = lst.at(i).split(";;;", QString::SkipEmptyParts);

        if( attr.size() < 2 )
        {
            qDebug() << "Unable to parse from XML: " << lst.at(i) << "\n";
            continue;
        }

        QString val = attr.at(1);
        for( int j=2; j < attr.size(); j++ )
            val += attr.at(j);

        out[i] = QPair<QString, QString>( attr.at(0), val );
    }
    return out;
}

bool FVBoxImporter::importToBoxMgr(FVBoxMgr * bm, QString fileName)
{
    QFile file( fileName );
    if ( !file.open( QFile::ReadOnly ) )
    {
        qDebug() << "Unable to open " << fileName << "\n";
        return false;
    }

    QDomDocument domTree;
    if ( !domTree.setContent( &file ) )
    {
        file.close();
        qDebug() << "Parsing error for file " << fileName << "\n";
        return false;
    }
    file.close();

    QDomNode elem = domTree.documentElement();
    while( !elem.isNull() )
    {
        if( elem.isElement() )
        {
            QString classType = elem.nodeName();
            if( classType == "FVBoxGrid" )
            {
                //TODO jest tu problem bo nie wiem jak brać openery :/
            }
            else if( classType == "FVGridDraw" )
            {
                //TODO brakuje info rodzica i połączenia
            }
            else
            {
                qDebug() << "Unknown for project loader box object type: " << classType << "\n";
            }
        }
        elem = elem.nextSibling();
    }

    return true;
}
