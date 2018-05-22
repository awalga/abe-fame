//
// Created by abel walga on 02/05/2018.
//

#ifndef ODNCRYPTO_CPABE_PK_HPP
#define ODNCRYPTO_CPABE_PK_HPP

#include "bilinear_pairing.hpp"
#include <vector>

using namespace ::std;

namespace odn {
namespace crypto {

class cpabe_pk {

 public :

  cpabe_pk(bilinear_pairing pairing, group_h h_, vector<group_h> h_k_, vector<group_gt> t_k_) : pairing_parm(std::move(
      pairing)), h(std::move(h_)), h_k(std::move(h_k_)), t_k(std::move(t_k_)) {}

  bool is_valid() const {
    return h_k.size() == pairing_parm.klinear() && t_k.size() == pairing_parm.klinear();
  }

  ~cpabe_pk() = default;

 private:

  friend class odn_cpabe;
  /***********************************************
 * * 		O/I operations
 * *********************************************/
  template<typename T>
  friend T &serialize(T &, const cpabe_pk &);

  bilinear_pairing pairing_parm;
  group_h h;// G2
  vector<group_h> h_k;// G2[]
  vector<group_gt> t_k;// GT[]
};

template<typename T>
T &serialize(T &ar, const cpabe_pk &pk) {
  auto _array_ar = ar.array(4);

  _array_ar.push(pk.pairing_parm);
  _array_ar.push(pk.h);
  _array_ar.push(pk.h_k);
  _array_ar.push(pk.t_k);

  return ar;
}
}// end namespace crypto
}// end namespace odn

#include "odnfc/cbor.hpp"

namespace odn {
namespace fc {
using namespace ::odn::crypto;

template<typename T>
struct adl_serializer<T, cpabe_pk> {

  cpabe_pk operator()(T &ar) {
    auto _array_reader = ar.array();

    bilinear_pairing pairing(_array_reader.template pop<bilinear_pairing>());

    group_h h;// G2
    _array_reader.pop(&h);

    vector<group_h> h_k;// G2[]
    _array_reader.pop(&h_k);

    vector<group_gt> t_k;// GT[]
    _array_reader.pop(&t_k);

    return cpabe_pk(std::move(pairing),
                    std::move(h),
                    std::move(h_k),
                    std::move(t_k));
  }

};
}// end namespace fc
}// end namespace odn
#endif //ODNCRYPTO_CPABE_PK_HPP
