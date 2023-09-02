project(Optim)

file(GLOB SOURCES CONFIGURE_DEPENDS "*.cpp")
add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE	
	${CMAKE_INCLUDE_DIR}/Optim
	${CMAKE_INCLUDE_DIR}/Functions
	${CMAKE_INCLUDE_DIR}/Utilities
	${CMAKE_INCLUDE_DIR}/Interpolation
	${CMAKE_INCLUDE_DIR}/Interval
	${CMAKE_INCLUDE_DIR}/MatrixVector
	${Boost_INCLUDE_DIRS}
)