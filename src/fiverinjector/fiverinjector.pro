# #####################################################################
# Automatically generated by qmake (2.01a) ?r. lip 29 15:59:05 2009
# #####################################################################
TEMPLATE = lib
DESTDIR = ../bin
QT += opengl \
    xml \
    qt

# Directories
INCLUDEPATH += .. \
    ../../QConsole/Console \
    /usr/include/python2.6 \
    /usr/include/boost \
    ../../libQGLViewer-2.2.6-1 \
    ../boxmgr \
    ../attr
LIBS += -lboost_python \
    -lpython2.6
HEADERS += fviewerinjector.h \
    fvboxmanagerinjector.h \
    qstringconverter.h \
    fvobjectinjector.h
SOURCES += fviewerinjector.cpp \
    fvboxmanagerinjector.cpp \
    qstringconverter.cpp \
    fvobjectinjector.cpp

