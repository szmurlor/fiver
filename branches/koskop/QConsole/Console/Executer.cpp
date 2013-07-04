#include <boost/python.hpp>
#include <Python.h>

#include <Executer.h>
#include <Interceptor.h>

using namespace boost::python;

Executer::Executer () {
    //logger = log4cxx::Logger::getLogger("Console.Executer");
    // Initialize context
    Py_Initialize();
    //LOG4CXX_INFO(logger, "Python environment initialized");
    // Retrieve the main module
    object main_module((handle<>(borrowed(PyImport_AddModule("__main__")))));
    // Retrieve the main module's namespace
    main_namespace = main_module.attr("__dict__");
    //LOG4CXX_INFO(logger, "Borrowed reference to main module");
}

Executer::~Executer () {
    Py_Finalize();
    //LOG4CXX_INFO(logger, "Python environment destroyed");
}

void Executer::execute (QString command) {
    //LOG4CXX_INFO(logger, "Executing \"" << command << "\"");

    if(!command.endsWith('\n'))
        command.append("\n");

    PyRun_SimpleString(command.toUtf8().constData());
}

object Executer::get_main_namespace() {
    //LOG4CXX_DEBUG(logger, "Getting main namespace");
    return main_namespace;
}
