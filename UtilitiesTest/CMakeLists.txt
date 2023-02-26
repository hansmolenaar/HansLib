project(UtilitiesTest)

file(GLOB SOURCES CONFIGURE_DEPENDS "*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

set(include_dirs ${CMAKE_INCLUDE_DIR}/Utilities )
message( "include_dirs" ${include_dirs})
target_include_directories(${PROJECT_NAME} PRIVATE ${include_dirs})

target_link_libraries(${PROJECT_NAME} Utilities ${GTestLibs})
gtest_discover_tests(${PROJECT_NAME})
