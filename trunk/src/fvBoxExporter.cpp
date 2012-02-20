#include "fvBoxExporter.h"
#include "fvobject.h"
#include "xmlExporter.h"
#include <qdom.h>
#include <QVector>
#include <QPair>

using namespace std;

QString FVBoxExporter::setAttributesToString(AttrModel* model)
{
    QString str="";
    Attr* a;
    for( unsigned int i = 0; (a=model->getAttr(i)) != NULL; i++ )
    {
        str += a->name() + ";;;" + a->value() + "'''";
    }
    return str;
}

QString FVBoxExporter::setBoxInfoToString(FVObject *obj)
{
    QString str="";
    str += "X;;;" + QString::number( obj->rRect.x() ) + "'''";
    str += "Y;;;" + QString::number( obj->rRect.y() ) + "'''";
    str += "Width;;;" + QString::number( obj->rRect.width() ) + "'''";
    str += "Height;;;" + QString::number( obj->rRect.height() ) + "'''";
    return str;
}

void FVBoxExporter::exportFVBox(FVObject* obj, FVBoxMgr * bm, AttrModel* model, list< BoxObj* > &lst)
{
    exporter.beginElement( obj->classType() );

    exporter.addAttribute( "Attributes", setAttributesToString(model) );
    exporter.addAttribute( "BoxName", obj->getName() );
    exporter.addAttribute( "BoxInfo", setBoxInfoToString(obj) );

    for(list< BoxObj* >::iterator lit = lst.begin(); lit != lst.end() && lst.size(); lit++)
    {
        BoxObj* o = *lit;

        if( o->btType != BoxObj::TCon )
            continue;

        BoxCon* conn = (BoxCon*)o;

        if( conn->bsrc == obj )
        {
            list< BoxObj* >::iterator litTmp = lit++;
            lst.erase( litTmp );

            for( list< BoxObj* >::iterator it = lst.begin(); it != lst.end(); it++ )
            {
                if( *it == conn->bdst )
                {
                    if( lit == it )
                        lit++;
                    lst.erase(it);

                    model = ((FVObject*)conn->bdst)->getModel();
                    exportFVBox((FVObject*)conn->bdst, bm, model, lst);

                    break;
                }
            }
        }
    }

    exporter.endElement();

}

bool FVBoxExporter::exportFromBoxMgr(FVBoxMgr * bm, QString fileName)
{
    if( !exporter.beginDocument(fileName) )
        return false;

    list< BoxObj* > lst = bm->objs;

    while( lst.size() )
    {
        BoxObj * o = lst.front();
        FVObject* obj = (FVObject*)o;

        lst.erase( lst.begin() );

        AttrModel* model = obj->getModel();

        if( obj->btType != BoxObj::TBox || model->getAttr(0) == NULL )
            continue;

        exportFVBox(obj, bm, model, lst);
    }

    exporter.endDocument();
    return true;
}
