#include <Interceptor.h>
#include "InterceptorInjector.h"
#include <boost/python.hpp>

using namespace boost;

InterceptorInjector::InterceptorInjector() {}
InterceptorInjector::~InterceptorInjector() {}

void InterceptorInjector::introduce(object main_namespace) {
    try
    {
        main_namespace["Interceptor"] =
                class_<Interceptor> ("Interceptor",init<int>())
                // Define its methods
                .def("grab", &Interceptor::grab);
    } catch (error_already_set& e) {
        PyErr_Print();
    }
}

void InterceptorInjector::inject(QString name,
                                 object main_namespace,
                                 void *obj) {
    // Save it so it will be accessible from Python
    main_namespace[name.toUtf8().constData()] = // Convert QString to c-string
            ptr((Interceptor*)obj);// I know... dynamic_cast is baaad:(
}
