//
// Created by abel walga on 19/04/2018.
//

#ifndef ODNCRYPTO_ZR_HPP
#define ODNCRYPTO_ZR_HPP
#include "relic_import.h"
#include "relic_util.hpp"
#include "group_arithmetics.hpp"

namespace odn {
namespace crypto {
/**
 * <p>Group element (+,x) of order r.</p>
 */
class field_zr {

 public:
  field_zr() {
    bn_new(r);
    bn_new(z);
  }

  field_zr(unsigned long long int r_, unsigned long long int z_) {
    bn_new(r);
    bn_new(z);
    unsigned char chars0[8];
    for (int v = 0; v < 8; v++)
      chars0[7 - v] = ((r_ >> (v * 8)) % 256);
    bn_read_bin(r, chars0, 8);

    unsigned char chars1[8];
    for (int v = 0; v < 8; v++)
      chars1[7 - v] = ((z_ >> (v * 8)) % 256);
    bn_read_bin(z, chars1, 8);
    bn_mod(z, z, r);
  }

  field_zr(const field_zr &other) {
    bn_new(r);
    bn_new(z)
    bn_copy(r, other.r);
    bn_copy(z, other.z);
  }

  field_zr(field_zr &&other) {
    bn_new(r);
    bn_new(z);
    relic_memory::relic_move(r, other.r);
    relic_memory::relic_move(z, other.z);
  }

  ~field_zr() {
    relic_memory::relic_free(z);
    relic_memory::relic_free(r);
  }

  field_zr &operator=(const field_zr &other) {
    relic_memory::relic_free(r);// TODO copy first and restore temporary if error
    relic_memory::relic_free(z);// TODO copy first and restore temporary if error

    bn_copy(r, other.r);
    bn_copy(z, other.z);
    return *this;
  }

  field_zr &operator=(field_zr &&other) {
    relic_memory::relic_free(r);// TODO copy first and restore temporary if error
    relic_memory::relic_free(z);// TODO copy first and restore temporary if error

    relic_memory::relic_move(r, other.r);
    relic_memory::relic_move(z, other.z);
    return *this;
  }

  bool operator==(const field_zr &rhs) const {
    return bn_cmp(r, rhs.r) == CMP_EQ &&
        bn_cmp(z, rhs.z) == CMP_EQ;
  }

  bool operator!=(const field_zr &rhs) const {
    return !(rhs == *this);
  }

  field_zr &random() {
    bn_rand_mod(z, r);
    return *this;
  }

  field_zr crandom() const {
    field_zr rand(*this);
    bn_rand_mod(rand.z, rand.r);
    return std::move(rand);
  }

  /***********************************************
   * * 		operations +, *, -, /
   * *********************************************/
  /**
   * <p> addition inverse in group </p>
   *
   * @return
   */
  field_zr &operator-() {
    bn_neg(z, z);
    bn_mod(z, z, r);
    return *this;
  }

  /**
   * <p> addition inverse in group </p>
   *
   * @return
   */
  field_zr operator-() const {
    field_zr neg(*this);
    -neg;
    return std::move(neg);
  }

  /**
   * <p>group addition </p>
   *
   * @param rhs
   * @return
   */
  field_zr &operator+=(const field_zr &rhs) {
    bn_add(z, z, rhs.z);
    bn_mod(z, z, r);
    return *this;
  }
  /**
   * <p>group addition </p>
   *
   * @param right
   * @return
   */
  const field_zr operator+(const field_zr &right) const {
    field_zr sum(*this);
    sum += right;
    return std::move(sum);
  }

  /**
   * <p>group multiplication </p>
   *
   * @param other
   * @return
   */
  field_zr &operator*=(const field_zr &other) {
    bn_mul(z, z, other.z);
    bn_mod(z, z, r);
    return *this;
  }

  /**
   * <p>group multiplication </p>
   *
   * @param right
   * @return
   */
  const field_zr operator*(const field_zr &right) const {
    field_zr product(*this);
    product *= right;
    return std::move(product);
  }

  /**
   * <p>group addition (inverse)</p>
   *
   * @param other
   * @return
   */
  field_zr &operator-=(const field_zr &other) {
    bn_sub(z, z, other.z);
    bn_mod(z, z, r);
    return *this;
  }

  /**
   * <p>group addition (inverse)</p>
   * @param right
   * @return
   */
  const field_zr operator-(const field_zr &right) const {
    field_zr sub(*this);
    sub -= right;
    return std::move(sub);
  }

  /**
   * <p>group modular division </p>
   *
   * @param rhs
   * @return
   * @throw ERR_NO_VALID if gcd(lhs, rhs) != 1
   */
  field_zr &operator/=(const field_zr &rhs) {
    group_arithmetics::mod_div(*this, *this, rhs);
    return *this;
  }

  /**
   * <p>groupd modular division </p>
   *
   * @param rhs
   * @return
   * @throw ERR_NO_VALID if gcd(lhs, rhs) != 1
   */
  const field_zr operator/(const field_zr &rhs) const {
    field_zr quot(*this);
    group_arithmetics::mod_div(quot, *this, rhs);
    return std::move(quot);
  }

  constexpr static size_t size() {
    return relic_type::size_bn();
  }

 private:
  friend group_arithmetics;
  template<typename T>
  friend
  class group;

  /***********************************************
   * * 		O/I operations
   * *********************************************/
  template<typename T>
  friend T &serialize(T &, const field_zr &);

  template<typename T>
  friend T &deserialize(T &, field_zr &);

  bn_t r;
  bn_t z;
  field_zr(const bn_t &r_, const bn_t &z_) {
    bn_new(r);
    bn_new(z);
    bn_copy(r, r_);
    bn_copy(z, z_);
  }
};

template<typename T>
T &serialize(T &ar, const field_zr &zr) {
  auto zr_vector = ar.array();
  uint8_t buffer[field_zr::size()];
  bn_write_bin(buffer, field_zr::size(), zr.r);
  zr_vector.push(buffer);
  bn_write_bin(buffer, field_zr::size(), zr.z);
  zr_vector.push(buffer);
  return ar;
}

template<typename T>
T &deserialize(T &ar, field_zr &zr) {
  auto zr_vector = ar.array();
  uint8_t buffer[field_zr::size()];
  zr_vector.pop(&buffer);
  bn_read_bin(zr.r, buffer, field_zr::size());
  zr_vector.pop(&buffer);
  bn_read_bin(zr.z, buffer, field_zr::size());
  return ar;
}
}// end namespace crypto
}// end namespace odn
#endif //ODNCRYPTO_ZR_HPP
