cmake_minimum_required(VERSION 3.10)

set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
set(BUILD_SHARED_LIBS OFF)
set (CMAKE_CXX_STANDARD 23)

set(BOOST_ROOT "/home/hans/repos/boost/boost_1_82_0")
#find_package( Boost REQUIRED COMPONENTS program_options regex )
set(Boost_USE_STATIC_LIBS OFF) 
#set(Boost_USE_MULTITHREADED ON)  
#set(Boost_USE_STATIC_RUNTIME OFF) 
#find_package(Boost REQUIRED COMPONENTS system) # header only libraries must not be added here
message("BOOST_ROOT " ${BOOST_ROOT})
find_package(Boost REQUIRED)
message("Boost_INCLUDE_DIRS " ${Boost_INCLUDE_DIRS})


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_INCLUDE_DIR ${CMAKE_SOURCE_DIR})

message("CMAKE_BINARY_DIR " ${CMAKE_BINARY_DIR})
message("CMAKE_SOURCE_DIR " ${CMAKE_SOURCE_DIR})
message("CMAKE_INCLUDE_DIR " ${CMAKE_INCLUDE_DIR})
message("CMAKE_RUNTIME_OUTPUT_DIRECTORY " ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

project(HansLib)

# Setup testing
set(GTestLibs GTest::GTest GTest::Main)
enable_testing()
find_package(GTest REQUIRED)
include(GoogleTest)

add_subdirectory(Utilities)
add_subdirectory(UtilitiesTest)

add_subdirectory(Point)
add_subdirectory(PointTest)

add_subdirectory(Interval)
add_subdirectory(IntervalTest)

add_subdirectory(BoundingBox)
add_subdirectory(BoundingBoxTest)

add_subdirectory(Geometry)
add_subdirectory(GeometryTest)

add_subdirectory(Graph)
add_subdirectory(GraphTest)

add_subdirectory(Groups)
add_subdirectory(GroupsTest)

add_subdirectory(Topology)
add_subdirectory(TopologyTest)

add_subdirectory(Tree)
add_subdirectory(TreeTest)

add_subdirectory(Permutation)
add_subdirectory(PermutationTest)

add_subdirectory(Functions)
add_subdirectory(FunctionsTest)

add_subdirectory(MatrixVector)
add_subdirectory(MatrixVectorTest)

add_subdirectory(NonLinearSolver)
add_subdirectory(NonLinearSolverTest)

add_subdirectory(KelvinRepr)
add_subdirectory(KelvinReprTest)

add_subdirectory(EigenValue)

add_subdirectory(Interpolation)
add_subdirectory(InterpolationTest)

add_subdirectory(Mesh)
add_subdirectory(MeshTest)

add_subdirectory(Optim)
add_subdirectory(OptimTest)

#add_subdirectory(MeshGeneration)
#add_subdirectory(MeshGenerationTest)

add_subdirectory(Sudoku)
add_subdirectory(SudokuTest)

add_subdirectory(ML)
add_subdirectory(MLTest)
