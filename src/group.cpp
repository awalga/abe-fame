//
// Created by abel walga on 02/05/2018.
//
#include "odncrypto/low/varint.hpp"
#include "odncrypto/group.hpp"
#include <string>

using namespace ::odn::crypto;

field_zr group<field_zr>::zr() {
  field_zr tmp;
  g1_get_ord(tmp.r);
  return std::move(tmp);
}

field_zr group<field_zr>::zr_zero() {
  auto tmp = zr();
  bn_zero(tmp.z);
  return std::move(tmp);
}

field_zr group<field_zr>::zr_unity() {
  auto tmp = zr();
  bn_set_dig(tmp.z, 1);
  return std::move(tmp);
}

field_zr group<field_zr>::zr_map(long long int number) {
  auto sign = ::odn::fc::signum(number);
  auto tmp = zr();
  if (sign == -1) {
    bn_set_dig(tmp.z, -1 * number);
    -tmp;
  } else {
    bn_set_dig(tmp.z, number);

  }
  return std::move(tmp);
}

field_zr group<group_g>::order() {
  field_zr tmp;
  g1_get_ord(tmp.r);
  return field_zr(tmp.r, tmp.r);
}

group_g group<group_g>::g() {
  return group_g();
}

group_g group<group_g>::g_zero() {
  group_g zero;
  g1_set_infty(zero.g1);
  return std::move(zero);
}

group_g group<group_g>::g_map(const std::string &str) {
  group_g map_element;
  const char *str_char = str.c_str();
  unsigned char cdata[str.length() - 1];
  for (size_t i = 0; i < str.length() - 1; ++i) {
    cdata[i] = *str_char++;
  }g1_map(map_element.g1, cdata, str.length() - 1);
  return std::move(map_element);
}

field_zr group<group_h>::order() {
  field_zr tmp;
  g2_get_ord(tmp.r);
  return field_zr(tmp.r, tmp.r);
}

group_h group<group_h>::h() {
  return group_h();
}

group_h group<group_h>::h_zero() {
  group_h zero;
  g2_set_infty(zero.g2);
  return std::move(zero);
}

field_zr group<group_gt>::order() {
  field_zr tmp;
  gt_get_ord(tmp.r);
  return field_zr(tmp.r, tmp.r);
}

group_gt group<group_gt>::gt() {
  return group_gt();
}

group_gt group<group_gt>::gt_unity() {
  group_gt unity;
  gt_set_unity(unity.gt);
  return std::move(unity);
}
