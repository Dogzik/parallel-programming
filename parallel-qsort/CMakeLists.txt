cmake_minimum_required(VERSION 3.10)
project(parallel-algorithms)
set(CMAKE_CXX_STANDARD 17)

add_compile_options(-fcilkplus)

add_subdirectory(tests/lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ${CMAKE_SOURCE_DIR})

set(PARALLEL_SOURCES
        src/parallel/map.h
        src/parallel/scan.h
        src/parallel/sort.h
        src/parallel/filter.h)
set(SEQUENTIAL_SOURCES
        src/sequential/map.h
        src/sequential/scan.h
        src/sequential/sort.h
        src/sequential/filter.h tests/test_common.cpp)
add_executable(run-tests  src/raw_array.h ${PARALLEL_SOURCES} ${SEQUENTIAL_SOURCES} tests/sort_tests.cpp tests/utils_tests.cpp tests/test_common.h)
add_executable(tmp tests/tests.cpp)

target_link_libraries(run-tests pthread cilkrts gtest_main)
