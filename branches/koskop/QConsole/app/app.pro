# -------------------------------------------------
# Project created by QtCreator 2009-06-11T12:22:44
# -------------------------------------------------
TARGET = app
TEMPLATE = app
CONFIG += debug
SOURCES += main.cpp \
    Foo.cpp \
    FooInjector.cpp \
    Bar.cpp
HEADERS += ../Console/Console.h \
    ../Console/Executer.h \
    ../Console/Interpreter.h \
    ../Console/InterceptorInjector.h \
    ../Console/Interceptor.h \
    Foo.h \
    FooInjector.h \
    Bar.h
LIBS += -L../Console/ \
    -lConsole \
    -lboost_python \
    -lpython2.6 \
    -L../QConsole/ \
    -lQConsole
INCLUDEPATH += ../Console \
    ../QConsole \
    /usr/include/python2.6 \
    /usr/include/boost
