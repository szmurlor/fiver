
#Check for QGLViewer
isEmpty(QGLVIEWER_LIB){
    QGLVIEWER_LIB=qglviewer-qt4
} else {
    win32 { error("Unsupported variable QGLVIEWER_LIB") }
}
unix { system("echo QGLVIEWER_LIB=$$QGLVIEWER_LIB > common.pri") }

#Check for INSTALL_ROOT
isEmpty(INSTALL_ROOT){
   INSTALL_ROOT=/usr
} else {
    win32 { error("Unsupported variable INSTALL_ROOT") }
}
unix { system("echo INSTALL_ROOT=$$INSTALL_ROOT >> common.pri") }

#SUBDIRS += src/fiverinjector QConsole src/log src/boxmgr src/attr src/coremesh2  src/meshreader2 src
SUBDIRS += log boxmgr attr coremesh2  meshreader2 meshjoiner tetgen fiver
log.subdir=src/log
boxmgr.subdir=src/boxmgr
attr.subdir=src/attr
coremesh2.subdir=src/coremesh2
meshreader2.subdir=src/meshreader2
meshreader2.depends=coremesh2
tetgen.subdir=src/tetgen
meshjoiner.subdir=src/meshjoiner
meshjoiner.depends=tetgen
fiver.subdir=src
fiver.depends= meshreader2 meshjoiner boxmgr log attr
TEMPLATE = subdirs
CONFIG += \
#release \
          warn_on \
          qt \
          thread 
TARGET = fivermain
DEFINES *= NO_QCONSOLE
example.path=$$INSTALL_ROOT/share/fiver/examples
example.files=data/*.grid
INSTALLS += example

utils.target=fiver-utils
utils.commands= @echo Building $$utils.target && cd src/utils/fieldavg && make && cd ../fieldenergy && make && cd ../fieldinfo && make && cd ../gridscale && make && cd ../vol2surf && make

utils-clean.target=fiver-utils-clean
utils-clean.commands= @echo Building $$utils.target && cd src/utils/fieldavg && make clean && cd ../fieldenergy && make clean && cd ../fieldinfo && make clean && cd ../gridscale && make clean && cd ../vol2surf && make clean

QMAKE_EXTRA_TARGETS += utils utils-clean

QMAKE_DISTCLEAN += common.pri

