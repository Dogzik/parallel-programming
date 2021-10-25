#pragma once

#include "src/raw_array.h"
#include "src/parallel/map.h"
#include "src/parallel/scan.h"

namespace par {

template<typename T, typename F>
raw_array<T> filter(const raw_array<T>& src, const F& predicate, size_t blocks_cnt) {
  if (src.empty()) {
    return src;
  }
  auto flags = par::map<size_t>(src, [predicate](const T& x) -> size_t { return predicate(x); }, blocks_cnt);
  auto [indices, total_cnt] = par::exclusive_scan(flags, blocks_cnt);
  raw_array<T> res(total_cnt);

  size_t block_size = (src.size() + blocks_cnt - 1) / blocks_cnt;
  #pragma grainsize 1
  cilk_for (size_t i = 0; i < blocks_cnt; ++i) {
    size_t l = i * block_size;
    size_t r = std::min(src.size(), (i + 1) * block_size);
    for (size_t j = l; j < r; ++j) {
      if (flags[j] == 1) {
        res.emplace_at(indices[j], src[j]);
      }
    }
  }
  return res;
}

}
