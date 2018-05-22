//
// Created by abel walga on 13/05/2018.
//

#ifndef ODNFC_MEMORY_BUFFER_HPP
#define ODNFC_MEMORY_BUFFER_HPP
#include <vector>                   /// std::vector
#include <memory>                   /// std::shared_ptr, std::make_shared
#include <cmath>                    /// std::sqrt

namespace odn {
namespace fc {
class memory_buffer {
  typedef std::vector<uint8_t> memory_type;
 public:
  memory_buffer() : buffer(std::make_shared<memory_type>(1)) {};
  explicit memory_buffer(const size_t len_) : buffer(std::make_shared<memory_type>(len_)) {}

  uint8_t *begin() {
    return buffer->data();
  }

  const uint8_t *begin() const {
    return buffer->data();
  }

  const uint8_t *end() {
    return begin() + buffer->capacity();
  }

  const uint8_t *end() const {
    return begin() + buffer->capacity();
  }

  void realloc() {
    buffer->resize(buffer->size() + (buffer->size() * 2 * (1 + std::sqrt(5)) / 3));// TODO reserver
  }

  void realloc(const size_t len_) {
    buffer->resize(len_ + (buffer->size() * 2 * (1 + std::sqrt(5)) / 3));// TODO reserver
  }
 private :
  std::shared_ptr<memory_type> buffer;
};
}// end namespace fc
}// end namespace odn

#endif //ODNFC_MEMORY_BUFFER_HPP
