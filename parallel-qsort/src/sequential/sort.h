#pragma once

#include <random>
#include <cassert>
#include "src/raw_array.h"

namespace seq {

template<typename T, typename G>
size_t partition(raw_array<T>& arr, size_t left, size_t right, G& rand_gen) {
  assert(0 <= left && left < right && right < arr.size());
  std::uniform_int_distribution<size_t> p_idx_distribution(left, right);
  auto partitioner = arr[p_idx_distribution(rand_gen)];

  size_t i = left;
  size_t j = right;
  while (i <= j) {
    while (arr[i] < partitioner) {
      ++i;
    }
    while (arr[j] > partitioner) {
      --j;
    }
    if (i >= j) {
      break;
    }
    std::swap(arr[i], arr[j]);
    ++i;
    --j;
  }
  return j;
}

namespace detail {

template<typename T, typename G>
void do_sort(raw_array<T>& arr, size_t left, size_t right, G& rand_gen) {
  if (left >= right) {
    return;
  }
  size_t middle = partition(arr, left, right, rand_gen);
  do_sort(arr, left, middle, rand_gen);
  do_sort(arr, middle + 1, right, rand_gen);
}

}

template<typename T>
void sort(raw_array<T>& arr) {
  std::default_random_engine engine(time(nullptr));
  detail::do_sort(arr, 0, arr.size() - 1, engine);
}

}
