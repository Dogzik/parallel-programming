#include <random>
#include <gtest/gtest.h>
#include "src/raw_array.h"
#include "src/sequential/sort.h"
#include "src/parallel/sort.h"
#include "tests/test_common.h"

namespace {

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

TEST(simple, parallel_sort_with_parallel_filter) {
  simple_test([](raw_array<int32_t>& arr) { par::sort_with_parallel_filter(arr, 2); });
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

TEST(stress, parallel_sort_with_parallel_filter) {
  stress_test([](raw_array<int32_t>& arr) { par::sort_with_parallel_filter(arr, 1000); });
}
