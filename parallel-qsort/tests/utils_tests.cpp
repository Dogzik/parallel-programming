#include <gtest/gtest.h>
#include <numeric>
#include <algorithm>
#include "tests/test_common.h"

#include "src/raw_array.h"
#include "src/sequential/map.h"
#include "src/sequential/scan.h"
#include "src/sequential/filter.h"
#include "src/parallel/map.h"
#include "src/parallel/scan.h"

namespace {

template<typename M>
void test_map(M&& map_func, bool simple) {
  raw_array<int32_t> src = simple ? raw_array<int32_t>{1, 2, 3, 6, 9, -8, 4, 17, 23}
                                  : gen_array(10'000'000, -1000, 1000);
  auto expected = src;
  auto mapper = [](int32_t x) { return x * x; };
  std::transform(expected.begin(), expected.end(), expected.begin(), mapper);
  auto real = map_func(src, mapper);
  ASSERT_EQ(expected, real);
}

template<typename S>
void test_exclusive_scan(S&& scan_func, bool simple) {
  raw_array<int32_t> src = simple ? raw_array<int32_t>{1, 2, 3, 6, 9, -8, 4, 17, 23}
                                  : gen_array(10'000'000, -10, 10);
  auto expected = src;
  { // no std::exclusive_scan in clang-10 :(
    std::partial_sum(src.begin(), src.end(), expected.begin());
    std::rotate(expected.begin(), std::prev(expected.end()), expected.end()); // rotate right
    expected[0] = 0;
  }
  auto real = scan_func(src);
  ASSERT_EQ(expected, real.first);
  ASSERT_EQ(expected.back() + src.back(), real.second);
}

template<typename F>
void test_filter(F&& filter_func, bool simple) {
  raw_array<int32_t> src = simple ? raw_array<int32_t>{1, -2, 3, 6, 9, -8, 4, 17, 23}
                                  : gen_array(10'000'000, -10, 10);
  auto filter = [](int32_t x) { return x > 0; };
  std::vector<int32_t> dst;
  std::copy_if(src.begin(), src.end(), std::back_inserter(dst), filter);
  raw_array<int32_t> expected(dst.begin(), dst.end());
  auto real = filter_func(src, filter);
  ASSERT_EQ(expected, real);
}

}

TEST(simple, sequential_map) {
  test_map(
      [](raw_array<int32_t>& src, const auto& f) {
        return seq::map<int32_t>(src, f);
      },
      true
  );
}

TEST(simple, parallel_map) {
  test_map(
      [](raw_array<int32_t>& src, const auto& f) {
        return par::map<int32_t>(src, f, 2);
      },
      true
  );
}

TEST(stress, sequential_map) {
  test_map(
      [](raw_array<int32_t>& src, const auto& f) {
        return seq::map<int32_t>(src, f);
      },
      false
  );
}

TEST(stress, parallel_map) {
  test_map(
      [](raw_array<int32_t>& src, const auto& f) {
        return par::map<int32_t>(src, f, 60);
      },
      false
  );
}

TEST(simple, sequential_exclusive_scan) {
  test_exclusive_scan(seq::exclusive_scan<int32_t>, true);
}

TEST(simple, parallel_exclusive_scan) {
  test_exclusive_scan(
      [](const raw_array<int32_t>& src) {
        return par::exclusive_scan(src, 2);
      },
      true
  );
}

TEST(stress, sequential_exclusive_scan) {
  test_exclusive_scan(seq::exclusive_scan<int32_t>, false);
}

TEST(stress, parallel_exclusive_scan) {
  test_exclusive_scan(
      [](const raw_array<int32_t>& src) {
        return par::exclusive_scan(src, 60);
      },
      false
  );
}

TEST(simple, sequential_filter) {
  test_filter(
      [](const raw_array<int32_t>& src, const auto& p) {
        return seq::filter(src, p);
      },
      true
  );
}

TEST(stress, sequential_filter) {
  test_filter(
      [](const raw_array<int32_t>& src, const auto& p) {
        return seq::filter(src, p);
      },
      false
  );
}



