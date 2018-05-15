//
// Created by abel walga on 02/05/2018.
//

#ifndef ODNCRYPTO_CPABE_CIPHER_TEXT_HPP
#define ODNCRYPTO_CPABE_CIPHER_TEXT_HPP

#include "bilinear_pairing.hpp"
#include "binary_policy_tree.hpp"
#include "group_g.hpp"
#include "group_h.hpp"
#include "group_gt.hpp"

#include <memory>                             // std::unique_ptr
#include <vector>

using namespace ::std;
namespace odn {
namespace crypto {

class cpabe_cipher_text {

 public :
  cpabe_cipher_text(bilinear_pairing pairing_param_,
                    binary_policy_tree policy_,
                    vector<group_h> ct0_,
                    vector<group_g> cty_,
                    group_gt ct_)
      : pairing_param(std::move(pairing_param_)),
        policy(std::move(policy_)),
        ct0(std::move(ct0_)),
        cty(std::move(cty_)),
        ct(std::move(ct_)) {
  }

  ~cpabe_cipher_text() {}

 private :
  friend class odn_cpabe;

  bilinear_pairing pairing_param;
  binary_policy_tree policy;
  vector<group_h> ct0;// G2[klinear]
  vector<group_g> cty;// cipher key components
  group_gt ct;// cipher secret
};

}// end namespace crypto
}// end namespace odn
#endif //ODNCRYPTO_CPABE_CIPHER_TEXT_HPP
