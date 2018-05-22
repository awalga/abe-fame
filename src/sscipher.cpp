//
// Created by abel walga on 23/04/2018.
//
#include "odncrypto/sscipher.hpp"
#include "odncrypto/sscipher_session_key.hpp"
#include "odncrypto/group.hpp"
#include "odnfc/memory_buffer.hpp"
#include "odnfc/cbor_encoder.hpp"

using namespace ::std;
using namespace ::odn::crypto;

sscipher_session sscipher::session(const unsigned char *key, unsigned long long keylen) {
  sscipher_session_key sessionKey;

  if (sodium_init() != -1 && crypto_generichash(sessionKey.data, KEY_SIZE, key, keylen, NULL, 0) == 0) {

    sscipher_session sscs(sessionKey);
    sscs.valid = 0;

    return sscs;
  } else {

    sscipher_session sscs(sessionKey);
    sscs.valid = 1;

    return sscs;
  }
}

sscipher_session sscipher::session(const odn::crypto::group_gt &secret,
                                   const odn::crypto::cpabe_cipher_text &cpabect) {
  sscipher_session_key sessionKey;
  using namespace ::odn::fc;

  memory_buffer sec_buffer(group<group_gt>::size());
  cbor_writer_root<::odn::fc::memory_buffer> encoder(sec_buffer);
  serialize<decltype(encoder)>(encoder, secret);

  if (sodium_init() != -1
      && crypto_generichash(sessionKey.data, KEY_SIZE, sec_buffer.begin(),
                            static_cast<size_t >(sec_buffer.end() - sec_buffer.begin()), NULL, 0) == 0) {

    sscipher_session sscs(sessionKey);
    sscs.valid = 0;

    return sscs;
  } else {

    sscipher_session sscs(sessionKey);
    sscs.valid = 1;

    return sscs;
  }
}