project(Tree)

file(GLOB SOURCES CONFIGURE_DEPENDS "*.cpp")
add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE	
	${CMAKE_INCLUDE_DIR}/Tree
	${CMAKE_INCLUDE_DIR}/Point
	${CMAKE_INCLUDE_DIR}/BoundingBox
	${CMAKE_INCLUDE_DIR}/Geometry
	${CMAKE_INCLUDE_DIR}/Utilities
	${CMAKE_INCLUDE_DIR}/Interval
	${Boost_INCLUDE_DIRS}
)