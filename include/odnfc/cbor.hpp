//
// Created by abel walga on 14/05/2018.
//

#ifndef ODNFC_CBOR_HPP
#define ODNFC_CBOR_HPP
#include "cbor.h"
#include <type_traits>

namespace odn {
namespace fc {

template<typename writer, typename object>
struct serializer {
  writer &operator()(writer &, const object &);
  writer &operator()(writer &, object &);
};

template<typename writer, typename object>
struct adl_serializer {
  object operator()(writer &);
};

struct cbor_container {};

struct cbor_array_container {};

struct cbor_map_container {};

struct cbor_encoder_cursor {
 public :
  cbor_encoder_cursor() : container(), encoder(&container) {};
  cbor_encoder_cursor(CborEncoder *encoder_, CborEncoder container_) : container(container_), encoder(encoder_) {}
  cbor_encoder_cursor(const cbor_encoder_cursor &) = delete;
  cbor_encoder_cursor(cbor_encoder_cursor &&other) noexcept
      : container(other.container), encoder(other.encoder) {
  }
  cbor_encoder_cursor &operator=(const cbor_encoder_cursor &) = delete;
  cbor_encoder_cursor &operator=(cbor_encoder_cursor &&other) noexcept {
    using namespace std;
    swap(container, other.container);
    swap(encoder, other.encoder);
    return *this;
  }

  CborEncoder *getContainer() {
    return &container;
  }

  const CborEncoder *getContainer() const {
    return &container;
  }

  void roll_back_state(const CborEncoder *tag) {
    container.data.ptr = tag->data.ptr;
    container.data.bytes_needed = tag->data.bytes_needed;
    container.end = tag->end;
    container.flags = tag->flags;
    container.remaining = tag->remaining;
  }

  ~cbor_encoder_cursor() {
    if (encoder != &container) {
      cbor_encoder_close_container(encoder, &container);
    }
  }
 private:
  CborEncoder container, *encoder;
};
struct cbor_parser_cursor {
 public :
  cbor_parser_cursor() noexcept : container(), parent(&container) {};
  cbor_parser_cursor(CborValue container_, CborValue *parent_) noexcept
      : container(container_), parent(parent_) {
  }
  cbor_parser_cursor(const cbor_parser_cursor &) = delete;
  cbor_parser_cursor(cbor_parser_cursor &&other) noexcept
      : container(other.container), parent(other.parent) {}

  cbor_parser_cursor &operator=(const cbor_parser_cursor &) = delete;
  cbor_parser_cursor &operator=(cbor_parser_cursor &&other) {
    using namespace ::std;
    swap(container, other.container);
    swap(parent, other.parent);
    return *this;
  }

  const CborValue *getContainer() const {
    return &container;
  }

  CborValue *getContainer() {
    return &container;
  }

  const CborValue *getParent() const {
    return parent;
  }

  CborValue *getParent() {
    return parent;
  }

  ~cbor_parser_cursor() {
    if (cbor_value_is_container(parent)) {
      while (!cbor_value_at_end(&container)) {
        cbor_value_advance(&container);
      }
      cbor_value_leave_container(parent, &container);
    }
  }
 private :
  CborValue container, *parent;
};
};// end namespace fc
}// end namespace odn
#endif //ODNFC_CBOR_HPP
