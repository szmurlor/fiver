#ifndef FVBOXMANAGERINJECTOR_H
#define FVBOXMANAGERINJECTOR_H

#include <QString>
#include <Injector.h>
#include <boost/python.hpp>

using namespace boost::python;
using std::string;

class FVBoxManagerInjector : public Injector
{
public:
    FVBoxManagerInjector();

    void introduce(object);
    void inject(QString, object, void*);
};

#endif // FVBOXMANAGERINJECTOR_H
