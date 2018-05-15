//
// Created by abel walga on 03/05/2018.
//
#include "gtest/gtest.h"
#include "odncrypto/bilinear_pairing.hpp"

TEST (group_arithmetics, pairing) {
  using namespace ::odn::crypto;
  bilinear_pairing pairing(2);

  const field_zr a = group<field_zr>::zr().crandom();
  const field_zr b = group<field_zr>::zr().crandom();
  const group_g g = group<group_g>::g().crandom();
  const group_h h = group<group_h>::h().crandom();

  const group_gt gt = group<group_gt>::gt();

  EXPECT_EQ(gt(g^a, h^b), gt(g,h)^(a*b));

  EXPECT_EQ(gt(g^(a+b), h), gt(g,h)^(a+b));

  EXPECT_EQ(gt(g, h^(a+b)), gt(g,h)^(a+b));
}