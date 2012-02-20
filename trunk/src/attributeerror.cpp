#include <exception>
#include <QString>
#include "attributeerror.h"

AttributeError::AttributeError(QString wrongAttr) :
        std::exception() {
    this->wrongAttr = wrongAttr;
}

AttributeError::~AttributeError() throw() {}

const char* AttributeError::what() const throw() {
    QString s = "AttributError: " + wrongAttr;
    const char* cs = s.toUtf8().constData();
    return cs;
}
