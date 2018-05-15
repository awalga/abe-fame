//
// Created by abel walga on 29/04/2018.
//

#include "odncrypto/binary_policy_tree.hpp"

using namespace ::odn::crypto;

binary_policy_tree_node &binary_policy_tree_node::addLeft(binary_policy_tree &set,
                                                          NodeType type,
                                                          const string &attr) const {
  //set.backend.emplace_back(2 * pos + 1, type, attr);
  set.backend.emplace(2 * pos + 1, binary_policy_tree_node(2 * pos + 1, type, attr));
  return set.backend.find(2 * pos + 1)->second;
}

binary_policy_tree_node &binary_policy_tree_node::addRight(binary_policy_tree &set,
                                                           NodeType type,
                                                           const string &attr) const {
  // set.backend.emplace_back( 2 * pos + 2, type, attr);
  set.backend.emplace(2 * pos + 2, binary_policy_tree_node(2 * pos + 2, type, attr));

  return set.backend.find(2 * pos + 2)->second;
}

const binary_policy_tree_node &binary_policy_tree_node::left(const binary_policy_tree &set) const {

  return set.backend.find(
      2 * pos + 1)->second;// TODO check bound malformed policy eg OR does not have children or one child
}

const binary_policy_tree_node &binary_policy_tree_node::right(const binary_policy_tree &set) const {

  return set.backend.find(
      2 * pos + 2)->second;// TODO check bound malformed policy eg OR does not have children or one child
}

const binary_policy_tree_node &binary_policy_tree_node::parent(const odn::crypto::binary_policy_tree &set) const {
  auto parent_pos = (pos - 1) / 2;// TODO Perf check if root return root
  auto parent = set.backend.find(parent_pos);

  return parent == set.backend.cend() ? set.backend.find(0)->second : parent->second;
}

const set<string> binary_policy_tree_node::prune(const odn::crypto::binary_policy_tree &policy,
                                                                  const set<string> attributes) const {
  stack<binary_policy_tree_node> nodes;
  nodes.push(*this);

  map<size_t, set<string>> partials;// treshold computation partials
  partials.insert({pos, set<string>()});
  while (!nodes.empty()) {
    auto current = nodes.top();
    nodes.pop();
    switch (current.treshold()) {
      case NodeType::LEAF: {
        auto *parent_node = &current.parent(policy);
        auto *gate = &parent_node->treshold();
        auto *gate_input = &partials[parent_node->pos];

        if (attributes.find(current.attribute()) != attributes.cend()) {// treshold 1-2 OK
          gate_input->insert(current.attribute());
          while (this != parent_node
              && (*gate == NodeType::OR || (*gate == NodeType::AND && gate_input->size() % 2 == 0))) {// check treshold 2-2
            auto *tmp_parent_node = &parent_node->parent(policy);
            gate = &tmp_parent_node->treshold();
            partials[tmp_parent_node->pos].insert(gate_input->cbegin(), gate_input->cend());
            gate_input = &partials[tmp_parent_node->pos];
            parent_node = tmp_parent_node;
          }
          // terminate
          if (this == parent_node
              && (*gate == NodeType::LEAF || *gate == NodeType::OR || (*gate == NodeType::AND && gate_input->size() % 2 == 0))) {
            return std::move(partials[pos]);
          }
        } else {// empty

          if (current.isLeft() && parent_node->treshold() == NodeType::OR) {
            nodes.push(parent_node->right(policy));// treshold Gate 1-2 : probe left only if it fails start probing on right
          }
        }
        break;
      }
      case NodeType::OR: {
        partials.insert({current.pos, set<string>()});// current node partials
        nodes.push(current.left(policy));// treshold Gate 1-2 : probe left only if it fails start probing on right
        break;
      }
      case NodeType::AND: {
        partials.insert({current.pos, set<string>()});// current node partials
        nodes.push(current.right(policy));// treshold Gate 2-2 : probe left and right starting with left
        nodes.push(current.left(policy));//
        break;
      }
    }
  }
  return set<string>();
}

binary_policy_tree::binary_policy_tree(NodeType type, string attr) {

  backend.emplace(0, binary_policy_tree_node(0, type, std::move(attr)));
};

size_t odn::crypto::binary_policy_tree::countLeaf() const {
  size_t countLeaf = 0;
  for (auto &entry: backend) {
    if (entry.second.treshold() == NodeType::LEAF)
      ++countLeaf;
  }
  return countLeaf;
}

map<string, size_t> odn::crypto::binary_policy_tree::attributes() const {
  map<string, size_t> attributes;
  size_t countLeaf = 0;
  for (auto &entry: backend) {
    if (entry.second.treshold() == NodeType::LEAF) {
      attributes.emplace(entry.second.attribute(), countLeaf);// TODO check duplicate
      ++countLeaf;
    }
  }
  return attributes;
}
