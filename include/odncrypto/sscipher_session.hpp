//
// Created by abel walga on 07/05/2018.
//
#ifndef ODNCRYPTO_SSCIPHER_SESSION_HPP
#define ODNCRYPTO_SSCIPHER_SESSION_HPP
#include "sscipher_session_key.hpp"
#include "sscipher_enc_stream.hpp"
#include "sscipher_dec_stream.hpp"

namespace odn {
namespace crypto {
class sscipher_session {

 public:

  sscipher_session() = delete;
  sscipher_session(const sscipher_session &) = delete;
  sscipher_session(sscipher_session
                   &&) = default;

  sscipher_session &operator=(const sscipher_session &) = delete;
  sscipher_session &operator=(sscipher_session &&) = default;

  virtual ~sscipher_session() {}

  sscipher_enc_stream enc_stream();
  sscipher_dec_stream dec_stream(const unsigned char *, const size_t);

  int is_valid() const {
    return valid;
  }

 private:
  friend class sscipher;
  // key
  const sscipher_session_key key;
  // initialized
  int valid = 0;
  // private constructor
  sscipher_session(const sscipher_session_key &key_) : key(key_) {}
};

}// end namespace crypto
}// end namespace odn
#endif //ODNCRYPTO_SSCIPHER_SESSION_HPP
