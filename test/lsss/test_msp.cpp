//
// Created by abel walga on 01/05/2018.
//

#include "gtest/gtest.h"
#include "odncrypto/msp.hpp"
#include "odncrypto/algorithms.hpp"

TEST(lsss, msp_or) {
  using namespace ::odn::crypto;
  binary_policy_tree policy(NodeType::OR);

  auto *orNode = &policy.root();
  orNode->addLeft(policy, NodeType::LEAF, "A");
  orNode->addRight(policy, NodeType::LEAF, "B");

  auto lw_msp = lsss::lewko_water(policy);
  int mij[][2] = {
      {1},
      {1}
  };
  EXPECT_EQ(2, lw_msp.size());
  for (auto &entry : policy.attributes()) {
    auto &row = lw_msp[entry.second];
    for (auto i = 0; i != row.size(); ++i) {
      EXPECT_EQ(mij[entry.second][i], row[i]);
    }
  }
}

TEST(lsss, msp_and) {
  using namespace ::odn::crypto;
  binary_policy_tree policy(NodeType::AND);

  auto *orNode = &policy.root();
  orNode->addLeft(policy, NodeType::LEAF, "A");
  orNode->addRight(policy, NodeType::LEAF, "B");

  auto lw_msp = lsss::lewko_water(policy);
  int mij[][2] = {
      {1, 1},
      {0, -1}
  };
  EXPECT_EQ(2, lw_msp.size());
  for (auto &entry : policy.attributes()) {
    auto &row = lw_msp[entry.second];
    for (auto i = 0; i != row.size(); ++i) {
      EXPECT_EQ(mij[entry.second][i], row[i]);
    }
  }
}

TEST(lsss, msp_big) {
  using namespace ::odn::crypto;
  binary_policy_tree policy(NodeType::OR);

  auto *orNode = &policy.root();
  orNode->addLeft(policy, NodeType::AND, "").addLeft(policy, NodeType::LEAF, "A0");
  orNode->addLeft(policy, NodeType::AND, "").addRight(policy, NodeType::LEAF, "C0");

  orNode = &orNode->addRight<NodeType::OR>(policy);
  orNode->addLeft<NodeType::AND>(policy).addLeft(policy, NodeType::LEAF, "A1");
  orNode->addLeft<NodeType::AND>(policy).addRight(policy, NodeType::LEAF, "D0");

  orNode = &orNode->addRight<NodeType::OR>(policy);
  orNode->addLeft<NodeType::AND>(policy).addLeft(policy, NodeType::LEAF, "B0");
  orNode->addLeft<NodeType::AND>(policy).addRight(policy, NodeType::LEAF, "C1");

  orNode = &orNode->addRight<NodeType::OR>(policy);
  orNode->addLeft<NodeType::AND>(policy).addLeft(policy, NodeType::LEAF, "B1");
  orNode->addLeft<NodeType::AND>(policy).addRight(policy, NodeType::LEAF, "D1");

  orNode = &orNode->addRight<NodeType::OR>(policy);
  orNode->addLeft<NodeType::AND>(policy).addLeft(policy, NodeType::LEAF, "E0");
  orNode->addLeft<NodeType::AND>(policy).addRight(policy, NodeType::LEAF, "G0");

  orNode = &orNode->addRight<NodeType::OR>(policy);
  orNode->addLeft<NodeType::AND>(policy).addLeft(policy, NodeType::LEAF, "E1");
  orNode->addLeft<NodeType::AND>(policy).addRight(policy, NodeType::LEAF, "H0");

  orNode = &orNode->addRight<NodeType::OR>(policy);
  orNode->addLeft<NodeType::AND>(policy).addLeft(policy, NodeType::LEAF, "F0");
  orNode->addLeft<NodeType::AND>(policy).addRight(policy, NodeType::LEAF, "G1");

  orNode->addRight<NodeType::AND>(policy).addLeft(policy, NodeType::LEAF, "F1");
  orNode->addRight<NodeType::AND>(policy).addRight(policy, NodeType::LEAF, "H1");

  auto lw_msp = lsss::lewko_water(policy);
  int mij[][9] = {
      {1, 1},
      {1, 0, 1},
      {0, -1},
      {1, 0, 0, 1},
      {0, 0, -1},
      {1, 0, 0, 0, 1},
      {0, 0, 0, -1},
      {1, 0, 0, 0, 0, 1},
      {0, 0, 0, 0, -1},
      {1, 0, 0, 0, 0, 0, 1},
      {0, 0, 0, 0, 0, -1},
      {1, 0, 0, 0, 0, 0, 0, 1},
      {0, 0, 0, 0, 0, 0, -1},
      {1, 0, 0, 0, 0, 0, 0, 0, 1},
      {0, 0, 0, 0, 0, 0, 0, -1},
      {0, 0, 0, 0, 0, 0, 0, 0, -1}
  };
  EXPECT_EQ(16, lw_msp.size());
  for (auto &attr : policy.attributes()) {
    EXPECT_TRUE(lw_msp.get_share(attr.first) != -1);
  }
  //std::cout << std::endl;
  for (size_t i = 0; i != lw_msp.size(); ++i) {
    //std::cout << "{";
    auto &row = lw_msp[i];
    for (size_t j = 0; j != row.size(); ++j) {
      EXPECT_EQ(mij[i][j], row[j]);
      //std::cout << row[j];
      //if (j != row.size() - 1) {
      //  std::cout << ",";
      //}
    }
    //std::cout << "}";
    //std::cout << std::endl;
  }
}