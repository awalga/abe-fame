//
// Created by abel walga on 23/04/2018.
//

#ifndef ODNCRYPTO_SSCIPHER_HPP
#define ODNCRYPTO_SSCIPHER_HPP

#include "sscipher_session.hpp"
#include "group_gt.hpp"
#include "cpabe_cipher_text.hpp"

namespace odn {
namespace crypto {

class sscipher {
 public :
  static sscipher_session session(const unsigned char *, unsigned long long);
  static sscipher_session session(const group_gt &, const cpabe_cipher_text &);
};

} // end namespace crypto
} // end namespace odn
#endif //ODNCRYPTO_SSCIPHER_HPP
