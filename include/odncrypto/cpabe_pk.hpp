//
// Created by abel walga on 02/05/2018.
//

#ifndef ODNCRYPTO_CPABE_PK_HPP
#define ODNCRYPTO_CPABE_PK_HPP

#include "bilinear_pairing.hpp"
#include "group_h.hpp"
#include "group_gt.hpp"
#include <vector>

using namespace ::std;

namespace odn {
namespace crypto {

class cpabe_pk {
 public :
  cpabe_pk() = default;
  cpabe_pk(bilinear_pairing pairing, group_h h_, vector<group_h> h_k_, vector<group_gt> t_k_) : pairing_parm(std::move(
      pairing)), h(std::move(h_)), h_k(h_k_), t_k(t_k_) {}
  bool is_valid() const {
    return h_k.size() == pairing_parm.klinear() && t_k.size() == pairing_parm.klinear();
  }
  ~cpabe_pk() {}
 private:
  friend class odn_cpabe;

  bilinear_pairing pairing_parm;
  group_h h;// G2
  vector<group_h> h_k;// G2[]
  vector<group_gt> t_k;// GT[]
};
}// end namespace crypto
}// end namespace odn
#endif //ODNCRYPTO_CPABE_PK_HPP
