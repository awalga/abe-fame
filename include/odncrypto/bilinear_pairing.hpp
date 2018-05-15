//
// Created by abel walga on 02/05/2018.
//

#ifndef ODNCRYPTO_BILINEAR_PAIRING_HPP
#define ODNCRYPTO_BILINEAR_PAIRING_HPP
#include "field_zr.hpp"

namespace odn {
namespace crypto {
class bilinear_pairing {

 public :
  explicit bilinear_pairing(size_t klinear) : k(klinear) {
    init_pairing(*this);
  }
  bilinear_pairing(const bilinear_pairing &other)
      : k(other.k), order_g1(other.order_g1), order_g2(other.order_g2), order_gt(other.order_gt) {
  }

  bilinear_pairing(bilinear_pairing &&other)
      : k(other.k), order_g1(std::move(other.order_g1)), order_g2(std::move(other.order_g2)), order_gt(other.order_gt) {
  }

  ~bilinear_pairing() {
  }

  size_t klinear() const {
    return k;
  }

 private:
  const size_t k;
  field_zr order_g1;
  field_zr order_g2;
  field_zr order_gt;

  static void init_pairing(bilinear_pairing &);
};
}// end namespace crypto
}// end namespace odn
#endif //ODNCRYPTO_BILINEAR_PAIRING_HPP
