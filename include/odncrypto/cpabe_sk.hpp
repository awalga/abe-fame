//
// Created by abel walga on 02/05/2018.
//

#ifndef ODNCRYPTO_CPABE_SK_HPP
#define ODNCRYPTO_CPABE_SK_HPP

#include "bilinear_pairing.hpp"

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

  ~cpabe_sk() = default;

 private:

  friend class odn_cpabe;
/***********************************************
 * * 		O/I operations
 * *********************************************/
  template<typename T>
  friend T &serialize(T &, const cpabe_sk &);

  bilinear_pairing pairing_param;
  vector<group_h> sk0;// G2[klinear + 1]
  vector<group_g> sk1;// G1[klinear + 1
  map<string, vector<group_g>> sky;// G1[size_attributes]
};

template<typename T>
T &serialize(T &ar, const cpabe_sk &sk) {
  auto _array_ar = ar.array(4);

  _array_ar.push(sk.pairing_param);
  _array_ar.push(sk.sk0);
  _array_ar.push(sk.sk1);
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
struct adl_serializer<T, cpabe_sk> {

  cpabe_sk operator()(T &ar) {
    auto _array_reader = ar.array();

    bilinear_pairing pairing(_array_reader.template pop<bilinear_pairing>());

    vector<group_h> sk0;// G2[klinear + 1]
    _array_reader.pop(&sk0);

    vector<group_g> sk1;// G1[klinear + 1
    _array_reader.pop(&sk1);

    map<string, vector<group_g>> sky;// G1[size_attributes]
    _array_reader.pop(&sky);

    return cpabe_sk(std::move(pairing),
                    std::move(sk0),
                    std::move(sk1),
                    std::move(sky));
  }

};
}// end namespace fc
}// end namespace odn
#endif //ODNCRYPTO_CPABE_SK_HPP
