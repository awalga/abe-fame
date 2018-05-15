//
// Created by abel walga on 29/04/2018.
//

#ifndef ODNCRYPTO_MSP_HPP
#define ODNCRYPTO_MSP_HPP

#include <string>
#include <vector>                   // std::vector
#include <map>                      // std::map

using namespace ::std;

namespace odn {
namespace crypto {

class msp {
 public :
  msp() : num_columns(1), mij({{1}}) {};
  msp(const size_t &size_) : num_columns(1), mij(size_) {};
  msp(const msp &) = default;
  msp(msp &&) = default;

  msp &operator=(const msp &) = default;
  msp &operator=(msp &&) = default;

  size_t multiply_share(const size_t &row) {
    auto &row_share = mij[row];
    mij.push_back(vector<int>());
    ++num_columns;
    return mij.size() - 1;
  }

  size_t add_share(const size_t &row) {
    auto &row_share = mij[row];
    mij.push_back(vector<int>(row_share.cbegin(), row_share.cend()));
    return mij.size() - 1;
  }

  vector<int> &operator[](const size_t &i) {
    return mij[i];
  }

  const vector<int> &operator[](const size_t &i) const {
    return mij[i];
  }

  size_t depth() const {
    return num_columns;
  }

  size_t size() const {
    return mij.size();
  }

  void map_share(int i, const string &share) {
    rho.insert({share, i});
  }

  int get_share(const string &share) const {
    auto found = rho.find(share);
    return found != rho.cend() ? found->second : -1;
  }
 private :
  size_t num_columns;
  map<string, int> rho;
  vector<vector<int>> mij;
};
}// end namespace crypto
}// end namespace odn
#endif //ODNCRYPTO_MSP_HPP
