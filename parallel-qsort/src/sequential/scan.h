#pragma once

namespace seq {

template<typename T>
std::pair<raw_array<T>, T> exclusive_scan(const raw_array<T>& src) {
  if (src.empty()) {
    return {src, T{}};
  }
  raw_array<T> res(src.size());
  res.emplace_at(0);
  for (size_t i = 1; i < src.size(); ++i) {
    res.emplace_at(i, res[i - 1] + src[i - 1]);
  }
  T total = res.back() + src.back();
  return {std::move(res), std::move(total)};
}

template<typename T>
T exclusive_scan_inplace(raw_array<T>& src) {
  if (src.empty()) {
    return T{};
  }
  T tmp = src[0];
  src[0] = T{};
  for (size_t i = 1; i < src.size(); ++i) {
    T new_tmp = src[i];
    src[i] = src[i - 1] + tmp;
    tmp = std::move(new_tmp);
  }
  return src.back() + tmp;
}

template<typename T>
raw_array<T> inclusive_scan(const raw_array<T>& src) {
  if (src.empty()) {
    return src;
  }
  raw_array<T> res(src.size());
  res.emplace_at(0, src[0]);
  for (size_t i = 1; i < src.size(); ++i) {
    res.emplace_at(i, res[i - 1] + src[i]);
  }
  return res;
}

}
