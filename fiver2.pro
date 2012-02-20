#SUBDIRS += src/fiverinjector QConsole src/log src/boxmgr src/attr src/coremesh2  src/meshreader2 src
SUBDIRS += src/log src/boxmgr src/attr src/coremesh2  src/meshreader2 src src/meshjoiner
TEMPLATE = subdirs
CONFIG += \
#release \
          warn_on \
          qt \
          thread 
TARGET = fivermain
DEFINES *= NO_QCONSOLE










