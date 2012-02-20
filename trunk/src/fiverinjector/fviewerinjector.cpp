#include "fviewerinjector.h"
#include <fviewer.h>

using namespace boost::python;
using std::string;


FViewerInjector::FViewerInjector()
{
}


void FViewerInjector::introduce(object main_namespace) {
    try
    {
        main_namespace["FViewer"] =
                class_<FViewer> ("FViewer")
                // Define its methods
                .def("repaint", &FViewer::repaint)
                .def("snapshot", &FViewer::snapshot)
                .def("rotx", &FViewer::rotx)
                .def("roty", &FViewer::roty)
                .def("rotz", &FViewer::rotz)
                .def("label", &FViewer::addLabel)
                .def("trans", &FViewer::trans);
    } catch (error_already_set& e) {
        PyErr_Print();
    }
}

void FViewerInjector::inject(QString name,  object main_namespace, void *obj) {
    // Save it so it will be accessible from Python
    main_namespace[name.toUtf8().constData()] = ptr((FViewer*)obj); // I know... dynamic_cast is baaad:(
}
