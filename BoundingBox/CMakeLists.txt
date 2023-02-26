project(BoundingBox)

file(GLOB SOURCES CONFIGURE_DEPENDS "*.cpp")
add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_INCLUDE_DIR}/BoundingBox)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_INCLUDE_DIR}/Interval)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_INCLUDE_DIR}/Point)