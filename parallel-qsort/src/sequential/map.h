#pragma once

#include "src/raw_array.h"

namespace seq {

template<typename U, typename T, typename F>
raw_array<U> map(const raw_array<T>& src, F const& mapper) {
  raw_array<U> res(src.size());
  for (size_t i = 0; i < src.size(); ++i) {
    res.emplace_at(i, mapper(src[i]));
  }
  return res;
}

}
