//
// Created by abel walga on 07/05/2018.
//

#include "odncrypto/sscipher_session.hpp"
#include "sscipher_enc_stream.cpp"
#include "sscipher_dec_stream.cpp"
#include "odncrypto/sodium_import.hpp"

#include <utility>                                // std::move
#include <algorithm>                              // std::copy

using namespace ::std;
using namespace ::odn::crypto;

sscipher_enc_stream sscipher_session::enc_stream() {
  sscipher_session_state state;
  /* Set up a new stream: initialize the state and create the header */
  if (crypto_secretstream_xchacha20poly1305_init_push(&state.state, state.header, key.data) != 0) {

    sscipher_enc_stream encryption(std::move(state));
    encryption.set_valid(1);

    return encryption;
  } else {

    sscipher_enc_stream encryption(std::move(state));
    encryption.set_valid(0);

    return encryption;
  }
}

sscipher_dec_stream sscipher_session::dec_stream(const unsigned char *header, const size_t len) {
  // TODO check length matches header size
  sscipher_session_state state;
  std::copy(header, header + len, state.header);

  if (crypto_secretstream_xchacha20poly1305_init_pull(&state.state, state.header, key.data) != 0) {

    sscipher_dec_stream decryption(std::move(state));
    decryption.set_valid(1);

    return decryption;
  } else {

    sscipher_dec_stream decryption(std::move(state));
    decryption.set_valid(0);

    return decryption;
  }
}