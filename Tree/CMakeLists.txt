project(Tree)

file(GLOB SOURCES CONFIGURE_DEPENDS "*.cpp")
add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE	
	${CMAKE_INCLUDE_DIR}/Tree
	${CMAKE_INCLUDE_DIR}/BoundingBox
	${CMAKE_INCLUDE_DIR}/Utilities
)