//
// Created by abel walga on 17/05/2018.
//
#include "gtest/gtest.h"
#include <iostream>
#include <utility>

#include "test_test.hpp"
struct my_type {
  int serialize() const {
    std::cout << "serialize" << std::endl;
    return 0;
  }
};
TEST (test, test_one) {
  std::cout << std::endl;
  ::odn::fc::insert(std::string("key"), std::string("value"));
  ::odn::fc::insert(int64_t(0), int64_t(0));
  ::odn::fc::insert(std::string("key"), (uint8_t[2]) {0, 0});
  ::odn::fc::insert(std::string("key"), my_type());
  ::odn::fc::insert(std::string("key"), (my_type[2]) {my_type(), my_type()});
}
