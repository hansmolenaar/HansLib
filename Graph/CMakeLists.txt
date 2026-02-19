project(Graph)

file(GLOB SOURCES CONFIGURE_DEPENDS "*.cpp")
add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC	
	${CMAKE_INCLUDE_DIR}/Utilities
	${CMAKE_INCLUDE_DIR}/Permutation
	${CMAKE_INCLUDE_DIR}/EigenValue
	${CMAKE_INCLUDE_DIR}/MatrixVector
	${Boost_INCLUDE_DIRS}
        ${Eigen_INCLUDE_DIRS}
)
