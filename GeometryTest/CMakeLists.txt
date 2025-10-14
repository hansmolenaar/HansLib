project(GeometryTest)

file(GLOB SOURCES CONFIGURE_DEPENDS "*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_INCLUDE_DIR}/BoundingBox
	${CMAKE_INCLUDE_DIR}/Point
	${CMAKE_INCLUDE_DIR}/Geometry
	${CMAKE_INCLUDE_DIR}/Interval
	${CMAKE_INCLUDE_DIR}/Utilities
	${CMAKE_INCLUDE_DIR}/Topology
	${CMAKE_INCLUDE_DIR}/Permutation
	${Boost_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} Point Utilities Geometry Topology Permutation ${GTestLibs})
gtest_discover_tests(${PROJECT_NAME})