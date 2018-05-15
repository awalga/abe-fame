//
// Created by abel walga on 04/05/2018.
//

#ifndef ODNCRYPTO_VARINT_HPP
#define ODNCRYPTO_VARINT_HPP
#include <type_traits>

namespace odn {
namespace fc {
template <typename T> inline constexpr
int signum(T x, std::false_type is_signed) {
  return T(0) < x;
}

template <typename T> inline constexpr
int signum(T x, std::true_type is_signed) {
  return (T(0) < x) - (x < T(0));
}

template <typename T> inline constexpr
int signum(T x) {
  return signum(x, std::is_signed<T>());
}
}// end namespace fc
}// end namespace odn
#endif //ODNCRYPTO_VARINT_HPP
