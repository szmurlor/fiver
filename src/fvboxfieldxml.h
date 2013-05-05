#ifndef FVBOXFIELDXML_H
#define FVBOXFIELDXML_H

#include <fvobject.h>
#include <fvboxmgr.h>
#include <dolfin/mesh/Mesh.h>
//#include <grid.h>
#include <dolfin/function/Function.h>
//#include <field.h>
#include <fvfieldinterfacexml.h>
//#include <fvfieldinterface.h>
#include <fvopener.h>

class FVBoxFieldXml : public FVObject
{
        Q_OBJECT

public:
        FVBoxFieldXml(FVBoxMgr * manager, dolfin::Function * f, int x = 0, int y = 0);
        virtual ~FVBoxFieldXml();

        virtual QString classType() { return QString("FVBoxFieldXml"); }
        virtual FVInterface * getInterface( QString interfaceName );

        virtual void update();
        virtual QString getName();

        void setOpener(FVOpener *opener) {
            this->fvOpener = opener;
        }
        FVOpener * getOpener() {
            return this->fvOpener;
        }

public slots:
        void slotDrawColormap();
        void slotDrawVectors();
        void slotAnimate();
        void slotSlice();
        void slotReload();

protected:
//        Field * field;
        dolfin::Function * field;
        FVFieldInterfaceXml * fvFieldInterfaceXml;
        FVOpener * fvOpener;

        virtual void setupAttributes();
        virtual void setupAttributesMinMax();
        virtual void setupMenu();
        virtual bool message( const QList<QString> &argv );

};

#endif
