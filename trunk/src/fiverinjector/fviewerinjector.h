#ifndef FVIEWERINJECTOR_H
#define FVIEWERINJECTOR_H

#include <QString>
#include <Injector.h>
#include <boost/python.hpp>

using namespace boost::python;
using std::string;


class FViewerInjector : public Injector
{
public:
    FViewerInjector();

    void introduce(object);
    void inject(QString, object, void*);
};

#endif // FVIEWERINJECTOR_H
