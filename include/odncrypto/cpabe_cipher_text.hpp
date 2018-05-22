//
// Created by abel walga on 02/05/2018.
//

#ifndef ODNCRYPTO_CPABE_CIPHER_TEXT_HPP
#define ODNCRYPTO_CPABE_CIPHER_TEXT_HPP

#include "bilinear_pairing.hpp"
#include "binary_policy_tree.hpp"

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

  ~cpabe_cipher_text() = default;
 private :
  friend class odn_cpabe;
  /***********************************************
   * * 		O/I operations
   * *********************************************/
  template<typename T>
  friend T &serialize(T &, const cpabe_cipher_text &);

  bilinear_pairing pairing_param;
  binary_policy_tree policy;
  vector<group_h> ct0;// G2[klinear]
  vector<group_g> cty;// cipher key components
  group_gt ct;// cipher secret
};
template<typename T>
T &serialize(T &ar, const cpabe_cipher_text &ct) {
  auto _array_ar = ar.array(5);

  _array_ar.push(ct.pairing_param);
  _array_ar.push(ct.policy);
  _array_ar.push(ct.ct0);
  _array_ar.push(ct.cty);
  _array_ar.push(ct.ct);

  return ar;
}
}// end namespace crypto
}// end namespace odn
#include "odnfc/cbor.hpp"

namespace odn {
namespace fc {
using namespace ::odn::crypto;

template<typename T>
struct adl_serializer<T, cpabe_cipher_text> {

  cpabe_cipher_text operator()(T &ar) {
    auto _array_reader = ar.array();

    bilinear_pairing pairing(_array_reader.template pop<bilinear_pairing>());
    binary_policy_tree policy(_array_reader.template pop<binary_policy_tree>());

    vector<group_h> ct0;
    _array_reader.pop(&ct0);
    vector<group_g> cty;// cipher key components
    _array_reader.pop(&cty);
    group_gt ct;// cipher secret
    _array_reader.pop(&ct);

    return cpabe_cipher_text(std::move(pairing), std::move(policy), std::move(ct0), std::move(cty), std::move(ct));
  }

};
}// end namespace fc
}// end namespace odn
#endif //ODNCRYPTO_CPABE_CIPHER_TEXT_HPP
