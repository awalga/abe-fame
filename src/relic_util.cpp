//
// Created by abel walga on 02/05/2018.
//
#include "odncrypto/relic_util.hpp"

namespace odn {
namespace crypto {
template<>
void copyT(bn_t &lhs, const bn_t &rhs) {
  bn_new(lhs);
  bn_copy(lhs, rhs);
}

template<>
void destroyT(bn_t &element) {
  relic_memory::relic_free(element);
}

template<>
void copyT(g1_t &lhs, const g1_t &rhs) {
  g1_new(lhs);
  g1_copy(lhs, rhs);
}

template<>
void destroyT(g1_t &element) {
  relic_memory::relic_free(element);
}

template<>
void copyT(g2_t &lhs, const g2_t &rhs) {
  g2_new(lhs);
  g2_copy(lhs, const_cast<g2_t &>(rhs));
}

template<>
void destroyT(g2_t &element) {
  relic_memory::relic_free(element);
}

template<>
void copyT(gt_t &lhs, const gt_t &rhs) {
  gt_new(lhs);
  gt_copy(lhs, const_cast<gt_t &>(rhs));
}

template<>
void destroyT(gt_t &element) {
  gt_free(element);
}
}
}
