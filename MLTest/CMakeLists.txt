project(MLTest)

file(GLOB SOURCES CONFIGURE_DEPENDS "*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_INCLUDE_DIR}/ML
	${CMAKE_INCLUDE_DIR}/Functions
	${CMAKE_INCLUDE_DIR}/MatrixVector
)

target_link_libraries(${PROJECT_NAME} ML Utilities Functions MatrixVector NonLinearSolver ${GTestLibs})
gtest_discover_tests(${PROJECT_NAME})