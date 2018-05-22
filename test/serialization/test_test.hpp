//
// Created by abel walga on 17/05/2018.
//

#ifndef ODNFC_TEST_TEST_HPP
#define ODNFC_TEST_TEST_HPP
#include <utility>
namespace odn {
namespace fc {

int _encode(const int64_t& o);
int _encode(const std::string&o);
int _encode(const uint8_t *o, size_t);

template<typename T>
int _encode(const T * o, size_t size) {
  std::cout << "encode array custom type" << std::endl;
  return 0;
}

template <typename T>
int _encode(const T& o) {
  return o.serialize();
}

template<class T>
int encode(T &&o) {
  return _encode(std::forward<T>(o));
}

template<class T, size_t size>
int encode(const T (&o)[size]) {
  std::cout << "encode array T" << std::endl;
  return _encode(o, size);
}

template<class KeyType, class ValueType>
void insert(const KeyType &key, const ValueType &value) {
  encode(key);
  encode(value);
}

int _encode(const int64_t& o) {
  std::cout << "encode int64" << std::endl;
  return 0;
}
int _encode(const std::string&o) {
  std::cout << "encode std::string" << std::endl;
  return 0;
}
int _encode(const uint8_t *o, size_t) {
  std::cout << "encode array uint8" << std::endl;
  return 0;
}

}
}
#endif //ODNFC_TEST_TEST_HPP
