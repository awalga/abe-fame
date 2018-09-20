//
// Created by abel walga on 23/05/2018.
//

#ifndef ODNFC_KPABE_CIPHER_TEXT_HPP
#define ODNFC_KPABE_CIPHER_TEXT_HPP

#include "bilinear_pairing.hpp"

#include <string>
#include <set>
#include <map>
#include <vector>

namespace odn {
namespace crypto {
class kpabe_cipher_text {

 public :

  kpabe_cipher_text(bilinear_pairing pairing_param_,
                    set<string> attributes_,
                    vector<group_h> ct0_,
                    map<string, vector<group_g>> cty_,
                    group_gt ct_) : pairing_param(std::move(pairing_param_)),
                                    attributes(std::move(attributes_)),
                                    ct0(std::move(ct0_)),
                                    cty(std::move(cty_)),
                                    ct(std::move(ct_)) {}

  bool is_valid() const {

    return ct0.size() == (pairing_param.klinear() + 1) && cty.size() == attributes.size();

 }

  ~kpabe_cipher_text() = default;

 private:

  friend class odn_abe;
  /***********************************************
   * * 		O/I operations
   * *********************************************/
  template<typename T>
  friend T &serialize(T &, const kpabe_cipher_text &);

  bilinear_pairing pairing_param;
  set<string> attributes;
  vector<group_h> ct0;// G2[klinear]
  map<string, vector<group_g>> cty;// cipher key components
  group_gt ct;// cipher secret
};

template<typename T>
T &serialize(T &ar, const kpabe_cipher_text &ct) {
  auto _array_ar = ar.array(5);

  _array_ar.push(ct.pairing_param);
  _array_ar.push(ct.attributes);
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
struct adl_serializer<T, kpabe_cipher_text> {

  kpabe_cipher_text operator()(T &ar) {
    auto _array_reader = ar.array();

    bilinear_pairing pairing(_array_reader.template pop<bilinear_pairing>());
    set<string> attributes;
    _array_reader.pop(&attributes);

    vector<group_h> ct0;// G2[klinear]
    _array_reader.pop(&ct0);

    map<string, vector<group_g>> cty;// cipher key components
    _array_reader.pop(&cty);

    group_gt ct;// cipher secret
    _array_reader.pop(&ct);

    return kpabe_cipher_text(std::move(pairing), std::move(attributes), std::move(ct0), std::move(cty), std::move(ct));
  }

};
}// end namespace fc
}// end namespace odn
#endif //ODNFC_KPABE_CIPHER_TEXT_HPP
