project(KelvinReprTest)

file(GLOB SOURCES CONFIGURE_DEPENDS "*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_INCLUDE_DIR}/KelvinRepr
	${CMAKE_INCLUDE_DIR}/Utilities
	${CMAKE_INCLUDE_DIR}/MatrixVector
	${CMAKE_INCLUDE_DIR}/Functions
)

target_link_libraries(${PROJECT_NAME} KelvinRepr Functions MatrixVector NonLinearSolver Utilities ${GTestLibs})
gtest_discover_tests(${PROJECT_NAME})