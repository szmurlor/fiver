TEMPLATE = lib
TARGET = dolfin
DEPENDPATH += .
INCLUDEPATH += . .. /usr/include/libxml2 /usr/include/boost
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
    mesh/CellType.cpp \
    function/FunctionSpace.cpp \
    function/Function.cpp \
    common/Variable.cpp \
    common/MPI.cpp \
    function/GenericFunction.cpp \
    function/Expression.cpp \
    mesh/ParallelData.cpp \
    mesh/MeshPartitioning.cpp \
    mesh/MeshDistributed.cpp \
    common/UniqueIdGenerator.cpp \
    common/timing.cpp \
    common/SubSystemsManager.cpp \
    mesh/MeshDomains.cpp \
    mesh/LocalMeshData.cpp \
    mesh/BoundaryMesh.cpp \
    mesh/BoundaryComputation.cpp \
    adaptivity/Extrapolation.cpp \
    fem/UFC.cpp \
    fem/FiniteElement.cpp \
    fem/DirichletBC.cpp \
    fem/BoundaryCondition.cpp \
    intersection/IntersectionOperator.cpp \
    la/LUSolver.cpp \
    la/KrylovSolver.cpp \
    la/DefaultFactory.cpp \
    log/Table.cpp \
    log/Progress.cpp \
    log/LogStream.cpp \
    log/LogManager.cpp \
    log/Logger.cpp \
    log/log.cpp \
    log/Event.cpp \
    parameter/Parameters.cpp \
    parameter/Parameter.cpp \
    parameter/GlobalParameters.cpp \
    fem/Form.cpp \
    fem/Equation.cpp \
    fem/DomainAssigner.cpp \
    fem/DofMap.cpp \
    function/Constant.cpp \
    mesh/SubDomain.cpp \
    mesh/FacetCell.cpp \
    mesh/Edge.cpp \
    la/LinearSolver.cpp \
    la/GenericMatrix.cpp \
    fem/DofMapBuilder.cpp \
    la/UmfpackLUSolver.cpp \
    la/uBLASVector.cpp \
    la/uBLASKrylovSolver.cpp \
    la/uBLASKrylovMatrix.cpp \
    la/SparsityPattern.cpp \
    la/uBLASILUPreconditioner.cpp \
    la/uBLASDummyPreconditioner.cpp \
    io/XMLFunctionData.cpp

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
    mesh/Cell.h \
    function/FunctionSpace.h \
    function/Function.h \
    common/Variable.h \
    common/NoDeleter.h \
    common/MPI.h \
    common/Hierarchical.h \
    common/constants.h \
    common/Array.h \
    function/GenericFunction.h \
    function/Expression.h \
    mesh/SubsetIterator.h \
    mesh/ParallelData.h \
    mesh/MeshPartitioning.h \
    mesh/MeshDistributed.h \
    mesh/LocalMeshValueCollection.h \
    common/UniqueIdGenerator.h \
    common/timing.h \
    common/Timer.h \
    common/SubSystemsManager.h \
    mesh/MeshValueCollection.h \
    mesh/MeshDomains.h \
    mesh/LocalMeshData.h \
    mesh/BoundaryMesh.h \
    mesh/BoundaryComputation.h \
    adaptivity/Extrapolation.h \
    fem/UFCMesh.h \
    fem/ufcexp.h \
    fem/UFCCell.h \
    fem/UFC.h \
    fem/GenericDofMap.h \
    fem/FiniteElement.h \
    fem/DirichletBC.h \
    fem/BoundaryCondition.h \
    intersection/IntersectionOperatorImplementation.h \
    intersection/IntersectionOperator.h \
    intersection/cgal_includes.h \
    la/LUSolver.h \
    la/LinearAlgebraFactory.h \
    la/KrylovSolver.h \
    la/GenericVector.h \
    la/GenericTensor.h \
    la/GenericSparsityPattern.h \
    la/GenericLUSolver.h \
    la/GenericLinearSolver.h \
    la/DefaultFactory.h \
    log/Table.h \
    log/Progress.h \
    log/LogStream.h \
    log/LogManager.h \
    log/LogLevel.h \
    log/Logger.h \
    log/log.h \
    log/Event.h \
    log/dolfin_log.h \
    parameter/Parameters.h \
    parameter/Parameter.h \
    parameter/GlobalParameters.h \
    parameter/dolfin_parameter.h \
    fem/Form.h \
    fem/Equation.h \
    fem/DomainAssigner.h \
    fem/DofMap.h \
    fem/BasisFunction.h \
    function/Constant.h \
    mesh/SubDomain.h \
    mesh/FacetCell.h \
    mesh/Edge.h \
    la/LinearSolver.h \
    la/GenericMatrix.h \
    common/Set.h \
    fem/DofMapBuilder.h \
    la/UmfpackLUSolver.h \
    la/uBLASVector.h \
    la/uBLASPreconditioner.h \
    la/uBLASMatrix.h \
    la/uBLASKrylovSolver.h \
    la/uBLASKrylovMatrix.h \
    la/uBLASFactory.h \
    la/ublas.h \
    la/SparsityPattern.h \
    la/uBLASSparseMatrix.h \
    la/uBLASILUPreconditioner.h \
    la/uBLASDummyPreconditioner.h \
    io/XMLFunctionData.h \
    finiteelementsclasses.h

!win32 {
    LIBS +=  -L/usr/lib -lboost_program_options -larmadillo -lxml2 -lboost_iostreams -lboost_system -lboost_filesystem
}







