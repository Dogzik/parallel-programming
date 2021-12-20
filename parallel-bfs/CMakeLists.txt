cmake_minimum_required(VERSION 3.10)
project(parallel-algorithms)
set(CMAKE_CXX_STANDARD 17)

add_compile_options(-fcilkplus -DUSE_CILK_PLUS_RUNTIME)

add_subdirectory(tests/lib)
add_subdirectory(benchmarks/lib)
include_directories(lib/pctl/include)
include_directories(lib/cmdline/include)
include_directories(lib/chunkedseq/include)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR} benchmarks/lib/include)
link_directories(${CMAKE_BINARY_DIR}/benchmarks/lib/src)

set(PARALLEL_SOURCES src/parallel/bfs.h)
set(SEQUENTIAL_SOURCE src/sequential/bfs.h)
add_executable(run-tests
        ${PARALLEL_SOURCES} ${SEQUENTIAL_SOURCES}
        tests/bfs_tests.cpp src/cube_builder.cpp src/cube_builder.h)
add_executable(run-benchmarks benchmarks/bfs_benchmarks.cpp src/cube_builder.cpp src/cube_builder.h)

target_link_libraries(run-tests pthread cilkrts gtest_main)
target_link_libraries(run-benchmarks pthread cilkrts benchmark_main)
