#pragma once

#include <cilk/cilk.h>
#include "src/sequential/filter.h"
#include "src/raw_array.h"
#include "src/sequential/sort.h"

namespace par {

namespace detail {

template<typename T, typename G>
void do_sort_without_filters(raw_array<T>& arr, size_t left, size_t right, size_t block_size, G& rand_gen) {
  if (left >= right) {
    return;
  }
  size_t middle = seq::partition(arr, left, right, rand_gen);
  if (right - left + 1 <= block_size) {
    seq::detail::do_sort(arr, left, right, rand_gen);
  } else {
    cilk_spawn do_sort_without_filters(arr, left, middle, block_size, rand_gen);
    do_sort_without_filters(arr, middle + 1, right, block_size, rand_gen);
    cilk_sync;
  }
}

template<typename T>
void copy(const raw_array<T>& src, size_t dst_start_pos, raw_array<T>& dst, size_t blocks_cnt) {
  assert(blocks_cnt >= 1);
  size_t block_size = (src.size() + blocks_cnt - 1) / blocks_cnt;
  #pragma cilk grainsize 1
  cilk_for(size_t i = 0; i < blocks_cnt; ++i) {
    size_t l = i * block_size;
    size_t r = std::min(src.size(), (i + 1) * block_size);
    for (size_t j = l; j < r; ++j) {
      dst[dst_start_pos + j] = src[j];
    }
  }
}

template<typename T, typename G>
void do_sort_with_sequential_filter(raw_array<T>& arr, size_t block_size, G& rand_gen) {
  if (arr.size() <= block_size) {
    if (arr.size() > 1) {
      seq::detail::do_sort(arr, 0, arr.size() - 1, rand_gen);
    }
    return;
  }
  size_t blocks_cnt = (arr.size() + block_size - 1) / block_size;
  std::uniform_int_distribution<uint32_t> p_idx_distribution(0, arr.size() - 1);
  T const& partitioner = arr[p_idx_distribution(rand_gen)];


  auto less = cilk_spawn seq::filter(arr, [partitioner](const T& x) { return x < partitioner; });
  auto equal = cilk_spawn seq::filter(arr, [partitioner](const T& x) { return x == partitioner; });
  auto greater = seq::filter(arr, [partitioner](const T& x) { return x > partitioner; });
  cilk_sync;

  cilk_spawn do_sort_with_sequential_filter(less, block_size, rand_gen);
  do_sort_with_sequential_filter(greater, block_size, rand_gen);
  cilk_sync;

  cilk_spawn copy(less, 0, arr, blocks_cnt);
  cilk_spawn copy(equal, less.size(), arr, blocks_cnt);
  copy(greater, less.size() + equal.size(), arr, blocks_cnt);
  cilk_sync;
}

}

template<typename T>
void sort_without_filters(raw_array<T>& arr, size_t block_size) {
  std::default_random_engine engine(time(nullptr));
  detail::do_sort_without_filters(arr, 0, arr.size() - 1, block_size, engine);
}

template<typename T>
void sort_with_sequential_filter(raw_array<T>& arr, size_t block_size) {
  std::default_random_engine engine(time(nullptr));
  detail::do_sort_with_sequential_filter(arr, block_size, engine);
}

}
