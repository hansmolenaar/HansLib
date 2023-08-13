project(GeometryTest)

file(GLOB SOURCES CONFIGURE_DEPENDS "*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_INCLUDE_DIR}/Point
	${CMAKE_INCLUDE_DIR}/Geometry
	${CMAKE_INCLUDE_DIR}/Utilities
	${Boost_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} Point Utilities Geometry ${GTestLibs})
gtest_discover_tests(${PROJECT_NAME})