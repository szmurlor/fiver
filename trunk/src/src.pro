# Plik utworzony przez mened?era projekt�w kdevelopa
# ------------------------------------------- 
# Podkatalog wzgl?dem g?�wnego katalogu projektu: ./src
# Cel to program:  ../bin/fivermain

! include(../common.pri) {
	error( common.pri file does not exist!);
}

DEFINES *= NO_QCONSOLE

HEADERS += fviewer.h \
           fvlist.h \
           fivermain.h \
           fvopen.h \
           fvopener.h \
           fvopenerdiffpackgrid.h \
           fvopenerdiffpacksim.h \
           fvopenervtkfile.h \
           fvopenercblock.h \
           fvboxcblock.h \
           fvboxgrid.h \
           fvobject.h \
           main.h \
           fvgriddraw.h \
           setofint.h \
           fvgridbnd.h \
           fvboundbox.h \
           guiattributes.h \
           fiverapp.h \
           fvinteractionmodel.h \
           fvboxfield.h \
           fvboxsimulation.h \
           fvboxmgr.h \
           fvfielddraw.h \
           colormap.h \
           isolines.h \
           fvsettings.h \
           fvfieldline.h \
           fvfieldslice.h \
           fvinteractiontranslation.h \
           fvinteractionrotation.h \
           fvgridinterface.h \
           fvinterface.h \
           fvfieldinterface.h \
           fvrequirefield.h \
           fvrequiregrid.h \
           fvanimation.h \ 
           fvattribs.h \
           fvfieldlegend.h \
           Helpers.h \
    fvopenertextfield.h \
    fvhelpers.h \
    fvharmonicfieldbox.h \
    fvanimationtimesteps.h \
    fvopenerstlfile.h \
    fvboxsphere.h \
    fvboxcoil.h \
    fvopenercoil.h \
    fvboxstl.h \
    fvgridtostlextractor.h \
    fvmeshjoiner.h \
    fvopenerObjFile.h \
    xmlExporter.h \
    fvopenerFiverProj.h \
    fvBoxExporter.h \
    fvBoxImporter.h
SOURCES += main.cpp \
           fviewer.cpp \
           fvlist.cpp \
           fivermain.cpp \
           fvopen.cpp \
           fvopener.cpp \
           fvopenerdiffpackgrid.cpp \
           fvopenerdiffpacksim.cpp \
           fvopenervtkfile.cpp \
           fvopenercblock.cpp \
           fvboxgrid.cpp \
           fvobject.cpp \
           fvgriddraw.cpp \
           setofint.cpp \
           fvgridbnd.cpp \
           fvboundbox.cpp \
           guiattributes.cpp \
           fiverapp.cpp \
           fvinteractionmodel.cpp \
           fvboxfield.cpp \
           fvboxsimulation.cpp \
           fvboxmgr.cpp \
           fvfielddraw.cpp \
           colormap.cpp \
           isolines.cpp \
           fvsettings.cpp \
           fvfieldline.cpp \
           fvfieldslice.cpp \
           fvinteractiontranslation.cpp \
           fvinteractionrotation.cpp \
           fvgridinterface.cpp \
           fvinterface.cpp \
           fvfieldinterface.cpp \
           fvrequirefield.cpp \
           fvrequiregrid.cpp \
           fvanimation.cpp \
           fvattribs.cpp \
           fvfieldlegend.cpp \
           fvboxcblock.cpp \
           Helpers.cpp \
    fvopenertextfield.cpp \
    fvhelpers.cpp \
    fvharmonicfieldbox.cpp \
    fvanimationtimesteps.cpp \
    fvopenerstlfile.cpp \
    fvboxsphere.cpp \
    fvboxcoil.cpp \
    fvopenercoil.cpp \
    fvboxstl.cpp \
    fvgridtostlextractor.cpp \
    fvmeshjoiner.cpp \
    fvopenerObjFile.cpp \
    xmlExporter.cpp \
    fvopenerFiverProj.cpp \
    fvBoxExporter.cpp \
    fvBoxImporter.cpp
INCLUDEPATH += . ./boxmgr ./attr ./coremesh2/src ./meshreader2 ./meshjoiner ./fiverinjector ./tetgen \
        ../QConsole/QConsole \
        ../QConsole/Console \
        /usr/include/python2.6 \
        /usr/include/boost \

CONFIG(debug) {
    win32 { LIBS += -ldQGLViewer2 -lboxmgr -lfvattr -lcoremesh2 -lmeshreader2 -L./bin }
} else {
    win32 { LIBS += -lQGLViewer2 -lboxmgr -lfvattr -lcoremesh2 -lmeshreader2 -L./bin }
}
!win32 {
    LIBS += -l$$QGLVIEWER_LIB -lboxmgr -Lboxmgr -lfvattr -Lattr -lcoremesh2 -lmeshreader2 -ltrivlog -L./bin -lmeshjoiner -L./tetgen -ltet
#            -lQConsole -L../QConsole/QConsole \
#            -lConsole -L../QConsole/Console \
#            -lfiverinjector
}

QT += opengl xml
TEMPLATE = app
CONFIG += \
warn_on \
thread \
qt #\
#debug

TARGET = fivermain
DESTDIR=bin
INSTALLS += target 
target.path = $$INSTALL_ROOT/bin
# DEFINES += ENABLE_MAGELLAN
























