#include "fvobjectinjector.h"
#include <fvobject.h>

using namespace boost::python;
using std::string;

FVObjectInjector::FVObjectInjector() {}

void FVObjectInjector::introduce(object main_namespace) {
    try
    {
        main_namespace["FVObject"] =
                class_<FVObject, boost::noncopyable> ("FVObject",no_init)
                // Define its methods
                .def("set", &FVObject::setAttrValue)
                .def("get", &FVObject::getAttrValue)
                .def("update", &FVObject::updateAttributes)
                .def("__setitem__", &FVObject::setAttrValue)
                .def("__getitem__", &FVObject::getAttrValue)
                .def("slot", &FVObject::callSlot )
                .def("update", &FVObject::update);
    } catch (error_already_set& e) {
        PyErr_Print();
    }
}

void FVObjectInjector::inject(QString name,  object main_namespace, void *obj) {
    // Save it so it will be accessible from Python
    main_namespace[name.toUtf8().constData()] = ptr((FVObject*)obj); // I know... dynamic_cast is baaad:(
}
