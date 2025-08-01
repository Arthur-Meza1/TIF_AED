#ifndef MY_INITIALIZER_LIST_H
#define MY_INITIALIZER_LIST_H

#include <cstddef> // para std::size_t

template <typename T>
class my_initializer_list {
private:
  const T* data_;
  std::size_t size_;

public:
  constexpr my_initializer_list(const T* data, std::size_t size) :
      data_(data),
      size_(size) {}

  constexpr const T* begin() const noexcept {
    return data_;
  }
  constexpr const T* end() const noexcept {
    return data_ + size_;
  }
  constexpr std::size_t size() const noexcept {
    return size_;
  }
};

// Macro para crear un my_initializer_list de forma literal
#define MY_INIT_LIST(name, type, ...)                                          \
  static constexpr type name##_array[] = {__VA_ARGS__};                        \
  constexpr my_initializer_list<type> name(                                    \
      name##_array, sizeof(name##_array) / sizeof(type))

#endif // MY_INITIALIZER_LIST_H
