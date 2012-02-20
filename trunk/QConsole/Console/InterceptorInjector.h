#ifndef INTERCEPTORINJECTOR_H
#define INTERCEPTORINJECTOR_H

#include <QString>
#include <Injector.h>
#include <boost/python.hpp>

using namespace boost::python;

class InterceptorInjector : public Injector
{
public:
    InterceptorInjector();
    virtual ~InterceptorInjector();
    void introduce(object);
    void inject(QString, object, void*);
};
#endif // INTERCEPTORINJECTOR_H
