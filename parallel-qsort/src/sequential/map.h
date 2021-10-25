#pragma once

#include "src/raw_array.h"

namespace seq {

template<typename T, typename F>
raw_array<T> map(const raw_array<T>& src, F const& mapper) {
  raw_array<T> res(src.size());
  for (size_t i = 0; i < src.size(); ++i) {
    res.emplace_at(i, mapper(src[i]));
  }
  return res;
}

}
