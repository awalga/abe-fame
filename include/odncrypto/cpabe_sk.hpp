//
// Created by abel walga on 02/05/2018.
//

#ifndef ODNCRYPTO_CPABE_SK_HPP
#define ODNCRYPTO_CPABE_SK_HPP

#include "bilinear_pairing.hpp"
#include "group_g.hpp"
#include "group_h.hpp"

#include <vector>
#include <string>
#include <set>
#include <map>
#include <memory>                             // std::unique_ptr

namespace odn {
namespace crypto {

class cpabe_sk {
 public :

  cpabe_sk() = delete;
  cpabe_sk(bilinear_pairing pairing,
           vector<group_h> sk0_,
           vector<group_g> sk1_,
           map<string, vector<group_g>> sky_)
      : pairing_param(std::move(pairing)), sk0(std::move(sk0_)), sk1(std::move(sk1_)), sky(std::move(sky_)) {
  }
  bool is_valid() const {
    return sk0.size() == (pairing_param.klinear() + 1) && sk1.size() == (pairing_param.klinear() + 1) && sky.size() > 0;
  }
  ~cpabe_sk() {}

 private:
  friend class odn_cpabe;

  bilinear_pairing pairing_param;
  vector<group_h> sk0;// G2[klinear + 1]
  vector<group_g> sk1;// G1[klinear + 1
  map<string, vector<group_g>> sky;// G1[size_attributes]
};
}// end namespace crypto
}// end namespace odn

#endif //ODNCRYPTO_CPABE_SK_HPP
