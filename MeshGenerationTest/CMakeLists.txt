project(MeshGenerationTest)

file(GLOB SOURCES CONFIGURE_DEPENDS "*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_INCLUDE_DIR}/BoundingBox
	${CMAKE_INCLUDE_DIR}/Geometry
	${CMAKE_INCLUDE_DIR}/GeometryTest
	${CMAKE_INCLUDE_DIR}/Interval
	${CMAKE_INCLUDE_DIR}/MeshGeneration
	${CMAKE_INCLUDE_DIR}/Topology
	${CMAKE_INCLUDE_DIR}/Utilities
	${CMAKE_INCLUDE_DIR}/Tree
	${Boost_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME}  Point Utilities Tree MeshGeneration Geometry Graph ${GTestLibs})
gtest_discover_tests(${PROJECT_NAME})