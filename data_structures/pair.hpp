#ifndef PAIR_H
#define PAIR_H

#include <utility>

template <typename T1, typename T2>
struct Pair {
  T1 first;
  T2 second;

  Pair() = default;

  Pair(const T1& a, const T2& b) : first(a), second(b) {}

  Pair(T1&& a, T2&& b) : first(std::move(a)), second(std::move(b)) {}

  // Operador de igualdad
  bool operator==(const Pair& other) const {
    return first == other.first && second == other.second;
  }

  bool operator!=(const Pair& other) const {
    return !(*this == other);
  }

  bool operator<(const Pair& other) const {
    return first < other.first;
  }

  bool operator>(const Pair& other) const {
    return first > other.first;
  }

  bool operator<=(const Pair& other) const {
    return !(*this > other);
  }

  bool operator>=(const Pair& other) const {
    return !(*this < other);
  }
};
#endif
