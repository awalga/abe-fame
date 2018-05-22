//
// Created by abel walga on 02/05/2018.
//

#ifndef ODNCRYPTO_GROUP_HPP
#define ODNCRYPTO_GROUP_HPP
#include "relic_import.h"
#include "field_zr.hpp"
#include "group_g.hpp"
#include "group_h.hpp"
#include "group_gt.hpp"

namespace odn {
namespace crypto {

template<typename T>
struct group;
template<>
struct group<field_zr> {
  static field_zr zr();
  static field_zr zr_zero();
  static field_zr zr_unity();
  static field_zr zr_map(long long int);

  constexpr static size_t size() {
    return field_zr::size();
  }
};
template<>
struct group<group_g> {
  static field_zr order();
  static group_g g();
  static group_g g_zero();
  static group_g g_map(const std::string &str);

  constexpr static size_t size() {
    return group_g::size();
  }
};
template<>
struct group<group_h> {
  static field_zr order();
  static group_h h();
  static group_h h_zero();

  static constexpr size_t size() {
    return group_h::size();
  }
};
template<>
struct group<group_gt> {
  static field_zr order();
  static group_gt gt();
  static group_gt gt_unity();

  static constexpr size_t size() {
    return group_gt::size();
  }
};
}// end namespace crypto
}// end namespace odn
#endif //ODNCRYPTO_GROUP_HPP
