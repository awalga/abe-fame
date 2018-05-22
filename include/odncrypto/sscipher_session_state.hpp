//
// Created by abel walga on 07/05/2018.
//

#ifndef ODNCRYPTO_SSCIPHER_SESSION_STATE_HPP
#define ODNCRYPTO_SSCIPHER_SESSION_STATE_HPP

#include "sodium_import.hpp"
#include <string>

namespace odn {
namespace crypto {
using namespace ::std;
constexpr static size_t HEADER_SIZE = crypto_secretstream_xchacha20poly1305_HEADERBYTES;
constexpr static size_t CIPHER_SIZE = crypto_secretstream_xchacha20poly1305_ABYTES;

struct sscipher_session_state {
  unsigned char header[HEADER_SIZE];
  crypto_secretstream_xchacha20poly1305_state state;

  /***********************************************
   * * 		O/I operations
   * *********************************************/
  template<typename T>
  friend T &serialize(T &, const sscipher_session_state &);
  template<typename T>
  friend T &deserialize(T &, sscipher_session_state &);
};

template<typename T>
T &serialize(T &ar, const sscipher_session_state &ssss) {
  string header_str(&ssss.header[0], &ssss.header[0] + HEADER_SIZE);
  ar.push(header_str);
  return ar;
}
template<typename T>
T &deserialize(T &ar, sscipher_session_state &ssss) {
  string header_str;
  ar.pop(&header_str);
  memcpy(ssss.header, header_str.c_str(), HEADER_SIZE);
  return ar;
}
}// end namespace crypto
}// end namespace odn
#endif //ODNCRYPTO_SSCIPHER_SESSION_STATE_HPP
