#include "Foo.h"
#include "FooInjector.h"
#include <QString>
#include <boost/python.hpp>
#include <boost/python/to_python_converter.hpp>

using namespace boost;

FooInjector::FooInjector() {}
FooInjector::~FooInjector() {}

struct QString_to_python_str
{
    static PyObject* convert(QString const& s)
    {
        return boost::python::incref(boost::python::object(s.toUtf8().constData()).ptr());
    }
};

struct QString_from_python_str
{
    QString_from_python_str()
    {
        boost::python::converter::registry::push_back(
                &convertible,
                &construct,
                boost::python::type_id<QString>());
    }

    static void* convertible(PyObject* obj_ptr)
    {
        if (!PyString_Check(obj_ptr)) return 0;
        return obj_ptr;
    }

    static void construct(
            PyObject* obj_ptr,
            boost::python::converter::rvalue_from_python_stage1_data* data)
    {
        const char* value = PyString_AsString(obj_ptr);
        if (value == 0) boost::python::throw_error_already_set();
        void* storage = (
                (boost::python::converter::rvalue_from_python_storage<QString>*)
                data)->storage.bytes;
        new (storage) QString(value);
        data->convertible = storage;
    }
};

void FooInjector::introduce(object main_namespace) {
    try
    {
        main_namespace["Foo"] =
                class_<Foo> ("Foo")
                // Define its methods
                .def("hello", &Foo::hello)
                .def("add", &Foo::add)
                .def("length", &Foo::length)
                .def("getBar", &Foo::getBar,
                     return_value_policy<reference_existing_object>());

        main_namespace["Bar"] =
                class_<Bar> ("Bar")
                // Define its methods
                .def("setVal", &Bar::setVal)
                .def("getVal", &Bar::getVal);

        to_python_converter<QString, QString_to_python_str>();
        QString_from_python_str();

    } catch (error_already_set& e) {
        PyErr_Print();
    }
}

/*
/usr/include/boost/python/detail/invoke.hpp:88: error:
no match for call to
‘(const boost::python::detail::specify_a_return_value_policy_to_wrap_functions_returning<Bar*>) (Bar*)’
*/
void FooInjector::inject(QString name,  object main_namespace, void *obj) {
    // Save it so it will be accessible from Python
    main_namespace[name.toUtf8().constData()] = // Convert QString to c-string
            ptr((Foo*)obj); // I know... dynamic_cast is baaad:(
}
