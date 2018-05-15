//
// Created by abel walga on 02/05/2018.
//

#ifndef ODNCRYPTO_CPABE_MSK_HPP
#define ODNCRYPTO_CPABE_MSK_HPP

#include "bilinear_pairing.hpp"
#include "group_g.hpp"
#include "group_h.hpp"
#include "group_gt.hpp"
#include "cpabe_pk.hpp"
#include <vector>

using namespace ::std;

namespace odn {
namespace crypto {

class cpabe_msk {
 public :

  cpabe_msk() = default;
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

  ~cpabe_msk() {}

 private:
  friend class odn_cpabe;

  bilinear_pairing pairing_param;
  group_g g;// G1
  group_h h;// G2
  vector<field_zr> a_k;// Zr[]
  vector<field_zr> b_k;// Zr[]
  vector<field_zr> d_k;// Zr[]
  vector<group_g> g_k;// G1
};
}// end namespace crypto
}// end namespace odn
#endif //ODNCRYPTO_CPABE_MSK_HPP
