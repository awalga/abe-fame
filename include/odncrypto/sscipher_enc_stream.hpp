//
// Created by abel walga on 07/05/2018.
//

#ifndef ODNCRYPTO_SSCIPHER_ENC_STREAM_HPP
#define ODNCRYPTO_SSCIPHER_ENC_STREAM_HPP
#include "sscipher_session_state.hpp"

namespace odn {
namespace crypto {

class sscipher_enc_stream {

 public :
  sscipher_enc_stream() = delete;
  sscipher_enc_stream(const sscipher_enc_stream &) = delete;
  sscipher_enc_stream(sscipher_enc_stream &&) = default;

  sscipher_enc_stream &operator=(const sscipher_enc_stream &) = delete;
  sscipher_enc_stream &operator=(sscipher_enc_stream &&) = default;

  int operator()(const uint8_t *, size_t, uint8_t *, size_t &);

  int is_valid() const {
    return valid;
  }

  void set_valid(int valid_) {
    valid = valid_;
  }

  const unsigned char *const header() const {
    return session_state.header;
  }

  const sscipher_session_state &state() const {
    return session_state;
  }

 private:
  friend class sscipher_session;
  // session state
  sscipher_session_state session_state;
  // valid stream
  int valid = 0;
  // private ctor
  sscipher_enc_stream(sscipher_session_state &&session_state_) : session_state(session_state_) {}
};
}// end namespace crypto
}// end namespace odn
#endif //ODNCRYPTO_SSCIPHER_ENC_STREAM_HPP
