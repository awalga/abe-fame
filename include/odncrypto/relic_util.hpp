//
// Created by abel walga on 02/05/2018.
//

#ifndef ODNCRYPTO_RELIC_UTIL_HPP
#define ODNCRYPTO_RELIC_UTIL_HPP

#include "relic_import.h"
#include <type_traits>

using namespace ::std;

namespace odn {
namespace crypto {

struct relic_mem_alloc_auto {};
struct relic_mem_alloc_static {};
struct relic_mem_alloc_dynamic {};
struct relic_mem_alloc_stack {};

struct relic_mem_alloc_strategy {// TODO use relic flags to extract allocator type
  typedef relic_mem_alloc_auto type;
  static relic_mem_alloc_auto value;
};

template<typename T, typename Enabler>
void relic_mem_move(bn_t &, bn_t &, Enabler);
template<typename T, typename Enabler>
void relic_mem_free(bn_t &, Enabler);

template<typename T>
void relic_mem_move(T &lhs, T &rhs, relic_mem_alloc_auto) {
  using std::swap;
  swap(lhs, rhs);
}

template<typename T>
void relic_mem_free(T &, relic_mem_alloc_auto) {
  // NO_OP
}

struct relic_memory {
  template<typename T>
  inline
  static void relic_move(T &lhs, T &rhs) {
    relic_mem_move(lhs, rhs, relic_mem_alloc_strategy::value);
  }
  template<typename T>
  inline
  static void relic_free(T &lhs) {
    relic_mem_free(lhs, relic_mem_alloc_strategy::value);
  }
};

struct relic_type {
  constexpr static size_t size_bn() {
    return RELIC_BN_BYTES;
  }
  constexpr static size_t size_fp() {
    return FP_BYTES;
  }
};

template<typename T>
void copyT(T &, const T &);

template<typename T>
void destroyT(T &);

template<typename T>
struct _Eval_Type_Traits {
  typedef typename std::remove_const<T>::type _NO_CONSTANT_TYPE;
};

template<typename T, typename F>
void temporary(const T &other, F &&callable) {
  typename _Eval_Type_Traits<T>::_NO_CONSTANT_TYPE temp;
  copyT(temp, other);
  callable(temp);
  destroyT(temp);
};
}// end namespace crypt
}// end namepsace odn
#endif //ODNCRYPTO_RELIC_UTIL_HPP
