cmake_minimum_required(VERSION 3.22.1)
project ( Vector VERSION 1.0 LANGUAGES CXX )

# Using TestManager Library
# [1] Compile the TestManagere first into a lib.
set(TEST_LIB "TM")
add_library( ${TEST_LIB} STATIC ${CMAKE_CURRENT_SOURCE_DIR}/tm/test_manager.cpp )
target_include_directories( ${TEST_LIB} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tm )
set_target_properties( ${TEST_LIB} PROPERTIES CXX_STANDARD 17 )

# [2] Setup the executable that will run the tests.
set ( TEST_DRIVER "run_tests")
add_executable( ${TEST_DRIVER} main.cpp iterator_tests.cpp)
set_target_properties( ${TEST_DRIVER} PROPERTIES CXX_STANDARD 17 )
# [3] Link tests compiled sources with the TestManager lib.
target_link_libraries( ${TEST_DRIVER} PRIVATE ${TEST_LIB} )
