#include <random>
#include <gtest/gtest.h>
#include "src/raw_array.h"
#include "src/sequential/sort.h"
#include "src/parallel/sort.h"

namespace {

std::default_random_engine engine(1337); // to make tests reproducible

raw_array<int32_t> gen_array(size_t size, int32_t min_elem = -100'000, int32_t max_elem = 100'000) {
  raw_array<int32_t> res(size);
  std::uniform_int_distribution<int32_t> distribution(min_elem, max_elem);
  for (size_t i = 0; i < size; ++i) {
    res.emplace_at(i, distribution(engine));
  }
  return res;
}

template<typename S>
void simple_test(S&& sorter) {
  raw_array<int32_t> arr{8, 1, -4, -8, 2, 5, 1, -7, 10};
  auto copy = arr;
  ASSERT_EQ(copy, arr);
  sorter(arr);
  std::sort(copy.begin(), copy.end());
  ASSERT_EQ(copy, arr);
}

template<typename S>
void stress_test(S&& sorter) {
  auto arr = gen_array(10'000'000);
  auto copy = arr;
  ASSERT_EQ(copy, arr);
  sorter(arr);
  std::sort(copy.begin(), copy.end());
  ASSERT_EQ(copy, arr);
}

}

TEST(simple, sequential_sort) {
  simple_test([](raw_array<int32_t>& arr) { seq::sort(arr); });
}

TEST(simple, parallel_sort_without_filters) {
  simple_test([](raw_array<int32_t>& arr) { par::sort_without_filters(arr, 2); });
}

TEST(simple, parallel_sort_with_sequential_filter) {
  simple_test([](raw_array<int32_t>& arr) { par::sort_with_sequential_filter(arr, 2); });
}

TEST(stress, sequential_sort) {
  stress_test([](raw_array<int32_t>& arr) { seq::sort(arr); });
}

TEST(stress, parallel_sort_without_filters) {
  stress_test([](raw_array<int32_t>& arr) { par::sort_without_filters(arr, 500); });
}

TEST(stress, parallel_sort_with_sequential_filter) {
  stress_test([](raw_array<int32_t>& arr) { par::sort_with_sequential_filter(arr, 500); });
}
