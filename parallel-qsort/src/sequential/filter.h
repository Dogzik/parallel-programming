#pragma once

#include <algorithm>
#include <cassert>

namespace seq {

template<typename T, typename F>
raw_array<T> filter(const raw_array<T>& src, const F& predicate) {
  size_t res_size = std::count_if(src.begin(), src.end(), predicate);
  raw_array<T> res(res_size);
  size_t cur_pos = 0;
  for (const auto& x : src) {
    if (predicate(x)) {
      res.emplace_at(cur_pos++, x);
    }
  }
  assert(cur_pos == res_size);
  return res;
}

}
