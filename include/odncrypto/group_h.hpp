//
// Created by abel walga on 02/05/2018.
//

#ifndef ODNCRYPTO_H_HPP
#define ODNCRYPTO_H_HPP
#include "relic_import.h"
#include "relic_util.hpp"
#include "group_arithmetics.hpp"
#include "field_zr.hpp"
#include <functional>                                       // std::bind

namespace odn {
namespace crypto {
/**
 * <p>Group element (+)</p>
 */
class group_h {

 public :

  group_h() {
    g2_new(g2);
  }

  group_h(const group_h &other) {
    g2_new(g2);
    temporary(other.g2, std::bind(ep2_copy, std::ref(g2), std::placeholders::_1));
  }

  group_h(group_h &&other) {
    g2_new(g2);
    relic_memory::relic_move(g2, other.g2);
  }

  ~group_h() {
    relic_memory::relic_free(g2);
  }

  group_h &operator=(const group_h &other) {
    relic_memory::relic_free(g2);// TODO copy first and restore temporary if error
    temporary(other.g2, std::bind(ep2_copy, std::ref(g2), std::placeholders::_1));

    return *this;
  }
  group_h &operator=(group_h &&other) {
    relic_memory::relic_free(g2);// TODO copy first and restore temporary if error
    relic_memory::relic_move(g2, other.g2);
    return *this;
  }

  bool operator==(const group_h &rhs) const {
    return g2_cmp(const_cast<g2_t &>(g2), const_cast<g2_t &>(rhs.g2)) == CMP_EQ;
  }

  bool operator!=(const group_h &rhs) const {
    return !(rhs == *this);
  }

  group_h &random() {
    g2_rand(g2);
    return *this;
  }

  group_h crandom() const {
    group_h rand;
    g2_rand(rand.g2);
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
  group_h &operator-() {
    g2_neg(g2, g2);
    return *this;
  }

  /**
   * <p> addition inverse in group </p>
   *
   * @return
   */
  group_h operator-() const {
    group_h neg(*this);
    -neg;
    return std::move(neg);
  }

  /**
   * <p> group addition </p>
   * @param rhs
   * @return
   */
  group_h &operator+=(const group_h &rhs) {
    temporary(rhs.g2, std::bind(ep2_add_projc, std::ref(g2), std::ref(g2), std::placeholders::_1));
    return *this;
  }

  /**
   * <p> group addition </p>
   *
   * @param rhs
   * @return
   */
  const group_h operator+(const group_h &rhs) const {
    group_h sum(*this);
    sum += rhs;
    return std::move(sum);
  }

  /**
   * <p> group addition with inverse </p>
   *
   * @param rhs
   * @return
   */
  group_h &operator-=(const group_h &rhs) {
    temporary(rhs.g2, std::bind(ep2_sub_projc, std::ref(g2), std::ref(g2), std::placeholders::_1));

    return *this;
  }

  /**
   * <p> group addition with inverse </p>
   *
   * @param rhs
   * @return
   */
  const group_h operator-(const group_h &rhs) const {
    group_h sub(*this);
    sub -= rhs;
    return std::move(sub);
  }

  /**
   * <p> group recursive addition </p>
   * @param rhs
   * @return
   */
  group_h &operator^=(const field_zr &pow) {
    group_arithmetics::sum(*this, *this, pow);
    return *this;
  }

  /**
   * <p> group recursive addition </p>
   *
   * @param rhs
   * @return
   */
  const group_h operator^(const field_zr &pow) const {
    group_h product(*this);

    group_arithmetics::sum(product, *this, pow);

    return std::move(product);
  }

 private:
  friend group_arithmetics;
  template<typename T>
  friend
  class group;

  g2_t g2;
};
}// end namespace crypto
}// end namespace odn
#endif //ODNCRYPTO_H_HPP
