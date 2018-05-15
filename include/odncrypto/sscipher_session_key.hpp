//
// Created by abel walga on 07/05/2018.
//

#ifndef ODNCRYPTO_SSCIPHER_SESSION_KEY_HPP
#define ODNCRYPTO_SSCIPHER_SESSION_KEY_HPP
#include "sodium_import.hpp"

namespace odn {
namespace crypto {

constexpr static size_t KEY_SIZE = crypto_secretstream_xchacha20poly1305_KEYBYTES;

struct sscipher_session_key {
  unsigned char data[KEY_SIZE];
};

}// end namespace crypto
}// end namespace odn
#endif //ODNCRYPTO_SSCIPHER_SESSION_KEY_HPP
