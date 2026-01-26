project(Groups)
 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")

file(GLOB SOURCES CONFIGURE_DEPENDS "*.cpp")
add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE	
	${CMAKE_INCLUDE_DIR}/Geometry
	${CMAKE_INCLUDE_DIR}/Point
	${CMAKE_INCLUDE_DIR}/Utilities
	${CMAKE_INCLUDE_DIR}/Groups
	${CMAKE_INCLUDE_DIR}/Permutation
	${Boost_INCLUDE_DIRS}
)
