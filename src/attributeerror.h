#ifndef ATTRIBUTEERROR_H
#define ATTRIBUTEERROR_H
#include <exception>
#include <QString>

using namespace std;

class AttributeError : public std::exception
{
private:
    QString wrongAttr;
public:
    AttributeError(QString);
    ~AttributeError() throw();
    const char* what() const throw();
};

#endif // ATTRIBUTEERROR_H
