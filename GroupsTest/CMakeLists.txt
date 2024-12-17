project(GroupsTest)

file(GLOB SOURCES CONFIGURE_DEPENDS "*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_INCLUDE_DIR}/Groups
	${CMAKE_INCLUDE_DIR}/Geometry
	${CMAKE_INCLUDE_DIR}/Utilities
	${CMAKE_INCLUDE_DIR}/Permutation
)

target_link_libraries(${PROJECT_NAME} Groups Utilities Geometry Point Permutation ${GTestLibs})
gtest_discover_tests(${PROJECT_NAME})