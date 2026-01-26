project(EigenValueTest)

file(GLOB SOURCES CONFIGURE_DEPENDS "*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_INCLUDE_DIR}/Functions
	${CMAKE_INCLUDE_DIR}/NonLinearSolver
	${CMAKE_INCLUDE_DIR}/EigenValue
	${CMAKE_INCLUDE_DIR}/MatrixVector
	${CMAKE_INCLUDE_DIR}/Utilities
	${CMAKE_INCLUDE_DIR}/KelvinRepr
)

target_link_libraries(${PROJECT_NAME} EigenValue NonLinearSolver Functions KelvinRepr MatrixVector Utilities ${GTestLibs})
gtest_discover_tests(${PROJECT_NAME})
