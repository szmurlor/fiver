# -------------------------------------------------
# Project created by QtCreator 2009-06-11T12:22:44
# -------------------------------------------------
TARGET = QConsole
CONFIG += debug
TEMPLATE = lib
SOURCES += \
    qconsole.cpp \
    qconsoletextedit.cpp
HEADERS += qconsole.h \
    qconsoletextedit.h \
    ../Console/Console.h \
    ../Console/Executer.h \
    ../Console/Interpreter.h \
    ../Console/InterceptorInjector.h \
    ../Console/Interceptor.h
FORMS += qconsole.ui
LIBS += -L ../Console/ -lConsole \
    -lboost_python \
    -lpython2.6
INCLUDEPATH += ../Console \
    /usr/include/python2.6 \
    /usr/include/boost
