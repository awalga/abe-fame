//
// Created by abel walga on 02/05/2018.
//

#ifndef ODNCRYPTO_GROUP_HPP
#define ODNCRYPTO_GROUP_HPP
#include "low/varint.hpp"
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
    return RELIC_BN_BYTES;
  }
};

template<>
struct group<group_g> {
  static field_zr order();
  static group_g g();
  static group_g g_zero();
  static group_g g_map(const std::string& str);

  constexpr static size_t size() {
    return FP_BYTES;
  }

  static void write(const group_g&, uint8_t *, unsigned long long int);
  static group_g read(const uint8_t *, unsigned long long int);
};
template<>
struct group<group_h> {
  static field_zr order();
  static group_h h();
  static group_h h_zero();

  static constexpr size_t size() {
    return 2 * FP_BYTES;
  }

  static void write(const group_h&, uint8_t *, unsigned long long int);
  static group_h read(const uint8_t *, unsigned long long int);
};
template<>
struct group<group_gt> {
  static field_zr order();
  static group_gt gt();
  static group_gt gt_unity();

  static constexpr size_t size() {
    return 12 * group<group_g>::size();
  }

  static void write(const group_gt&, uint8_t *, unsigned long long int);
  static group_gt read(const uint8_t *, unsigned long long int);
};
}// end namespace crypto
}// end namespace odn
#endif //ODNCRYPTO_GROUP_HPP
