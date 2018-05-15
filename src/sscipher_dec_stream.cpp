//
// Created by abel walga on 07/05/2018.
//

#include "odncrypto/sscipher_dec_stream.hpp"

using namespace ::std;
using namespace ::odn::crypto;

int sscipher_dec_stream::operator()(uint8_t *buffer, size_t &len, const uint8_t *msg, size_t msglen) {

  unsigned long long int clen = 0;
  unsigned char tag = 0;
  if (crypto_secretstream_xchacha20poly1305_pull
      (&state.state, buffer, &clen, &tag, msg, msglen, NULL, 0) != 0) {

    len = clen;
    valid = 1;
    return 1;
  } else {

    len = clen;
    valid = 0;
    return 0;
  }
}