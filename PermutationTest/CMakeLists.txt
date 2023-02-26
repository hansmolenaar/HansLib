project(PermutationTest)

file(GLOB SOURCES CONFIGURE_DEPENDS "*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_INCLUDE_DIR}/Permutation
)

target_link_libraries(${PROJECT_NAME} Permutation Utilities ${GTestLibs})
gtest_discover_tests(${PROJECT_NAME})