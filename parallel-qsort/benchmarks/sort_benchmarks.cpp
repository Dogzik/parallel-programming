#include <random>
#include <benchmark/benchmark.h>
#include "src/raw_array.h"
#include "src/sequential/sort.h"
#include "src/parallel/sort.h"

namespace {

std::default_random_engine engine(2517);

template<bool parallel, typename S>
void benchmark_sort(benchmark::State &state, S&& sorter) {
  size_t size = state.range(0);
  raw_array<int> arr(size);
  std::uniform_int_distribution<int> distribution(-100'000, 100'000);
  for (size_t i = 0; i < size; ++i) {
    arr.emplace_at(i, distribution(engine));
  }
  for (auto _ : state) {
    state.PauseTiming();
    std::shuffle(arr.begin(), arr.end(), engine);
    state.ResumeTiming();
    if constexpr(parallel) {
      sorter(arr, state.range(1));
    } else {
      sorter(arr);
    }
  }
}

void BM_sequential_sort(benchmark::State &state) {
  benchmark_sort<false>(state, seq::sort<int>);
}

void BM_sort_without_filters(benchmark::State &state) {
  benchmark_sort<true>(state, par::sort_without_filters<int>);
}

void BM_sort_with_sequential_filter(benchmark::State &state) {
  benchmark_sort<true>(state, par::sort_with_sequential_filter<int>);
}

void BM_sort_with_parallel_filter(benchmark::State &state) {
  benchmark_sort<true>(state, par::sort_with_parallel_filter<int>);
}

}

BENCHMARK(BM_sequential_sort)
  ->RangeMultiplier(10)
  ->Range(1000, 10'000'000)
  ->ArgName("size")
  ->Unit(benchmark::kMillisecond);

BENCHMARK(BM_sort_without_filters)
  ->ArgsProduct({
    benchmark::CreateRange(1000, 10'000'000, 10),
    {10'000, 1'000'000, 3'000'000}
  })
  ->ArgNames({"size", "block_size"})
  ->Unit(benchmark::kMillisecond);

BENCHMARK(BM_sort_with_sequential_filter)
  ->ArgsProduct({
    benchmark::CreateRange(1000, 10'000'000, 10),
    {10'000, 1'000'000, 3'000'000}
  })
  ->ArgNames({"size", "block_size"})
  ->Unit(benchmark::kMillisecond);

BENCHMARK(BM_sort_with_parallel_filter)
  ->ArgsProduct({
    benchmark::CreateRange(1000, 10'000'000, 10),
    {10'000, 1'000'000, 3'000'000}
  })
  ->ArgNames({"size", "block_size"})
  ->Unit(benchmark::kMillisecond);
