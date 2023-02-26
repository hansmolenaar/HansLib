project(NonLinearSolverTest)

file(GLOB SOURCES CONFIGURE_DEPENDS "*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_INCLUDE_DIR}/NonLinearSolver
	${CMAKE_INCLUDE_DIR}/Functions
)

target_link_libraries(${PROJECT_NAME} NonLinearSolver ${GTestLibs})
gtest_discover_tests(${PROJECT_NAME})