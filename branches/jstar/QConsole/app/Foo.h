#ifndef FOO_H
#define FOO_H
#include <string>
#include <QString>
#include "Bar.h"
class Foo
{
private:
    Bar* bar;
public:
    Foo(){ bar = new Bar(); }
    ~Foo(){}
    void hello(); // says hello
    int add(int, int);
    int length(std::string);
    Bar* getBar(QString);
};

#endif // FOO_H
