#include "fvboxmanagerinjector.h"
#include <boost/shared_ptr.hpp>
#include <QString>
#include <fvboxmgr.h>
#include <fvobject.h>

using std::string;
using namespace boost::python;

FVBoxManagerInjector::FVBoxManagerInjector() {}

void FVBoxManagerInjector::introduce(object main_namespace) {
    try
    {
        FVObject* (FVBoxMgr::*find_by_name)(QString) = &FVBoxMgr::find;
        FVObject* (FVBoxMgr::*find_by_name_type)(QString, QString) = &FVBoxMgr::find;
        FVObject* (FVBoxMgr::*find_by_id)(int) = &FVBoxMgr::find;

        main_namespace["FVBoxMgr"] =
                class_<FVBoxMgr, boost::noncopyable> ("FVBoxMgr", no_init)
                // Define its methods
                .def("script", &FVBoxMgr::createScript)
                .def("find", find_by_name,
                     return_value_policy<reference_existing_object>());
    } catch (error_already_set& e) {
        PyErr_Print();
    }
}

void FVBoxManagerInjector::inject(QString name,  object main_namespace, void *obj) {
    // Save it so it will be accessible from Python
    main_namespace[name.toUtf8().constData()] = ptr((FVBoxMgr*)obj); // I know... dynamic_cast is baaad:(
}
