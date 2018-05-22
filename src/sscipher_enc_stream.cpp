//
// Created by abel walga on 07/05/2018.
//

#include "odncrypto/sscipher_enc_stream.hpp"

using namespace ::std;
using namespace ::odn::crypto;

int sscipher_enc_stream::operator()(const uint8_t *msg, size_t msglen, uint8_t *buffer, size_t &len) {

  unsigned long long int size = 0;

  if (crypto_secretstream_xchacha20poly1305_push(&session_state.state, buffer, &size, msg, msglen, NULL, 0, 0) != 0) {

    len = size;
    valid = 1;
    return 1;
  } else {

    len = size;
    valid = 0;
    return 0;
  }
}
