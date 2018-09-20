//
// Created by abel walga on 02/05/2018.
//

#ifndef ODNCRYPTO_CPABE_MSK_HPP
#define ODNCRYPTO_CPABE_MSK_HPP

#include "bilinear_pairing.hpp"
#include <vector>

using namespace ::std;

namespace odn {
namespace crypto {

class cpabe_msk {
 public :
  cpabe_msk(bilinear_pairing pairing, group_g g_,
            group_h h_,
            vector<field_zr> a_k_,
            vector<field_zr> b_k_,
            vector<field_zr> d_k_,
            vector<group_g> g_k_) :
      pairing_param(std::move(pairing)),
      g(std::move(g_)),
      h(std::move(h_)),
      a_k(std::move(a_k_)),
      b_k(std::move(b_k_)),
      d_k(std::move(d_k_)),
      g_k(std::move(g_k_)) {}
  bool is_valid() const {
    return a_k.size() == pairing_param.klinear() && b_k.size() == pairing_param.klinear()
        && d_k.size() == (pairing_param.klinear() + 1) && g_k.size() == (pairing_param.klinear() + 1);
  }

  ~cpabe_msk() = default;
 private:
  friend class odn_abe;
  /***********************************************
   * * 		O/I operations
   * *********************************************/
  template<typename T>
  friend T &serialize(T &, const cpabe_msk &);

  bilinear_pairing pairing_param;
  group_g g;// G1
  group_h h;// G2
  vector<field_zr> a_k;// Zr[]
  vector<field_zr> b_k;// Zr[]
  vector<field_zr> d_k;// Zr[]
  vector<group_g> g_k;// G1
};

template<typename T>
T &serialize(T &ar, const cpabe_msk &msk) {
  auto _array_ar = ar.array(7);

  _array_ar.push(msk.pairing_param);
  _array_ar.push(msk.g);
  _array_ar.push(msk.h);
  _array_ar.push(msk.a_k);
  _array_ar.push(msk.b_k);
  _array_ar.push(msk.d_k);
  _array_ar.push(msk.g_k);

  return ar;
}
}// end namespace crypto
}// end namespace odn
#include "odnfc/cbor.hpp"

namespace odn {
namespace fc {
using namespace ::odn::crypto;

template<typename T>
struct adl_serializer<T, cpabe_msk> {

  cpabe_msk operator()(T &ar) {
    auto _array_reader = ar.array();

    bilinear_pairing pairing(_array_reader.template pop<bilinear_pairing>());

    group_g g;// G1
    _array_reader.pop(&g);
    group_h h;// G2
    _array_reader.pop(&h);
    vector<field_zr> a_k;// Zr[]
    _array_reader.pop(&a_k);
    vector<field_zr> b_k;// Zr[]
    _array_reader.pop(&b_k);
    vector<field_zr> d_k;// Zr[]
    _array_reader.pop(&d_k);
    vector<group_g> g_k;// G1
    _array_reader.pop(&g_k);

    return cpabe_msk(std::move(pairing),
                     std::move(g),
                     std::move(h),
                     std::move(a_k),
                     std::move(b_k),
                     std::move(d_k),
                     std::move(g_k));
  }

};
}// end namespace fc
}// end namespace odn
#endif //ODNCRYPTO_CPABE_MSK_HPP
