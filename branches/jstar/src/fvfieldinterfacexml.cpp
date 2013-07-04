#include "fvfieldinterfacexml.h"

FVFieldInterfaceXml::FVFieldInterfaceXml( dolfin::Function * fieldPtr)
{
        fp = fieldPtr;
}


FVFieldInterfaceXml::~FVFieldInterfaceXml()
{
}

dolfin::Function * FVFieldInterfaceXml::getField( )
{
        return fp;
}

