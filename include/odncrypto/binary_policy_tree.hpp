//
// Created by abel walga on 29/04/2018.
//

#ifndef ODNCRYPTO_BINARY_POLICY_TREE_HPP
#define ODNCRYPTO_BINARY_POLICY_TREE_HPP

#include <string>                       // std::string
#include <memory>                       // std::move
#include <map>                          // std::map
#include <set>                          // std::set
#include <stack>                        // std::stack
using namespace ::std;

namespace odn {
namespace crypto {
class binary_policy_tree;

enum class NodeType { OR, AND, LEAF };

class binary_policy_tree_node {

 public :
  binary_policy_tree_node() = delete;
  binary_policy_tree_node(const binary_policy_tree_node &) = default;
  binary_policy_tree_node(binary_policy_tree_node &&other) noexcept
      : pos(other.pos), type(other.type), attr(std::move(other.attr)) {}
  explicit binary_policy_tree_node(const size_t pos_, NodeType type_, string attr_)
      : pos(pos_), type(type_), attr(std::move(attr_)) {}

  bool operator<(const binary_policy_tree_node &rhs) const {
    return pos < rhs.pos;
  }
  bool operator>(const binary_policy_tree_node &rhs) const {
    return rhs < *this;
  }
  bool operator<=(const binary_policy_tree_node &rhs) const {
    return !(rhs < *this);
  }
  bool operator>=(const binary_policy_tree_node &rhs) const {
    return !(*this < rhs);
  }

  binary_policy_tree_node &addLeft(binary_policy_tree &set, NodeType type, const string &attr) const;
  template<NodeType type>
  typename std::enable_if<type == NodeType::OR, binary_policy_tree_node>::type &addLeft(binary_policy_tree &set) const {
    return addLeft(set, NodeType::OR, "");
  };
  template<NodeType type>
  typename std::enable_if<type == NodeType::AND,
                          binary_policy_tree_node>::type &addLeft(binary_policy_tree &set) const {
    return addLeft(set, NodeType::AND, "");
  };
  template<NodeType type>
  typename std::enable_if<type == NodeType::AND, binary_policy_tree_node>::type &addLeft(binary_policy_tree &set,
                                                                                         const string &attr) const {
    return addLeft(set, NodeType::LEAF, attr);
  };

  binary_policy_tree_node &addRight(binary_policy_tree &set, NodeType type, const string &attr) const;

  template<NodeType type>
  typename std::enable_if<type == NodeType::OR,
                          binary_policy_tree_node>::type &addRight(binary_policy_tree &set) const {
    return addRight(set, NodeType::OR, "");
  };
  template<NodeType type>
  typename std::enable_if<type == NodeType::AND,
                          binary_policy_tree_node>::type &addRight(binary_policy_tree &set) const {
    return addRight(set, NodeType::AND, "");
  };
  template<NodeType type>
  typename std::enable_if<type == NodeType::AND, binary_policy_tree_node>::type &addRight(binary_policy_tree &set,
                                                                                          const string &attr) const {
    return addRight(set, NodeType::LEAF, attr);
  };

  const binary_policy_tree_node &left(const binary_policy_tree &set) const;
  const binary_policy_tree_node &right(const binary_policy_tree &set) const;

  bool isRight() const {
    return 2 * (pos / 2 ) == pos;// Right: 2 * parent.pos + 2, Left: 2 * parent.pos + 1
  }
  bool isLeft() const {
    return !isRight();
  }

  const string &attribute() const {
    return attr;
  }
  const NodeType &treshold() const {
    return type;
  }

  const binary_policy_tree_node &parent(const binary_policy_tree &policy) const;

  const set<string> prune(const binary_policy_tree &policy, const set<string> attributes) const;
 private:
  NodeType type;
  string attr;
  size_t pos;
};

class binary_policy_tree {
 public :
  explicit binary_policy_tree(NodeType type, string attr = "");
  binary_policy_tree(const binary_policy_tree &other) = default;
  binary_policy_tree(binary_policy_tree &&other) = default;

  const binary_policy_tree_node &root() const {
    return backend.find(0)->second;
  }

  size_t countLeaf() const;
  map<string, size_t> attributes() const;
 private :
  friend binary_policy_tree_node;
  map<size_t, binary_policy_tree_node> backend;
};
}// end namespace crypto
}// end namespace odn
#endif //ODNCRYPTO_BINARY_POLICY_TREE_HPP
