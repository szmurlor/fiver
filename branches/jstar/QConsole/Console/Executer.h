#ifndef EXECUTER_H
#define EXECUTER_H

#include <QString>
#include <boost/python.hpp>
#include <Interceptor.h>

using namespace boost::python;

class Executer
{
private:
    object main_namespace;
public:
    Executer();
    ~Executer();
    void execute(QString);
    void inject_interceptor(Interceptor *, QString);
    object get_main_namespace();
};

#endif // EXECUTER_H
