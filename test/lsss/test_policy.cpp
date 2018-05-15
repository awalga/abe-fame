//
// Created by abel walga on 01/05/2018.
//
#include "gtest/gtest.h"
#include "odncrypto/binary_policy_tree.hpp"

TEST(lsss, or_policy) {
  using namespace ::odn::crypto;
  binary_policy_tree policy(NodeType::OR);

  auto *orNode = &policy.root();
  orNode->addLeft(policy, NodeType::LEAF, "A");
  orNode->addRight(policy, NodeType::LEAF, "B");

  EXPECT_EQ(orNode->treshold(), NodeType::OR);

  EXPECT_EQ(orNode->left(policy).attribute(), "A");
  EXPECT_EQ(orNode->left(policy).treshold(), NodeType::LEAF);
  EXPECT_EQ(orNode->left(policy).parent(policy).treshold(), NodeType::OR);

  EXPECT_EQ(orNode->right(policy).attribute(), "B");
  EXPECT_EQ(orNode->right(policy).treshold(), NodeType::LEAF);
  EXPECT_EQ(orNode->right(policy).parent(policy).treshold(), NodeType::OR);
}

TEST(lsss, and_policy) {
  using namespace ::odn::crypto;
  binary_policy_tree policy(NodeType::AND);

  auto *andNode = &policy.root();
  andNode->addLeft(policy, NodeType::LEAF, "A");
  andNode->addRight(policy, NodeType::LEAF, "B");

  EXPECT_EQ(andNode->treshold(), NodeType::AND);

  EXPECT_EQ(andNode->left(policy).attribute(), "A");
  EXPECT_EQ(andNode->left(policy).treshold(), NodeType::LEAF);
  EXPECT_EQ(andNode->left(policy).parent(policy).treshold(), NodeType::AND);

  EXPECT_EQ(andNode->right(policy).attribute(), "B");
  EXPECT_EQ(andNode->right(policy).treshold(), NodeType::LEAF);
  EXPECT_EQ(andNode->right(policy).parent(policy).treshold(), NodeType::AND);
}

TEST(lsss, policy) {
  using namespace ::odn::crypto;
  binary_policy_tree policy(NodeType::AND);
  auto &andPolicy = policy.root();

  auto &orPolicy = andPolicy.addLeft<NodeType::OR>(policy);

  andPolicy.addRight(policy, NodeType::LEAF, "B");
  orPolicy.addLeft(policy, NodeType::LEAF, "A");
  orPolicy.addRight(policy, NodeType::LEAF, "C");

  EXPECT_EQ(andPolicy.left(policy).left(policy).attribute(), "A");
  EXPECT_EQ(andPolicy.left(policy).treshold(), NodeType::OR);
  EXPECT_EQ(andPolicy.left(policy).right(policy).attribute(), "C");
  EXPECT_EQ(andPolicy.treshold(), NodeType::AND);
  EXPECT_EQ(andPolicy.right(policy).attribute(), "B");

  EXPECT_EQ(orPolicy.addRight(policy, NodeType::LEAF, "D").attribute(), "C");
  EXPECT_EQ(andPolicy.left(policy).treshold(), NodeType::OR);
  EXPECT_EQ(andPolicy.left(policy).left(policy).attribute(), "A");
  EXPECT_EQ(andPolicy.left(policy).right(policy).attribute(), "C");
  EXPECT_EQ(andPolicy.right(policy).attribute(), "B");
}

TEST(lsss, policy_prune_leaf) {
  using namespace ::odn::crypto;
  binary_policy_tree simple_policy(NodeType::LEAF, "A");

  auto simple_policy_intersect = simple_policy.root().prune(simple_policy, set<string>{"A"});

  EXPECT_EQ(simple_policy_intersect.size(), 1);
  EXPECT_TRUE(simple_policy_intersect.find("A") != simple_policy_intersect.cend());
}

TEST(lsss, policy_prune_simple_or) {
  using namespace ::odn::crypto;
  binary_policy_tree simple_or_policy(NodeType::OR);

  auto *orNode = &simple_or_policy.root();
  orNode->addLeft(simple_or_policy, NodeType::LEAF, "A");
  orNode->addRight(simple_or_policy, NodeType::LEAF, "B");

  auto simple_or_policy_intersection = simple_or_policy.root().prune(simple_or_policy, set<string>{"A"});

  EXPECT_EQ(simple_or_policy_intersection.size(), 1);
  EXPECT_TRUE(simple_or_policy_intersection.find("A") != simple_or_policy_intersection.cend());
}

TEST(lsss, policy_prune_simple_and) {
  using namespace ::odn::crypto;

  binary_policy_tree simple_and_policy(NodeType::AND);

  auto *andNode = &simple_and_policy.root();
  andNode->addLeft(simple_and_policy, NodeType::LEAF, "A");
  andNode->addRight(simple_and_policy, NodeType::LEAF, "B");

  auto simple_and_policy_intersection_0 = simple_and_policy.root().prune(simple_and_policy, set<string>{"A"});

  EXPECT_EQ(simple_and_policy_intersection_0.size(), 0);

  auto simple_and_policy_intersection_1 = simple_and_policy.root().prune(simple_and_policy, set<string>{"A", "B"});

  EXPECT_EQ(simple_and_policy_intersection_1.size(), 2);

  EXPECT_TRUE(simple_and_policy_intersection_1.find("A") != simple_and_policy_intersection_1.cend());
  EXPECT_TRUE(simple_and_policy_intersection_1.find("B") != simple_and_policy_intersection_1.cend());
}

TEST(lsss, policy_prune_and) {
  using namespace ::odn::crypto;

  binary_policy_tree and_policy(NodeType::AND);

  auto *andNode = &and_policy.root();
  andNode->addLeft<NodeType::OR>(and_policy).addLeft(and_policy, NodeType::LEAF, "A");
  andNode->addLeft<NodeType::OR>(and_policy).addRight(and_policy, NodeType::LEAF, "B");

  andNode->addRight<NodeType::OR>(and_policy).addLeft(and_policy, NodeType::LEAF, "C");
  andNode->addRight<NodeType::OR>(and_policy).addRight(and_policy, NodeType::LEAF, "D");

  auto simple_and_policy_intersection_0 = and_policy.root().prune(and_policy, set<string>{"A"});

  EXPECT_EQ(simple_and_policy_intersection_0.size(), 0);

  auto simple_and_policy_intersection_1 = and_policy.root().prune(and_policy, set<string>{"A", "C", "D"});

  EXPECT_EQ(simple_and_policy_intersection_1.size(), 2);

  EXPECT_TRUE(simple_and_policy_intersection_1.find("A") != simple_and_policy_intersection_1.cend());
  EXPECT_TRUE(simple_and_policy_intersection_1.find("C") != simple_and_policy_intersection_1.cend());

  auto simple_and_policy_intersection_2 = and_policy.root().prune(and_policy, set<string>{"A", "B", "C", "D"});

  EXPECT_EQ(simple_and_policy_intersection_2.size(), 2);

  EXPECT_TRUE(simple_and_policy_intersection_2.find("A") != simple_and_policy_intersection_2.cend());
  EXPECT_TRUE(simple_and_policy_intersection_2.find("C") != simple_and_policy_intersection_2.cend());
}

TEST(lsss, policy_prune_or) {
  using namespace ::odn::crypto;

  binary_policy_tree and_policy(NodeType::OR);

  auto *andNode = &and_policy.root();
  andNode->addLeft<NodeType::AND>(and_policy).addLeft(and_policy, NodeType::LEAF, "A");
  andNode->addLeft<NodeType::AND>(and_policy).addRight(and_policy, NodeType::LEAF, "B");

  andNode->addRight<NodeType::AND>(and_policy).addLeft(and_policy, NodeType::LEAF, "C");
  andNode->addRight<NodeType::AND>(and_policy).addRight(and_policy, NodeType::LEAF, "D");

  auto simple_and_policy_intersection_0 = and_policy.root().prune(and_policy, set<string>{"A"});

  EXPECT_EQ(simple_and_policy_intersection_0.size(), 0);

  auto simple_and_policy_intersection_1 = and_policy.root().prune(and_policy, set<string>{"A", "C"});

  EXPECT_EQ(simple_and_policy_intersection_1.size(), 0);

  auto simple_and_policy_intersection_2 = and_policy.root().prune(and_policy, set<string>{"A", "B", "C"});

  EXPECT_EQ(simple_and_policy_intersection_2.size(), 2);

  EXPECT_TRUE(simple_and_policy_intersection_2.find("A") != simple_and_policy_intersection_2.cend());
  EXPECT_TRUE(simple_and_policy_intersection_2.find("B") != simple_and_policy_intersection_2.cend());
}

TEST(lsss, policy_prune_and_or) {
  using namespace ::odn::crypto;

  // (A | C) & B
  binary_policy_tree policy(NodeType::AND);
  auto &orPolicy = policy.root().addLeft<NodeType::OR>(policy);
  policy.root().addRight(policy, NodeType::LEAF, "B");
  orPolicy.addLeft(policy, NodeType::LEAF, "A");
  orPolicy.addRight(policy, NodeType::LEAF, "C");

  auto simple_and_policy_intersection_0 = policy.root().prune(policy, set<string>{"A", "B", "C"});

  EXPECT_EQ(simple_and_policy_intersection_0.size(), 2);
  EXPECT_TRUE(simple_and_policy_intersection_0.find("A") != simple_and_policy_intersection_0.cend());
  EXPECT_TRUE(simple_and_policy_intersection_0.find("B") != simple_and_policy_intersection_0.cend());
}
