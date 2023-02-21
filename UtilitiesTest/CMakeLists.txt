project(UtilitiesTest)

file(GLOB SOURCES CONFIGURE_DEPENDS "*.cpp")

# Setup testing
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ../)

# Link test executable against gtest & gtest_main
target_link_libraries(${PROJECT_NAME} Utilities GTest::GTest GTest::Main)
