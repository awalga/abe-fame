//
// Created by abel walga on 02/05/2018.
//
#include "gtest/gtest.h"
#include "odncrypto/relic_util.hpp"

TEST (relic_memory, relic_memory_type) {
  using namespace odn::crypto;
  // EXPECT_EQ(relic_mem_alloc_strategy::type, relic_mem_alloc_auto);
}

TEST (relic_memory, relic_memory_move) {
  using namespace odn::crypto;
  bn_t lhs;
  bn_new(lhs);
  bn_set_2b(lhs, 2);

  bn_t rhs;
  bn_new(rhs);
  bn_set_2b(lhs, 6);

  bn_t oldrhs;
  bn_new(oldrhs);
  bn_copy(oldrhs, rhs);

  relic_memory::relic_move(lhs, rhs);
  bn_new(rhs);
  bn_sub(rhs, lhs, oldrhs);

  EXPECT_TRUE(bn_is_zero(rhs));
}
