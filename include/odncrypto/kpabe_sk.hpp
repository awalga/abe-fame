//
// Created by abel walga on 23/05/2018.
//

#ifndef ODNFC_KPABE_SK_HPP
#define ODNFC_KPABE_SK_HPP

#include "bilinear_pairing.hpp"
#include "binary_policy_tree.hpp"

#include <vector>

namespace odn {
namespace crypto {

class kpabe_sk {

 public :

  kpabe_sk() = delete;
  kpabe_sk(bilinear_pairing pairing_param_,
           binary_policy_tree policy_,
           vector<group_h> sk0_,
           vector<group_g> sky_)
      : pairing_param(std::move(pairing_param_)), policy(policy_), sk0(std::move(sk0_)), sky(std::move(sky_)) {}

  bool is_valid() const {

    return sk0.size() == (pairing_param.klinear() + 1)
        && sky.size() == policy.attributes().size() * (pairing_param.klinear() + 1);

  }

  ~kpabe_sk() = default;

 private :

  friend class odn_abe;

  /***********************************************
   * * * 		O/I operations
   ***********************************************/
  template<typename T>
  friend T &serialize(T &, const kpabe_sk &);

  bilinear_pairing pairing_param;
  binary_policy_tree policy;

  vector<group_h> sk0;// G2[klinear + 1]
  vector<group_g> sky;// G1[size_attributes]
};

template<typename T>
T &serialize(T &ar, const kpabe_sk &sk) {
  auto _array_ar = ar.array(4);

  _array_ar.push(sk.pairing_param);
  _array_ar.push(sk.policy);
  _array_ar.push(sk.sk0);
  _array_ar.push(sk.sky);

  return ar;
}
}// end namespace crypto
}// end namespace odn
#include "odnfc/cbor.hpp"

namespace odn {
namespace fc {
using namespace ::odn::crypto;

template<typename T>
struct adl_serializer<T, kpabe_sk> {

  kpabe_sk operator()(T &ar) {
    auto _array_reader = ar.array();

    bilinear_pairing pairing(_array_reader.template pop<bilinear_pairing>());
    binary_policy_tree policy(_array_reader.template pop<binary_policy_tree>());

    vector<group_h> sk0;
    _array_reader.pop(&sk0);
    vector<group_g> sky;// cipher key components
    _array_reader.pop(&sky);

    return kpabe_sk(std::move(pairing), std::move(policy), std::move(sk0), std::move(sky));
  }

};
}// end namespace fc
}// end namespace odn
#endif //ODNFC_KPABE_SK_HPP
