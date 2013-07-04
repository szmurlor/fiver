#include <iostream>
#include <string>
#include <QString>
#include "Foo.h"
#include "Bar.h"

void Foo::hello() {
    std::cout << "Hello, nice to see you." << std::endl;
}

int Foo::add(int a, int b) {
    return a + b;
}

int Foo::length(std::string s) {
    std::cout << "Size of " << s << " is " << s.size() << std::endl;
    return s.size();
}

Bar* Foo::getBar(QString name) {
    if(name == "bar")
        return bar;
    else
        return new Bar();
}
