//
// Created by abel walga on 23/04/2018.
//
#include "odncrypto/sscipher.hpp"
#include "odncrypto/sscipher_session_key.hpp"

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
  // sessionKey.ct = cpabect;

  uint8_t sec_buffer[group<group_gt>::size()];// TODO destroy sec buffer
  group<group_gt>::write(secret, sec_buffer, group<group_gt>::size());

  if (sodium_init() != -1
      && crypto_generichash(sessionKey.data, KEY_SIZE, sec_buffer, group<group_gt>::size(), NULL, 0) == 0) {

    sscipher_session sscs(sessionKey);
    sscs.valid = 0;

    return sscs;
  } else {

    sscipher_session sscs(sessionKey);
    sscs.valid = 1;

    return sscs;
  }
}