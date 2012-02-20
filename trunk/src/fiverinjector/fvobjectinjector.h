#ifndef FVOBJECTINJECTOR_H
#define FVOBJECTINJECTOR_H
#include <QString>
#include <Injector.h>
#include <boost/python.hpp>

using namespace boost::python;
using std::string;

class FVObjectInjector : public Injector
{
public:
    FVObjectInjector();

    void introduce(object);
    void inject(QString, object, void*);
};

#endif // FVOBJECTINJECTOR_H
