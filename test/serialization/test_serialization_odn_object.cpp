//
// Created by abel walga on 17/05/2018.
//
#include "gtest/gtest.h"
#include "odncrypto/bilinear_pairing.hpp"
#include "odncrypto/group.hpp"
#include "odnfc/cbor_parser.hpp"
#include "odnfc/cbor_encoder.hpp"
#include "odnfc/memory_buffer.hpp"
#include "odncrypto/relic_util.hpp"
#include "odncrypto/relic_import.h"

namespace odn {
namespace crypto {
using namespace ::odn::fc;

using cbor_reader_memory = ::odn::fc::cbor_reader_root<::odn::fc::memory_buffer>;
using cbor_writer_memory = ::odn::fc::cbor_writer_root<::odn::fc::memory_buffer>;

}// end namespace crypto
}// end namespace odn

TEST (serialization_crypto, serialize_field_Zr) {
  using namespace ::odn::fc;
  using namespace ::odn::crypto;
  bilinear_pairing pairing(2);

  field_zr zr0(group<field_zr>::zr().random()), zr1;
  memory_buffer buffer;
  cbor_writer_memory encoder(buffer);
  {
    encoder.push(zr0);
  }
  // decode
  cbor_reader_memory parser(buffer);
  {
    parser.pop(&zr1);
    EXPECT_EQ(zr0, zr1);
  }
}

TEST (serialization_crypto, serialize_field_g) {
  using namespace ::odn::fc;
  using namespace ::odn::crypto;
  bilinear_pairing pairing(2);

  group_g g0(group<group_g>::g().random()), g1;
  memory_buffer buffer;
  cbor_writer_memory encoder(buffer);
  {
    encoder.push(g0);
  }
  // decode
  cbor_reader_memory parser(buffer);
  {
    parser.pop(&g1);
    EXPECT_EQ(g0, g1);
  }
}

TEST (serialization_crypto, serialize_field_h) {
  using namespace ::odn::fc;
  using namespace ::odn::crypto;
  bilinear_pairing pairing(2);

  group_h h0(group<group_h>::h().random()), h1;
  memory_buffer buffer;
  cbor_writer_memory encoder(buffer);
  {
    encoder.push(h0);
  }
  // decode
  cbor_reader_memory parser(buffer);
  {
    parser.pop(&h1);
    EXPECT_EQ(h0, h1);
  }
}

TEST (serialization_crypto, serialize_field_gt) {
  using namespace ::odn::fc;
  using namespace ::odn::crypto;
  bilinear_pairing pairing(2);

  group_gt gt0(group<group_gt>::gt().random()), gt1;
  memory_buffer buffer;
  cbor_writer_memory encoder(buffer);
  {
    encoder.push(gt0);
  }
  // decode
  cbor_reader_memory parser(buffer);
  {
    parser.pop(&gt1);
    EXPECT_EQ(gt0, gt1);
  }
}

TEST (serialization_crypto, serialize_bilinear_pairing) {
  using namespace ::odn::fc;
  using namespace ::odn::crypto;
  bilinear_pairing pairing(2);

  memory_buffer buffer;
  cbor_writer_memory encoder(buffer);
  {
    encoder.push(pairing);
  }
  // decode
  cbor_reader_memory parser(buffer);
  {
    bilinear_pairing pairing_decoded = parser.pop<bilinear_pairing>();

    EXPECT_EQ(pairing_decoded, pairing);
  }
}

#include "odncrypto/binary_policy_tree.hpp"

TEST (serialization_crypto, serialize_policy) {
  using namespace ::odn::crypto;

  memory_buffer buffer;
  cbor_writer_memory encoder(buffer);
  {
    binary_policy_tree policy(NodeType::OR);

    auto *orNode = &policy.root();
    orNode->addLeft(policy, NodeType::LEAF, "A");
    orNode->addRight(policy, NodeType::LEAF, "B");
    encoder.push(policy);
  }
  // decode
  cbor_reader_memory parser(buffer);
  {
    binary_policy_tree policy_decoded = parser.pop<binary_policy_tree>();

    auto *orNode = &policy_decoded.root();

    EXPECT_EQ(orNode->treshold(), NodeType::OR);

    EXPECT_EQ(orNode->left(policy_decoded).attribute(), "A");
    EXPECT_EQ(orNode->left(policy_decoded).treshold(), NodeType::LEAF);
    EXPECT_EQ(orNode->left(policy_decoded).parent(policy_decoded).treshold(), NodeType::OR);

    EXPECT_EQ(orNode->right(policy_decoded).attribute(), "B");
    EXPECT_EQ(orNode->right(policy_decoded).treshold(), NodeType::LEAF);
    EXPECT_EQ(orNode->right(policy_decoded).parent(policy_decoded).treshold(), NodeType::OR);
  }
}

#include "odncrypto/odn_abe.hpp"

TEST (serialization_crypto, serialize_msk) {
  using namespace ::odn::crypto;

  memory_buffer buffer_msk;
  cbor_writer_memory encoder_msk(buffer_msk);
  {
    auto msk = odn_abe::generate_authority(2);
    encoder_msk.push(msk);
  }
  memory_buffer buffer_pk;
  cbor_writer_memory encoder_pk(buffer_pk);
  cbor_reader_memory decoder_msk(buffer_msk);
  {
    // deserialize msk and create pk
    auto msk_decoded = decoder_msk.pop<cpabe_msk>();
    auto pk = odn_abe::generate_pk(msk_decoded);
    encoder_pk.push(pk);
  }
  cbor_reader_memory decoder_pk(buffer_pk);
  memory_buffer buffer_ct;
  cbor_writer_memory encoder_ct(buffer_ct);
  {
    // deserialize pk and encrypt data
    auto pk_decoded = decoder_pk.pop<cpabe_pk>();

    // (A | C) & B
    binary_policy_tree policy(NodeType::AND);
    policy.root().addLeft<NodeType::OR>(policy);
    policy.root().addLeft<NodeType::OR>(policy).addLeft(policy, NodeType::LEAF, "A");
    policy.root().addLeft<NodeType::OR>(policy).addRight(policy, NodeType::LEAF, "C");
    policy.root().addRight(policy, NodeType::LEAF, "B");

    auto encrypt_session = odn_abe::encrypt_session(pk_decoded, policy);

    auto encrypt = encrypt_session.first.enc_stream();
    uint8_t clear_text[] = {'s', 'e', 'c', 'r', 'e', 't', '0'};
    uint8_t cipher_text[7 + CIPHER_SIZE];
    size_t cipher_size = 0;
    encrypt(clear_text, 7, cipher_text, cipher_size);

    auto encoder_ct_array = encoder_ct.array(4);

    // encode cpabe_ct, cipher_text_size, ciper_text
    encoder_ct_array.push(encrypt_session.second);// cpabe_ct
    encoder_ct_array.push(cipher_size);// cipher_size
    // encode as string because of variable array size : template does not accept on pop variable length native array
    encoder_ct_array.push(std::string(&cipher_text[0], &(cipher_text[0]) + cipher_size));
    encoder_ct_array.push(encrypt.state());
  }

  memory_buffer buffer_sk;
  cbor_writer_memory encoder_sk(buffer_sk);
  cbor_reader_memory decoder_msk_second(buffer_msk);
  {
    // deserialize msk and create sk
    auto msk_decoded = decoder_msk_second.pop<cpabe_msk>();
    auto sk = odn_abe::generate_sk(msk_decoded, set<string>{"A", "B", "C"});
    encoder_sk.push(sk);
  }

  cbor_reader_memory decoder_sk(buffer_sk);
  cbor_reader_memory decoder_ct(buffer_ct);
  {
    // deserialize sk and decrypt data
    cpabe_sk sk_decoded = decoder_sk.pop<cpabe_sk>();

    auto decoder_ct_array = decoder_ct.array();

    auto cpabe_ct_decoded = decoder_ct_array.pop<cpabe_cipher_text>();
    auto cipher_text_size_decoded = decoder_ct_array.pop<size_t>();
    std::string cipher_text_str_decoded = decoder_ct_array.pop<string>();

    auto cipher_state_decoded = decoder_ct_array.pop<sscipher_session_state>();

    auto decrypt_session = odn_abe::decrypt_session(sk_decoded, cpabe_ct_decoded);
    auto decrypt = decrypt_session.dec_stream(cipher_state_decoded.header, HEADER_SIZE);

    uint8_t clear_text[cipher_text_str_decoded.length()];
    size_t clear_text_size = 0;
    uint8_t cipher_text_decoded[cipher_text_str_decoded.length()];
    memcpy(cipher_text_decoded, cipher_text_str_decoded.c_str(), cipher_text_str_decoded.length());

    EXPECT_EQ(decrypt(clear_text, clear_text_size, cipher_text_decoded, cipher_text_size_decoded), 0);
    EXPECT_EQ(clear_text_size, 7);

    // compare elemnt wise
    uint8_t clear_text_origin[] = {'s', 'e', 'c', 'r', 'e', 't', '0'};
    for (size_t i = 0; i < 7; ++i) {
      EXPECT_EQ(clear_text[i], clear_text_origin[i]);
    }
  }

}