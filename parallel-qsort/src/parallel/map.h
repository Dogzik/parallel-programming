#pragma once

#include <cassert>
#include <cilk/cilk.h>
#include "src/raw_array.h"

namespace par {

template<typename T, typename F>
raw_array<T> map(const raw_array<T>& src, F const& mapper, size_t blocks_cnt) {
  assert(blocks_cnt >= 1);
  raw_array<T> res(src.size());
  size_t block_size = (src.size() + blocks_cnt - 1) / blocks_cnt;
  #pragma cilk grainsize 1
  cilk_for(size_t i = 0; i < blocks_cnt; ++i) {
    size_t l = i * block_size;
    size_t r = std::min(src.size(), (i + 1) * block_size);
    for (size_t j = l; j < r; ++j) {
      res.emplace_at(j, mapper(src[j]));
    }
  }
  return res;
}

}
