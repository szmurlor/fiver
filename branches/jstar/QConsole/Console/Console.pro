# -------------------------------------------------
# Project created by QtCreator 2009-06-09T15:08:31
# -------------------------------------------------
TARGET = Console
CONFIG += console
CONFIG += debug
CONFIG -= app_bundle
TEMPLATE = lib
LIBS = -lpython2.6 \
    -lboost_python
SOURCES += Executer.cpp \
    Interpreter.cpp \
    Interceptor.cpp \
    Console.cpp \
    InterceptorInjector.cpp
HEADERS += Console.h \
    Executer.h \
    Interpreter.h \
    Interceptor.h \
    Injector.h \
    InterceptorInjector.h
INCLUDEPATH += /usr/include/python2.6
