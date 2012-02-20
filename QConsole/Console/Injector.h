#ifndef INJECTOR_H
#define INJECTOR_H

#include <QString>
#include <boost/python.hpp>

using namespace boost::python;

class Injector
{
public:
    Injector() {}
    virtual void introduce(object) = 0;
    virtual void inject(QString, object, void*) = 0;
};

#endif // INJECTOR_H
