cmake_minimum_required (VERSION 2.6)

project (xplat)

file(GLOB_RECURSE xplat_src ${PROJECT_SOURCE_DIR}/src/*)
file(GLOB_RECURSE xplat_tests ${PROJECT_SOURCE_DIR}/test/*.h)

add_custom_command (
	OUTPUT ${CMAKE_BINARY_DIR}/xplat_test.cpp
	COMMAND python ${PROJECT_SOURCE_DIR}/../cxxtest/bin/cxxtestgen --xunit-printer -o ${CMAKE_BINARY_DIR}/xplat_test.cpp ${xplat_tests}
)

find_package (Threads)
add_library (xplat ${xplat_src})
target_link_libraries (xplat ${CMAKE_THREAD_LIBS_INIT})

add_executable(xplat_test ${CMAKE_BINARY_DIR}/xplat_test.cpp)
target_link_libraries (xplat_test xplat)
