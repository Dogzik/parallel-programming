#pragma once

#include <cstdint>
#include <new>
#include <type_traits>

template<typename T>
class raw_array {
  static_assert(std::is_trivially_destructible_v<T>);
  static_assert(std::is_nothrow_copy_constructible_v<T>);
public:
  explicit raw_array(size_t array_size = 0)
      : size_(array_size), data_(nullptr) {
    if (size_ > 0) {
      data_ = static_cast<T*>(operator new(sizeof(T) * size_));
    }
  }

  raw_array(const std::initializer_list<T>& elems)
      : raw_array(elems.size()) {
    for (size_t i = 0; i < elems.size(); ++i) {
      emplace_at(i, *(elems.begin() + i));
    }
  }

  template<class It>
  raw_array(It begin, It end) : raw_array(std::distance(begin, end)) {
    for (size_t i = 0; i < size_; ++i) {
      emplace_at(i, begin[i]);
    }
  }

  raw_array(raw_array<T> const& other)
      : size_(other.size_), data_(nullptr) {
    if (size_ > 0) {
      data_ = static_cast<T*>(operator new(sizeof(T) * other.size_));
      for (uint32_t i = 0; i < other.size_; ++i) {
        new(data_ + i) T(other[i]);
      }
    }
  }

  raw_array(raw_array<T>&& other) noexcept
      : size_(other.size_), data_(other.data_) {
    other.data_ = nullptr;
    other.size_ = 0;
  }

  T* get_raw_ptr() {
    return data_;
  }

  T const* get_raw_ptr() const {
    return data_;
  }

  template<typename ... Args>
  T& emplace_at(size_t idx, Args&& ...args) {
    static_assert(std::is_nothrow_constructible_v<T, Args&& ...>);
    new(data_ + idx) T(std::forward<Args>(args)...);
    return data_[idx];
  }

  T const& operator[](size_t idx) const {
    return data_[idx];
  }

  T& operator[](size_t idx) {
    return data_[idx];
  }

  T& back() noexcept {
    return data_[size_ - 1];
  }

  T& back() const noexcept {
    return data_[size_ - 1];
  }

  bool empty() const noexcept {
    return size() == 0;
  }

  size_t size() const {
    return size_;
  }

  T* begin() noexcept {
    return data_;
  }

  T* begin() const noexcept {
    return data_;
  }

  T* end() noexcept {
    return data_ + size_;
  }

  T* end() const noexcept {
    return data_ + size_;
  }

  bool operator==(const raw_array& other) const {
    if (size() != other.size()) {
      return false;
    }
    return std::equal(begin(), end(), other.begin());
  }

  ~raw_array() {
    operator delete(data_);
  }

private:
  size_t size_;
  T* data_;
};
