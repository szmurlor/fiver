TEMPLATE = lib
TARGET = dolfin
DEPENDPATH += .
INCLUDEPATH += .
DESTDIR = ../bin

target.path = $$INSTALL_ROOT/lib
INSTALLS += target

QMAKE_CXXFLAGS += -DDOLFINLIBRARY


OTHER_FILES += \
    walec88.xml

SOURCES += \
    mesh.cpp \
    common/utils.cpp \
    io/XMLMesh.cpp \
    io/XMLFile.cpp \
    io/pugixml.cpp \
    io/GenericFile.cpp \
    io/File.cpp \
    mesh/TriangleCell.cpp \
    mesh/TopologyComputation.cpp \
    mesh/TetrahedronCell.cpp \
    mesh/PointCell.cpp \
    mesh/Point.cpp \
    mesh/MeshTopology.cpp \
    mesh/MeshGeometry.cpp \
    mesh/MeshFunction.cpp \
    mesh/MeshEntity.cpp \
    mesh/MeshEditor.cpp \
    mesh/MeshData.cpp \
    mesh/MeshConnectivity.cpp \
    mesh/Mesh.cpp \
    mesh/IntervalCell.cpp \
    mesh/Facet.cpp \
    mesh/CellType.cpp

HEADERS += \
    common/utils.h \
    common/types.h \
    io/XMLMeshFunction.h \
    io/XMLMesh.h \
    io/XMLFile.h \
    io/pugixml.hpp \
    io/pugiconfig.hpp \
    io/GenericFile.h \
    io/File.h \
    mesh/Vertex.h \
    mesh/TriangleCell.h \
    mesh/TopologyComputation.h \
    mesh/TetrahedronCell.h \
    mesh/PointCell.h \
    mesh/Point.h \
    mesh/MeshTopology.h \
    mesh/MeshGeometry.h \
    mesh/MeshFunction.h \
    mesh/MeshEntityIterator.h \
    mesh/MeshEntity.h \
    mesh/MeshEditor.h \
    mesh/MeshData.h \
    mesh/MeshConnectivity.h \
    mesh/Mesh.h \
    mesh/IntervalCell.h \
    mesh/Facet.h \
    mesh/CellType.h \
    mesh/Cell.h








