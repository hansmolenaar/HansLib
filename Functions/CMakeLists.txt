project(Functions)

file(GLOB SOURCES CONFIGURE_DEPENDS "*.cpp")
add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE	
	${CMAKE_INCLUDE_DIR}/Functions
	${CMAKE_INCLUDE_DIR}/Utilities
	${CMAKE_INCLUDE_DIR}/MatrixVector
	${CMAKE_INCLUDE_DIR}/Interval
	${Boost_INCLUDE_DIRS}
)
