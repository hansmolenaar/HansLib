project(UtilitiesTest)

file(GLOB SOURCES CONFIGURE_DEPENDS "*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

message("Boost_INCLUDE_DIRS " ${Boost_INCLUDE_DIRS})

target_include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_INCLUDE_DIR}/Utilities
	${Boost_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} Utilities ${GTestLibs})
gtest_discover_tests(${PROJECT_NAME})