project(Utilities)

file(GLOB SOURCES CONFIGURE_DEPENDS "*.cpp")
add_library(${PROJECT_NAME} ${SOURCES})

set(include_dirs ${CMAKE_INCLUDE_DIR}/Utilities )
target_include_directories(${PROJECT_NAME} PRIVATE ${include_dirs})

