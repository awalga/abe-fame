//
// Created by abel walga on 19/04/2018.
//

#ifndef ODNCRYPTO_G_HPP
#define ODNCRYPTO_G_HPP

#include "relic_import.h"
#include "relic_util.hpp"
#include "field_zr.hpp"
#include "group_arithmetics.hpp"

namespace odn {
namespace crypto {
/**
 * <p>Group element (+)</p>
 */
class group_g {

 public:

  group_g() : g1() {
    g1_new(g1);
  }

  group_g(const group_g &other) {
    g1_new(g1);
    g1_copy(g1, other.g1);
  }

  group_g(group_g &&other) {
    g1_new(g1);
    relic_memory::relic_move(g1, other.g1);
  }

  ~group_g() {
    relic_memory::relic_free(g1);
  }

  group_g &operator=(const group_g &other) {
    relic_memory::relic_free(g1);// TODO copy first and restore temporary if error
    g1_copy(g1, other.g1);
    return *this;
  }

  group_g &operator=(group_g &&other) {
    relic_memory::relic_free(g1);// TODO copy first and restore temporary if error
    relic_memory::relic_move(g1, other.g1);
    return *this;
  }

  bool operator==(const group_g &rhs) const {
    return g1_cmp(g1, rhs.g1) == CMP_EQ;
  }

  bool operator!=(const group_g &rhs) const {
    return !(rhs == *this);
  }

  group_g &random() {
    g1_rand(g1);
    return *this;
  }

  group_g crandom() const {
    group_g rand;
    g1_rand(rand.g1);
    return std::move(rand);
  }

  /***********************************************
   * * 		operations +, -, ^
   * *********************************************/
  /**
   * <p> addition inverse in group </p>
   *
   * @return
   */
  group_g &operator-() {
    g1_neg(g1, g1);
    return *this;
  }

  /**
   * <p> addition inverse in group </p>
   *
   * @return
   */
  group_g operator-() const {
    group_g neg(*this);
    -neg;
    return std::move(neg);
  }

  /**
   * <p> group addition </p>
   *
   * @param rhs
   * @return
   */
  group_g &operator+=(const group_g &rhs) {
    g1_add(g1, g1, rhs.g1)
    return *this;
  }

  /**
   * <p> group addition </p>
   *
   * @param rhs
   * @return
   */
  const group_g operator+(const group_g &rhs) const {
    group_g sum(*this);
    sum += rhs;
    return std::move(sum);
  }

  /**
   * <p> group addition with inverse </p>
   *
   * @param rhs
   * @return
   */
  group_g &operator-=(const group_g &rhs) {
    g1_sub(g1, g1, rhs.g1);
    return *this;
  }

  /**
   * <p> group addition with inverse </p>
   *
   * @param rhs
   * @return
   */
  const group_g operator-(const group_g &rhs) const {
    group_g sub(*this);
    sub -= rhs;
    return std::move(sub);
  }

  /**
   * <p> group recursive addition </p>
   *
   * @param pow
   * @return
   */
  group_g &operator^=(const field_zr &pow) {
    group_arithmetics::sum(*this, *this, pow);
    return *this;
  }

  /**
   * <p> group recursive addition </p>
   *
   * @param rhs
   * @return
   */
  const group_g operator^(const field_zr &pow) const {
    group_g product(*this);
    group_arithmetics::sum(product, *this, pow);

    return std::move(product);
  }

  /**
   *
   * @return size
   */
  constexpr static size_t size() {
    return 1 + 2 * relic_type::size_fp();
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
  friend T &serialize(T &, const group_g &);
  template<typename T>
  friend T &deserialize(T &, group_g &);

  g1_t g1;
};

template<typename T>
T &serialize(T &ar, const odn::crypto::group_g &g) {
  uint8_t buffer[group_g::size()];
  g1_write_bin(buffer, group_g::size(), g.g1, 0);
  ar.push(buffer);
  return ar;
}

template<typename T>
T &deserialize(T &ar, odn::crypto::group_g &g) {
  uint8_t buffer[group_g::size()];
  ar.pop(&buffer);
  g1_read_bin(g.g1, buffer, group_g::size());
  return ar;
}

}// end namespace crypto
}// end namespace odn
#endif //ODNCRYPTO_G_HPP
