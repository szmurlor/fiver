######################################################################
# Automatically generated by qmake (2.01a) Wt 31. mar 21:39:11 2009
######################################################################

! include(../../common.pri) {
	error( common.pri file does not exist!);
}

TEMPLATE = lib
TARGET = meshreader2
DEPENDPATH += . tests/testsimu
INCLUDEPATH += ../coremesh2/src/ ../log
LIBS += -lcoremesh2 -L../bin -ltrivlog
DESTDIR = ../bin

target.path = $$INSTALL_ROOT/lib
INSTALLS += target

CONFIG = qt dll
QT += xml

!win32 { DEFINES += Q_OS_UNIX }
 win32:DEFINES += QT_DLL

# Input
HEADERS += diffreader.h diffsimu.h vtkfile.h fieldreader.h gridreader.h \
    stlfile.h objFile.h
SOURCES += diffreader.cpp \
           diffsimu.cpp \
           vtkfile.cpp \
           fieldreader.cpp \
           gridreader.cpp \
	   stlfile.cpp \
	   objFile.cpp
           #main.cpp \
           #tests/testsimu/main.cpp
