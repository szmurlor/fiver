#ifndef FVFIELDINTERFACEXML_H
#define FVFIELDINTERFACEXML_H

#include <fvinterface.h>
//#include <field.h>
#include <dolfin/function/Function.h>

class FVFieldInterfaceXml : public FVInterface{
public:
        FVFieldInterfaceXml( dolfin::Function * fieldPtr);
        virtual ~FVFieldInterfaceXml();

        virtual dolfin::Function * getField();

private:
        dolfin::Function * fp;
};

#endif
