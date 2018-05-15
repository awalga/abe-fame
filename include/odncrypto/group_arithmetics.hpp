//
// Created by abel walga on 02/05/2018.
//

#ifndef ODNCRYPTO_GROUP_ARITHMETICS_HPP
#define ODNCRYPTO_GROUP_ARITHMETICS_HPP

namespace odn {
namespace crypto {

class field_zr;
class group_g;
class group_h;
class group_gt;

class group_arithmetics {
 public:
  static void mod_div(field_zr &quotient, const field_zr &num, const field_zr &den);
  static void sum(group_g &, const group_g &, const field_zr &);
  static void sum(group_h &, const group_h &, const field_zr &);
  static void product(group_gt &, const group_gt &, const field_zr &);
  static void pair(group_gt &, const group_g &, const group_h &);
};
}// end namespace crypto
}// end namespace odn
#endif //ODNCRYPTO_GROUP_ARITHMETICS_HPP
