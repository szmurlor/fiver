
! include(../../common.pri) {
	error( common.pri file does not exist!);
}

TEMPLATE = lib
TARGET = tet
DEPENDPATH += .
INCLUDEPATH += .
DESTDIR = ../bin

target.path = $$INSTALL_ROOT/lib
INSTALLS += target

QMAKE_CXXFLAGS += -DTETLIBRARY

HEADERS += tetgen.h
SOURCES += tetgen.cxx \
	   tetcall.cxx \
	   predicates.cxx
