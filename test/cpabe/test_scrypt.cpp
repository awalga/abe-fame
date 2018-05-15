//
// Created by abel walga on 06/05/2018.
//
#include "gtest/gtest.h"

#include "odncrypto/sscipher.hpp"

TEST (sscipher, create_session) {
  using namespace ::odn::crypto;

  const unsigned char password[] = {'m', 'y', 'p', 'a', 's', 's', 'w', 'o', 'r', 'd'};
  auto session = sscipher::session(password, 10);

  EXPECT_EQ(session.is_valid(), 0);
}

TEST (sscipher, ss_encrypt) {
  using namespace ::odn::crypto;

  const unsigned char password[] = {'m', 'y', 'p', 'a', 's', 's', 'w', 'o', 'r', 'd'};
  sscipher_session session = sscipher::session(password, 10);

  sscipher_enc_stream encrypt = session.enc_stream();
  EXPECT_EQ(encrypt.is_valid(), 0);

  uint8_t text0[] = {'s', 'e', 'c', 'r', 'e', 't', '0'};
  uint8_t ciph0[7 + CIPHER_SIZE];
  size_t cipher0_size = 0;

  EXPECT_EQ(encrypt(text0, 7, ciph0, cipher0_size), 0);

  uint8_t text1[] = {'s', 'e', 'c', 'r', 'e', 't', '1'};
  uint8_t ciph1[7 + CIPHER_SIZE];
  size_t cipher1_size = 0;

  EXPECT_EQ(encrypt(text1, 7, ciph1, cipher1_size), 0);
  // TODO close
}

TEST (sscipher, ss_decrypt) {
  using namespace ::odn::crypto;

  const unsigned char password[] = {'m', 'y', 'p', 'a', 's', 's', 'w', 'o', 'r', 'd'};
  sscipher_session session = sscipher::session(password, 10);

  sscipher_enc_stream encrypt = session.enc_stream();

  // encrypt two messages
  uint8_t text0[] = {'s', 'e', 'c', 'r', 'e', 't', '0'};
  uint8_t cipher0[7 + CIPHER_SIZE];
  size_t cipher0_size = 0;
  encrypt(text0, 7, cipher0, cipher0_size);

  uint8_t text1[] = {'s', 'e', 'c', 'r', 'e', 't', '1'};
  uint8_t cipher1[7 + CIPHER_SIZE];
  size_t cipher1_size = 0;
  encrypt(text1, 7, cipher1, cipher1_size);

  sscipher_dec_stream decrypt = session.dec_stream(encrypt.header(), HEADER_SIZE);
  EXPECT_EQ(decrypt.is_valid(), 0);

  uint8_t rev_text0[7];
  size_t rev_text0_size = 0;
  EXPECT_EQ(decrypt(rev_text0, rev_text0_size, cipher0, cipher0_size), 0);
  EXPECT_EQ(rev_text0_size, 7);
  // compare element wise
  for (size_t i = 0; i < rev_text0_size; ++i) {
    EXPECT_EQ(text0[i], rev_text0[i]);
  }

  uint8_t rev_text1[7];
  size_t rev_text1_size = 0;
  EXPECT_EQ(decrypt(rev_text1, rev_text1_size, cipher1, cipher1_size), 0);
  EXPECT_EQ(rev_text1_size, 7);

  // compare element wise
  for (size_t i = 0; i < rev_text1_size; ++i) {
    EXPECT_EQ(text1[i], rev_text1[i]);
  }
}
