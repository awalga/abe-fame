//
// Created by abel walga on 02/05/2018.
//

#include "gtest/gtest.h"
#include "odncrypto/bilinear_pairing.hpp"

TEST (group_arithmetics, field_Zr_initialization) {
  using namespace ::odn::crypto;
  bilinear_pairing pairing(2);

  field_zr a = group<field_zr>::zr();
  a.random();

  field_zr b{a};
  EXPECT_EQ(b, a);

  field_zr c{a.crandom()};
  EXPECT_EQ(b, a);

  field_zr b1 = b;
  EXPECT_EQ(b1, a);

  field_zr c1(std::move(a));
  EXPECT_EQ(b1, c1);

  field_zr neg_map = group<field_zr>::zr_map(-1);

  EXPECT_EQ(neg_map + group<field_zr>::zr_map(1), group<field_zr>::zr_zero() );
}

TEST (group_arithmetics, field_Zr_addition) {
  using namespace ::odn::crypto;
  bilinear_pairing pairing(2);

  field_zr a = group<field_zr>::zr().crandom();
  field_zr b = group<field_zr>::zr_zero();

  field_zr c = a + b;
  EXPECT_EQ(c, a);

  b = group<field_zr>::zr().crandom();
  c = a + b;
  EXPECT_EQ(c - b, a);

  b = a;
  EXPECT_EQ((-a) + b, group<field_zr>::zr_zero());
}

TEST (group_arithmetics, field_Zr_multiplication) {
  using namespace ::odn::crypto;
  bilinear_pairing pairing(2);

  field_zr a = group<field_zr>::zr().crandom();
  field_zr b = group<field_zr>::zr_unity();

  field_zr c = a * b;
  EXPECT_EQ(c, a);

  b = group<field_zr>::zr().crandom();
  c = a * b;
  EXPECT_EQ(c / b, a);

  EXPECT_EQ(b / b, group<field_zr>::zr_unity());
}

TEST (group_arithmetics, group_g_initialization) {
  using namespace ::odn::crypto;
  bilinear_pairing pairing(2);

  group_g a = group<group_g>::g();

  group_g b{a};
  EXPECT_EQ(b, a);

  group_g c{b};
  EXPECT_EQ(c, a);

  group_g c1(std::move(a));
  EXPECT_EQ(c1, c);
}

TEST (group_arithmetics, group_g_addition) {
  using namespace ::odn::crypto;
  bilinear_pairing pairing(2);

  group_g a = group<group_g>::g().crandom();
  group_g b = group<group_g>::g_zero();

  group_g c = a + b;
  EXPECT_EQ(c, a);

  b = group<group_g>::g().crandom();
  c = a + b;
  EXPECT_EQ(c - b, a);

  b = a;
  EXPECT_EQ((-a) + b, group<group_g>::g_zero());

  field_zr pow_zero = group<field_zr>::zr_unity();
  EXPECT_EQ(b ^ pow_zero, b);

  field_zr pow_2 = group<field_zr>::zr_map(2);
  EXPECT_EQ(b ^ pow_2, b + b);
}

TEST (group_arithmetics, group_h_initialization) {
  using namespace ::odn::crypto;
  bilinear_pairing pairing(2);

  group_h a = group<group_h>::h();

  group_h b{a};
  EXPECT_EQ(b, a);

  group_h c{b};
  EXPECT_EQ(c, a);

  group_h c1(std::move(a));
  EXPECT_EQ(c1, c);
}

TEST (group_arithmetics, group_h_addition) {
  using namespace ::odn::crypto;
  bilinear_pairing pairing(2);

  group_h a = group<group_h>::h().crandom();
  group_h b = group<group_h>::h_zero();

  group_h c = a + b;
  EXPECT_EQ(c, a);

  b = group<group_h>::h().crandom();
  c = a + b;
  EXPECT_EQ(c - b, a);

  b = a;
  EXPECT_EQ((-a) + b, group<group_h>::h_zero());

  field_zr pow_zero = group<field_zr>::zr_unity();
  EXPECT_EQ(b ^ pow_zero, b);

  field_zr pow_2 = group<field_zr>::zr_map(2);
  EXPECT_EQ(b ^ pow_2, b + b);
}

TEST (group_arithmetics, group_gt_initialization) {
  using namespace ::odn::crypto;
  bilinear_pairing pairing(2);

  group_gt a = group<group_gt>::gt();

  group_gt b{a};
  EXPECT_EQ(b, a);

  group_gt c{b};
  EXPECT_EQ(c, a);

  group_gt c1(std::move(a));
  EXPECT_EQ(c1, c);
}

TEST (group_arithmetics, group_gt_multiplication) {
  using namespace ::odn::crypto;
  bilinear_pairing pairing(2);

  const group_gt a = group<group_gt>::gt().crandom();
  group_gt b = group<group_gt>::gt_unity();

  group_gt c = a * b;
  EXPECT_EQ(c, a);

  c = a / a;
  EXPECT_EQ(c, group<group_gt>::gt_unity());

  b = group<group_gt>::gt().crandom();
  EXPECT_EQ((a * b) / b, a);

  field_zr pow_zero = group<field_zr>::zr_unity();
  EXPECT_EQ(b ^ pow_zero, b);

  field_zr pow_2 = group<field_zr>::zr_map(2);
  EXPECT_EQ(b ^ pow_2, b * b);
}