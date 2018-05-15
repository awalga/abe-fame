//
// Created by abel walga on 02/05/2018.
//

#ifndef ODNCRYPTO_GROUP_GT_HPP
#define ODNCRYPTO_GROUP_GT_HPP
#include "relic_import.h"
#include "relic_util.hpp"
#include "group_arithmetics.hpp"
#include <functional>                                       // std::bind

namespace odn {
namespace crypto {
/**
 * <p>group element (*)</p>
 */
class group_gt {

 public :
  group_gt() {
    gt_new(gt);
  }
  group_gt(const group_gt &other) {
    gt_new(gt);
    temporary(other.gt, std::bind(fp12_copy, std::ref(gt), std::placeholders::_1));
  }
  group_gt(group_gt &&other) {
    gt_new(gt);
    relic_memory::relic_move(gt, other.gt);
  }

  group_gt &operator=(const group_gt &other) {
    relic_memory::relic_free(gt);// TODO copy first and restore temporary if error
    temporary(other.gt, std::bind(fp12_copy, std::ref(gt), std::placeholders::_1));

    return *this;
  }
  group_gt &operator=(group_gt &&other) {
    relic_memory::relic_free(gt);// TODO copy first and restore temporary if error
    relic_memory::relic_move(gt, other.gt);

    return *this;
  }

  bool operator==(const group_gt &rhs) const {
    return gt_cmp(const_cast<gt_t &>(gt), const_cast<gt_t &>(rhs.gt)) == CMP_EQ;
  }

  bool operator!=(const group_gt &rhs) const {
    return !(rhs == *this);
  }

  group_gt &random() {
    gt_rand(gt);
    return *this;
  }

  group_gt crandom() const {
    group_gt rand(*this);
    gt_rand(rand.gt);
    return std::move(rand);
  }

  group_gt &operator()(const group_g &g, const group_h &h) {
    group_arithmetics::pair(*this, g, h);
    return *this;
  }

  group_gt operator()(const group_g &g, const group_h &h) const {
    group_gt pair(*this);
    group_arithmetics::pair(pair, g, h);
    return std::move(pair);
  }
  /***********************************************
  * * 		operations *, /, ^
  * *********************************************/

  /**
   * <p> group multiplication  </p>
   * @param rhs
   * @return
   */
  group_gt &operator*=(const group_gt &rhs) {
    // temporary(rhs.gt, std::bind(fp12_mul_lazyr, std::ref(gt), std::ref(gt), std::placeholders::_1));
    gt_mul(gt, gt, const_cast<gt_t &>(rhs.gt));
    return *this;
  }

  /**
   * <p> group multiplication </p>
   *
   * @param rhs
   * @return
   */
  const group_gt operator*(const group_gt &rhs) const {
    group_gt product(*this);
    product *= rhs;
    return std::move(product);
  }

  /**
   * <p> group multiplication with inverse </p>
   *
   * @param rhs
   * @return
   */
  group_gt &operator/=(const group_gt &rhs) {
    group_gt inverse;
    //temporary(rhs.gt, std::bind(fp12_inv_uni, std::ref(inverse.gt), std::placeholders::_1));
    gt_inv(inverse.gt, const_cast<gt_t &>(rhs.gt));
    gt_mul(gt, gt, inverse.gt);
    // (*this) *= inverse;
    return *this;
  }

  /**
   * <p> group addition with inverse </p>
   *
   * @param rhs
   * @return
   */
  const group_gt operator/(const group_gt &rhs) const {
    group_gt quotient(*this);
    quotient /= rhs;
    return std::move(quotient);
  }

  /**
   * <p> group recursive multiplication </p>
   * @param rhs
   * @return
   */
  group_gt &operator^=(const field_zr &pow) {
    group_arithmetics::product(*this, *this, pow);
    return *this;
  }

  /**
   * <p> group recursive multiplication </p>
   *
   * @param rhs
   * @return
   */
  const group_gt operator^(const field_zr &pow) const {
    group_gt product(*this);
    group_arithmetics::product(product, *this, pow);

    return std::move(product);
  }

 private:
  friend group_arithmetics;
  template<typename T>
  friend
  class group;

  gt_t gt;
};
}// end namespace crypto
}// end odn
#endif //ODNCRYPTO_GROUP_GT_HPP
