//
// Created by abel walga on 01/05/2018.
//

#ifndef ODNCRYPTO_ALGORITHMS_HPP
#define ODNCRYPTO_ALGORITHMS_HPP
#include "msp.hpp"
#include "binary_policy_tree.hpp"
#include <stack>

using namespace ::std;

namespace odn {
namespace crypto {
struct lsss {

  static msp lewko_water(const binary_policy_tree &policy) {
    msp lw_msp;

    stack<pair<size_t, binary_policy_tree_node>> nodes;
    nodes.push({0, policy.root()});
    while (!nodes.empty()) {
      auto current = nodes.top();
      nodes.pop();
      switch (current.second.treshold()) {
        case NodeType::OR : {
          auto row_add = lw_msp.add_share(current.first);
          lw_msp[row_add] = vector<int>(lw_msp[row_add].cbegin(), lw_msp[row_add].cend());

          nodes.push({row_add, current.second.right(policy)});
          nodes.push({current.first, current.second.left(policy)});

          break;
        }
        case NodeType::AND  : {
          auto row_mul = lw_msp.multiply_share(current.first);
          // right (0,...,0) || -1
          for (size_t i = 0; i != lw_msp.depth() - 1; ++i) {// lw_msp.depth > 0
            lw_msp[row_mul].push_back(0);
          }
          lw_msp[row_mul].push_back(-1);
          nodes.push({row_mul, current.second.right(policy)});

          // left v || (0,...,0) || 1
          for (size_t i = 0, l = lw_msp[current.first].size(); i != (lw_msp.depth() - 1 - l); ++i) {
            lw_msp[current.first].push_back(0);
          }
          lw_msp[current.first].push_back(1);
          nodes.push({current.first, current.second.left(policy)});
          break;
        }
        case NodeType::LEAF : {
          lw_msp.map_share(current.first, current.second.attribute());
          break;
        };
      }
    }
    return std::move(lw_msp);
  }
};
}// end namespace crypto
}// end namespace odn
#endif //ODNCRYPTO_ALGORITHMS_HPP
