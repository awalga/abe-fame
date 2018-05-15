//
// Created by abel walga on 07/05/2018.
//

#ifndef ODNCRYPTO_SSCIPHER_SESSION_STATE_HPP
#define ODNCRYPTO_SSCIPHER_SESSION_STATE_HPP

#include "sodium_import.hpp"

namespace odn {
namespace crypto {

constexpr static size_t HEADER_SIZE = crypto_secretstream_xchacha20poly1305_HEADERBYTES;
constexpr static size_t CIPHER_SIZE = crypto_secretstream_xchacha20poly1305_ABYTES;

struct sscipher_session_state {
  unsigned char header[HEADER_SIZE];
  crypto_secretstream_xchacha20poly1305_state state;
};
}// end namespace crypto
}// end namespace odn
#endif //ODNCRYPTO_SSCIPHER_SESSION_STATE_HPP
