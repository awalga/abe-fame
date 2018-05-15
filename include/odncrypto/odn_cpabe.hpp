//
// Created by abel walga on 02/05/2018.
//

#ifndef ODNCRYPTO_ODNCPABE_HPP
#define ODNCRYPTO_ODNCPABE_HPP
#include "cpabe_msk.hpp"
#include "cpabe_pk.hpp"
#include "cpabe_sk.hpp"
#include "cpabe_cipher_text.hpp"
#include "binary_policy_tree.hpp"
#include "sscipher_session.hpp"

#include <set>

namespace odn {
namespace crypto {
class odn_cpabe {

 public :
  static const cpabe_msk generate_authority(size_t klinear);

  static const cpabe_pk generate_pk(const cpabe_msk &msk);

  static cpabe_sk generate_sk(const cpabe_msk &msk, const std::set<string> attributes);

  static std::pair<sscipher_session, cpabe_cipher_text> encrypt_session(const cpabe_pk &, const binary_policy_tree &);

  static sscipher_session decrypt_session(const cpabe_sk &, const cpabe_cipher_text &);
};
}// end namespace crypto
}// end namespace odn
#endif //ODNCRYPTO_ODNCPABE_HPP
