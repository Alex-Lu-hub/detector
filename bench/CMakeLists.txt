find_package(benchmark REQUIRED)

add_executable(detector_bench
    bench.cpp)

target_link_libraries(detector_bench
    benchmark::benchmark
    detector
)

add_custom_target(bench
    COMMAND ./detector
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} # build/bench/
)