#ifndef FOOINJECTOR_H
#define FOOINJECTOR_H

#include <QString>
#include <Injector.h>
#include <boost/python.hpp>

using namespace boost::python;

class FooInjector : public Injector
{
public:
    FooInjector();
    virtual ~FooInjector();
    void introduce(object);
    void inject(QString, object, void*);
};
#endif // FOOINJECTOR_H
