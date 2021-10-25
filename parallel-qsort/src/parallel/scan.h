#pragma once

#include "src/sequential/scan.h"

namespace par {

template<typename T>
std::pair<raw_array<T>, T> exclusive_scan(const raw_array<T>& src, size_t blocks_cnt) {
  assert(blocks_cnt >= 1);
  if (src.empty()) {
    return {src, T{}};
  }
  size_t block_size = (src.size() + blocks_cnt - 1) / blocks_cnt;
  raw_array<T> res(src.size());
  raw_array<T> block_sums(blocks_cnt);

  #pragma grainsize 1
  cilk_for (size_t i = 0; i < blocks_cnt; ++i) {
    size_t l = i * block_size;
    size_t r = std::min(src.size(), (i + 1) * block_size);
    if (l < src.size()) {
      res.emplace_at(l);
      for (size_t j = l + 1; j < r; ++j) {
        res.emplace_at(j, res[j - 1] + src[j - 1]);
      }
      block_sums.emplace_at(i, res[r - 1] + src[r - 1]);
    } else {
      block_sums.emplace_at(i);
    }
  }

  seq::exclusive_scan_inplace(block_sums);

  #pragma grainsize 1
  cilk_for (size_t i = 0; i < blocks_cnt; ++i) {
    size_t l = i * block_size;
    size_t r = std::min(src.size(), (i + 1) * block_size);
    for (size_t j = l; j < r; ++j) {
      res[j] += block_sums[i];
    }
  }
  T total = res.back() + src.back();
  return {std::move(res), std::move(total)};
}

}
