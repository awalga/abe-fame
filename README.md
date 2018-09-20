# abe-fame

An implementation of CP-ABE and KP-ABE based on [Fast Attribute-based Message Encryption](https://eprint.iacr.org/2017/807.pdf)

# usage

## Cipher Policy ABE

```
 using namespace ::odn::crypto;

  auto msk = odn_abe::generate_authority(2);
  auto pk = odn_abe::generate_pk(msk);
  auto sk = odn_abe::generate_sk(msk, set<string>{"A", "B", "C"});

  // (A | C) & B
  binary_policy_tree policy(NodeType::AND);
  auto &orPolicy = policy.root().addLeft<NodeType::OR>(policy);
  policy.root().addRight(policy, NodeType::LEAF, "B");
  orPolicy.addLeft(policy, NodeType::LEAF, "A");
  orPolicy.addRight(policy, NodeType::LEAF, "C");

  auto encrypt_session = odn_abe::encrypt_session(pk, policy);
  EXPECT_EQ(encrypt_session.first.is_valid(), 0);

  auto encrypt = encrypt_session.first.enc_stream();
  // encrypt with random key
  uint8_t text0[] = {'s', 'e', 'c', 'r', 'e', 't', '0'};
  uint8_t cipher0[7 + CIPHER_SIZE];
  size_t cipher0_size = 0;
  EXPECT_EQ(encrypt(text0, 7, cipher0, cipher0_size), 0);

  auto decrypt_session = odn_abe::decrypt_session(sk, encrypt_session.second);
  EXPECT_EQ(decrypt_session.is_valid(), 0);

  auto decrypt = decrypt_session.dec_stream(encrypt.header(), HEADER_SIZE);

  uint8_t rev_text0[7];
  size_t rev_text0_size = 0;
  EXPECT_EQ(decrypt(rev_text0, rev_text0_size, cipher0, cipher0_size), 0);
  EXPECT_EQ(rev_text0_size, 7);

  // compare elemnt wise
  for (size_t i = 0; i < rev_text0_size; ++i) {
    EXPECT_EQ(text0[i], rev_text0[i]);
  }
```

## Key Policy ABE

```
 using namespace ::odn::crypto;

  auto msk = odn_abe::generate_authority(2);
  auto pk = odn_abe::generate_pk(msk);
  // (A | C) & B
  binary_policy_tree policy(NodeType::AND);
  policy.root().addLeft<NodeType::OR>(policy).addLeft(policy, NodeType::LEAF, "A");
  policy.root().addLeft<NodeType::OR>(policy).addRight(policy, NodeType::LEAF, "C");
  policy.root().addRight(policy, NodeType::LEAF, "B");
  // policy.root().addLeft(policy, NodeType::LEAF, "A");
  // policy.root().addRight(policy, NodeType::LEAF, "B");

  auto sk = odn_abe::generate_sk(msk, policy);

  auto encrypt_session = odn_abe::encrypt_session(pk, set<string>{"B", "C"});
  EXPECT_EQ(encrypt_session.first.is_valid(), 0);

  auto encrypt = encrypt_session.first.enc_stream();
  // encrypt with random key
  uint8_t text0[] = {'s', 'e', 'c', 'r', 'e', 't', '0'};
  uint8_t cipher0[7 + CIPHER_SIZE];
  size_t cipher0_size = 0;
  EXPECT_EQ(encrypt(text0, 7, cipher0, cipher0_size), 0);

  auto decrypt_session = odn_abe::decrypt_session(sk, encrypt_session.second);
  EXPECT_EQ(decrypt_session.is_valid(), 0);

  auto decrypt = decrypt_session.dec_stream(encrypt.header(), HEADER_SIZE);

  uint8_t rev_text0[7];
  size_t rev_text0_size = 0;
  EXPECT_EQ(decrypt(rev_text0, rev_text0_size, cipher0, cipher0_size), 0);
  EXPECT_EQ(rev_text0_size, 7);

  // compare elemnt wise
  for (size_t i = 0; i < rev_text0_size; ++i) {
    EXPECT_EQ(text0[i], rev_text0[i]);
  }


# Disclaimer

This is not a production grade code. Implementations may not be correct or secure and may include patented algorithms.

# License
MIT
