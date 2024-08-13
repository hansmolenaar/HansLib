project(ML)

file(GLOB SOURCES CONFIGURE_DEPENDS "*.cpp")
add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC	
	${CMAKE_INCLUDE_DIR}/Utilities
	${Boost_INCLUDE_DIRS}
)