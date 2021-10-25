#include <random>
#include <algorithm>
#include "tests/test_common.h"

static std::default_random_engine engine(1337); // to make tests reproducible

raw_array<int32_t> gen_array(size_t size, int32_t min_elem, int32_t max_elem) {
  raw_array<int32_t> res(size);
  std::uniform_int_distribution<int32_t> distribution(min_elem, max_elem);
  for (size_t i = 0; i < size; ++i) {
    res.emplace_at(i, distribution(engine));
  }
  return res;
}

