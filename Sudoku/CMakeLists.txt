project(Sudoku)

file(GLOB SOURCES CONFIGURE_DEPENDS "*.cpp")
add_library(${PROJECT_NAME} ${SOURCES} "Potentials.h")

target_include_directories(${PROJECT_NAME} PUBLIC	
	${CMAKE_INCLUDE_DIR}/Utilities
)