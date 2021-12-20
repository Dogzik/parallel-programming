#include <benchmark/benchmark.h>
#include "src/cube_builder.h"
#include "src/parallel/bfs.h"
#include "src/sequential/bfs.h"

namespace {

template<typename BFSFun>
void benchmark_bfs(benchmark::State& state, BFSFun&& bfs) {
  int cube_edge = state.range(0);
  std::vector<int> dimension{cube_edge, cube_edge, cube_edge};
  auto edges = build_cube(dimension);
  for (auto _: state) {
    auto dist = bfs(0, edges);
    benchmark::ClobberMemory();
  }
}

void BM_seq_bfs(benchmark::State& state) {
  benchmark_bfs(state, seq::bfs);
}

void BM_par_bfs(benchmark::State& state) {
  benchmark_bfs(state, par::bfs);
}

}

BENCHMARK(BM_seq_bfs)
    ->DenseRange(200, 400, 100)
    ->ArgName("cube_edge")
    ->Iterations(5)
    ->Unit(benchmark::kSecond);

BENCHMARK(BM_par_bfs)
    ->DenseRange(200, 400, 100)
    ->ArgName("cube_edge")
    ->Iterations(5)
    ->Unit(benchmark::kSecond);
