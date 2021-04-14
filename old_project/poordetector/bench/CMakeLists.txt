find_package(benchmark REQUIRED)

add_executable(poordetector_bench
    bench.cpp)

target_link_libraries(poordetector_bench
    benchmark::benchmark
    poordetector
)

add_custom_target(bench
    COMMAND ./poordetector
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} # build/bench/
)