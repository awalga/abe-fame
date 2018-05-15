//
// Created by abel walga on 07/05/2018.
//

#ifndef ODNCRYPTO_SSCIPHER_DEC_STREAM_HPP
#define ODNCRYPTO_SSCIPHER_DEC_STREAM_HPP
#include "sscipher_session_state.hpp"

namespace odn {
namespace crypto {

class sscipher_dec_stream {

 public :
  sscipher_dec_stream() = delete;
  sscipher_dec_stream(const sscipher_dec_stream &) = delete;
  sscipher_dec_stream(sscipher_dec_stream &&) = default;

  sscipher_dec_stream &operator=(const sscipher_dec_stream &) = delete;
  sscipher_dec_stream &operator=(sscipher_dec_stream &&) = default;

  int operator()(uint8_t *, size_t&, const uint8_t *, size_t);

  int is_valid() const {
    return valid;
  }

  void set_valid(int valid_) {
    valid = valid_;
  }

 private:
  friend class sscipher_session;
// session state
  sscipher_session_state state;
// valid stream
  int valid;
// private ctor
  sscipher_dec_stream(sscipher_session_state &&state_) : state(state_) {}
};
}// end namespace crypto
}// end namespace odn
#endif //ODNCRYPTO_SSCIPHER_DEC_STREAM_HPP
