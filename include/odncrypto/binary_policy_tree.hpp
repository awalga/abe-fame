//
// Created by abel walga on 29/04/2018.
//

#ifndef ODNCRYPTO_BINARY_POLICY_TREE_HPP
#define ODNCRYPTO_BINARY_POLICY_TREE_HPP

#include "odnfc/cbor.hpp"
// #include "odncrypto/low/enum.h"
#include <string>                       // std::string
#include <memory>                       // std::move
#include <map>                          // std::map
#include <set>                          // std::set
#include <stack>                        // std::stack

using namespace ::std;

namespace odn {
namespace crypto {

class binary_policy_tree;
enum class NodeType : std::uint8_t {
  OR, AND, LEAF
};

// BETTER_ENUM(NodeType, int, OR = 1, AND, LEAF)

class binary_policy_tree_node {

 public :

  binary_policy_tree_node() = delete;
  binary_policy_tree_node(const binary_policy_tree_node &) = default;
  binary_policy_tree_node(binary_policy_tree_node &&other) noexcept
      : pos(other.pos), type(other.type), attr(std::move(other.attr)) {}
  binary_policy_tree_node(const size_t pos_, NodeType type_, string attr_)
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
    return 2 * (pos / 2) == pos;// Right: 2 * parent.pos + 2, Left: 2 * parent.pos + 1
  }
  bool isLeft() const {
    return !isRight();
  }

  const size_t position() const {
    return pos;
  }
  const string &attribute() const {
    return attr;
  }
  const NodeType &treshold() const {
    return type;
  }

  const binary_policy_tree_node &parent(const binary_policy_tree &policy) const;
  const set<string> prune(const binary_policy_tree &policy, set<string> attributes) const;

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
  size_t countNode() const;
  map<string, size_t> attributes() const;

 private :

  friend binary_policy_tree_node;
  template<typename U, typename V>
  friend
  class odn::fc::adl_serializer;

  map<size_t, binary_policy_tree_node> backend;

};

template<typename T>
T &serialize(T &ar, const binary_policy_tree_node &node) {

  auto _vector_writer = ar.array(3);
  _vector_writer.push(node.position());// size_t
  _vector_writer.push(node.attribute());// std::string
  _vector_writer.push(static_cast<uint8_t >(node.treshold()));// NodeType

  return ar;

}

template<typename T>
T &serialize(T &ar, const binary_policy_tree &policy) {

  auto _map_writer = ar.map(policy.countNode());
  auto root = policy.root();

  stack<binary_policy_tree_node> nodes;
  nodes.push(policy.root());
  // traverse and add all nodes
  while (!nodes.empty()) {
    auto current = nodes.top();
    nodes.pop();
    switch (current.treshold()) {
      case NodeType::OR :
      case NodeType::AND : {
        _map_writer.insert(current.position(), current);
        nodes.push(current.left(policy));
        nodes.push(current.right(policy));
        break;
      }
      case NodeType::LEAF : {
        _map_writer.insert(current.position(), current);
        break;
      }
    }
  }
  return ar;

}

}// end namespace crypto
}// end namespace odn



namespace odn {
namespace fc {

using namespace ::odn::crypto;

template<typename T>
struct adl_serializer<T, binary_policy_tree_node> {

  binary_policy_tree_node operator()(T &ar) {
    auto _vector_reader = ar.array();

    size_t _position;
    _vector_reader.pop(&_position);

    string _attribute;
    _vector_reader.pop(&_attribute);

    uint8_t _treshold_idx;
    _vector_reader.pop(&_treshold_idx);

    NodeType _treshold = static_cast<NodeType >(_treshold_idx);

    return binary_policy_tree_node(_position, _treshold, _attribute);
  }

};

template<typename T>
struct adl_serializer<T, binary_policy_tree> {

  binary_policy_tree operator()(T &ar) {
    auto _map_reader = ar.map();

    size_t _size = _map_reader.size();
    std::pair<size_t, binary_policy_tree_node> _root = _map_reader.template get<size_t, binary_policy_tree_node>();
    // create root
    binary_policy_tree tree(_root.second.treshold(), _root.second.attribute());

    for (size_t i = 1; i < _size; ++i) {
      std::pair<size_t, binary_policy_tree_node> _node = _map_reader.template get<size_t, binary_policy_tree_node>();
      tree.backend.insert({_node.first, _node.second});
    }

    return tree;
  }

};

}// end namespace fc
}// end namespace odn
#endif //ODNCRYPTO_BINARY_POLICY_TREE_HPP
