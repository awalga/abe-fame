//
// Created by abel walga on 02/05/2018.
//

#ifndef ODNCRYPTO_BILINEAR_PAIRING_HPP
#define ODNCRYPTO_BILINEAR_PAIRING_HPP

#include "odnfc/cbor.hpp"
#include "group.hpp"

namespace odn {
namespace crypto {

class bilinear_pairing {
 public :
  explicit bilinear_pairing(size_t klinear) : k(klinear) {
    init_pairing(*this);
  }
  bilinear_pairing(const bilinear_pairing &other) = default;
  bilinear_pairing(bilinear_pairing &&other) = default;
  ~bilinear_pairing() = default;

  size_t klinear() const {
    return k;
  }

  bool operator==(const bilinear_pairing &rhs) const;
  bool operator!=(const bilinear_pairing &rhs) const;
 private:
  /***********************************************
   * * 		O/I operations
   * *********************************************/
  template<typename T>
  friend T &serialize(T &, const bilinear_pairing &);
  template<typename U, typename V>
  friend
  class odn::fc::adl_serializer;

  const size_t k;
  field_zr order_g1;
  field_zr order_g2;
  field_zr order_gt;

  static void init_pairing(bilinear_pairing &);
};
template<typename T>
T &serialize(T &ar, const bilinear_pairing &pairing) {
  auto zr_vector = ar.array(4);
  zr_vector.push(pairing.k);
  zr_vector.push(pairing.order_g1);
  zr_vector.push(pairing.order_g2);
  zr_vector.push(pairing.order_gt);
  return ar;
}

}// end namespace crypto
}// end namespace odn

namespace odn {
namespace fc {
using namespace ::odn::crypto;

template<typename T>
struct adl_serializer<T, bilinear_pairing> {

  bilinear_pairing operator()(T &ar) {
    auto zr_vector = ar.array();
    size_t klinear = 0;
    zr_vector.pop(&klinear);// TODO deserialize other fields and check them
    return bilinear_pairing(klinear);
  }
};
}// end namespace fc
}// end namespace odn
#endif //ODNCRYPTO_BILINEAR_PAIRING_HPP
