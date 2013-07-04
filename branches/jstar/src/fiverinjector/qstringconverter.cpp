#include "qstringconverter.h"
#include <QString>
#include <boost/python/to_python_converter.hpp>
#include <boost/python.hpp>

using namespace boost::python;

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

void QStringConverter::init() {
    to_python_converter<QString, QString_to_python_str>();
    QString_from_python_str();
}
