project(IntervalTest)

file(GLOB SOURCES CONFIGURE_DEPENDS "*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_INCLUDE_DIR}/Interval
	${CMAKE_INCLUDE_DIR}/Utilities
)

target_link_libraries(${PROJECT_NAME} Interval Utilities ${GTestLibs})
gtest_discover_tests(${PROJECT_NAME})