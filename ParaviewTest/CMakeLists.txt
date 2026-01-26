project(ParaviewTest)
 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")

file(GLOB SOURCES CONFIGURE_DEPENDS "*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_INCLUDE_DIR}/Utilities
	${CMAKE_INCLUDE_DIR}/Paraview
	${CMAKE_INCLUDE_DIR}/Point
	${Boost_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} Paraview Utilities  ${GTestLibs})
gtest_discover_tests(${PROJECT_NAME})
