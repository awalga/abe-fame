//
// Created by abel walga on 02/05/2018.
//

#ifndef ODNCRYPTO_ODNABE_HPP
#define ODNCRYPTO_ODNABE_HPP
#include "cpabe_msk.hpp"
#include "cpabe_pk.hpp"
#include "cpabe_sk.hpp"
#include "kpabe_sk.hpp"
#include "kpabe_cipher_text.hpp"
#include "cpabe_cipher_text.hpp"
#include "binary_policy_tree.hpp"
#include "sscipher_session.hpp"

#include <set>

namespace odn {
namespace crypto {
class odn_abe {

 public :
  static const cpabe_msk generate_authority(size_t klinear);

  static const cpabe_pk generate_pk(const cpabe_msk &msk);

  static cpabe_sk generate_sk(const cpabe_msk &msk, const std::set<string> attributes);
  static kpabe_sk generate_sk(const cpabe_msk &msk, const binary_policy_tree &policy);

  static std::pair<sscipher_session, cpabe_cipher_text> encrypt_session(const cpabe_pk &, const binary_policy_tree &);
  static std::pair<sscipher_session, kpabe_cipher_text> encrypt_session(const cpabe_pk &, const std::set<string>);

  static sscipher_session decrypt_session(const cpabe_sk &, const cpabe_cipher_text &);
  static sscipher_session decrypt_session(const kpabe_sk &, const kpabe_cipher_text &);
};
}// end namespace crypto
}// end namespace odn
#endif //ODNCRYPTO_ODNABE_HPP
