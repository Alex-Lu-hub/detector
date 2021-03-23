#include <benchmark/benchmark.h>

#include <detector.h>

static void BM_detector(benchmark::State& state) {
    struct detector_t *detector = detector_init("/home/lw/model_deploy/config.yaml");
    int64_t test[6][100] = {1};
    detector_submit(detector, test, 6);

    for (auto _ : state)
        detector_submit(detector, test, 6);
}

BENCHMARK(BM_detector);

BENCHMARK_MAIN();