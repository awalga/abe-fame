//
// Created by abel walga on 13/05/2018.
//

#ifndef ODNFC_CBOR_ENCODER_HPP
#define ODNFC_CBOR_ENCODER_HPP

#include "cbor.h"
#include "cbor.hpp"
#include <string>
#include <type_traits>
#include <utility>

namespace odn {
namespace fc {

class cbor_map_encoder {
 public :
  cbor_map_encoder() = delete;
  cbor_map_encoder(CborEncoder *encoder_, CborEncoder container_) : encoder(encoder_), container(container_) {}
  cbor_map_encoder(const cbor_map_encoder &) = delete;
  cbor_map_encoder(cbor_map_encoder &&other) noexcept
      : encoder(other.encoder), container(other.container) {
  }
  cbor_map_encoder &operator=(const cbor_map_encoder &) = delete;
  cbor_map_encoder &operator=(cbor_map_encoder &&other) noexcept {
    using namespace std;
    swap(encoder, other.encoder);
    swap(container, other.container);
    return *this;
  }

  CborEncoder *getEncoder() {
    return encoder;
  }

  CborEncoder *getContainer() {
    return &container;
  }

  void roll_back_state(const CborEncoder *tag) {
    container.data.ptr = tag->data.ptr;
    container.data.bytes_needed = tag->data.bytes_needed;
    container.end = tag->end;
    container.flags = tag->flags;
    container.remaining = tag->remaining;
  }

  ~cbor_map_encoder() {
    cbor_encoder_close_container(encoder, &container);
  }
 private:
  CborEncoder *encoder, container;
};

class cbor_array_encoder {
 public :
  cbor_array_encoder() = delete;
  cbor_array_encoder(CborEncoder *encoder_, CborEncoder container_) : encoder(encoder_), container(container_) {}
  cbor_array_encoder(const cbor_array_encoder &) = delete;
  cbor_array_encoder(cbor_array_encoder &&other) noexcept : encoder(other.encoder), container(other.container) {}
  cbor_array_encoder &operator=(const cbor_array_encoder &) = delete;
  cbor_array_encoder &operator=(cbor_array_encoder &&other) noexcept {
    using namespace std;
    swap(encoder, other.encoder);
    swap(container, other.container);
    return *this;
  }

  CborEncoder *getEncoder() {
    return encoder;
  }
  CborEncoder *getContainer() {
    return &container;
  }
  void roll_back_state(const CborEncoder *tag) {
    container.data.ptr = tag->data.ptr;
    container.data.bytes_needed = tag->data.bytes_needed;
    container.end = tag->end;
    container.flags = tag->flags;
    container.remaining = tag->remaining;
  }

  ~cbor_array_encoder() {
    cbor_encoder_close_container(encoder, &container);
  }
 private:
  CborEncoder *encoder, container;
};

class cbor_encoder {
 public :
  cbor_encoder() noexcept : encoder() {}
  explicit cbor_encoder(CborEncoder encoder_) noexcept : encoder(std::move(encoder_)) {}
  cbor_encoder(const cbor_encoder &) = delete;
  cbor_encoder(cbor_encoder &&other) noexcept : encoder(std::move(other.encoder)) {}
  cbor_encoder &operator=(const cbor_encoder &) = delete;
  cbor_encoder &operator=(cbor_encoder &&other) {
    using namespace ::std;
    swap(encoder, other.encoder);
    return *this;
  }

  CborEncoder *getEncoder() {
    return &encoder;
  }
  void roll_back_state(const CborEncoder *tag) {
    encoder.data.ptr = tag->data.ptr;
    encoder.data.bytes_needed = tag->data.bytes_needed;
    encoder.end = tag->end;
    encoder.flags = tag->flags;
    encoder.remaining = tag->remaining;
  }
 private:
  CborEncoder encoder;
};

template<>
struct is_array_type<cbor_array_encoder> : std::true_type {
  constexpr static bool value = true;
  typedef cbor_array_encoder type;
};
template<>
struct is_dictionary_type<cbor_map_encoder> : std::true_type {
  constexpr static bool value = true;
  typedef cbor_map_encoder type;
};
template<>
struct is_container_type<cbor_array_encoder> : std::true_type {
  constexpr static bool value = true;
  typedef cbor_array_encoder type;

};
template<>
struct is_container_type<cbor_map_encoder> : std::true_type {
  constexpr static bool value = true;
  typedef cbor_map_encoder type;
};
template<typename cbor_writer, typename cbor_encoder_type>
class cbor_writer_base;
template<typename cbor_writer>
class cbor_writer_base<cbor_writer, cbor_encoder> {

};
template<typename cbor_writer>
class cbor_writer_base<cbor_writer, cbor_array_encoder> {
 public:
  void push(const uint8_t *data, const size_t len) {
    auto encoder = static_cast<cbor_writer *>(this)->getEncoder();
    auto container = encoder->getContainer();
    auto buffer = static_cast<cbor_writer *>(this)->getBuffer();
    auto err = CborNoError;
    do {
      const CborEncoder tag(*container);
      err = cbor_encode_byte_string(container, data, len);
      if (err == CborErrorOutOfMemory) {
        auto bytes_needed = static_cast<size_t>(container->data.bytes_needed);
        encoder->roll_back_state(&tag);
        auto offset = container->data.ptr - buffer->begin();
        buffer->realloc(bytes_needed);
        container->data.ptr = buffer->begin() + offset;
        container->end = buffer->end();
      }
    } while (err == CborErrorOutOfMemory);
  }

  template<size_t len_>
  void push(const uint8_t(&data)[len_]) {
    auto encoder = static_cast<cbor_writer *>(this)->getEncoder();
    auto container = encoder->getContainer();
    auto buffer = static_cast<cbor_writer *>(this)->getBuffer();
    auto err = CborNoError;
    do {
      const CborEncoder tag(*container);
      err = cbor_encode_byte_string(container, data, len_);

      if (err == CborErrorOutOfMemory) {
        auto bytes_needed = static_cast<size_t>(container->data.bytes_needed);
        encoder->roll_back_state(&tag);
        auto offset = container->data.ptr - buffer->begin();
        buffer->realloc(bytes_needed);
        container->data.ptr = buffer->begin() + offset;
        container->end = buffer->end();
      }
    } while (err == CborErrorOutOfMemory);
  }

  void push(int64_t value) {
    auto encoder = static_cast<cbor_writer *>(this)->getEncoder();
    auto container = encoder->getContainer();
    auto buffer = static_cast<cbor_writer *>(this)->getBuffer();
    auto err = CborNoError;
    do {
      const CborEncoder tag(*container);
      err = cbor_encode_int(container, value);
      if (err == CborErrorOutOfMemory) {
        auto bytes_needed = static_cast<size_t>(container->data.bytes_needed);
        encoder->roll_back_state(&tag);
        auto offset = container->data.ptr - buffer->begin();
        buffer->realloc(bytes_needed);
        container->data.ptr = buffer->begin() + offset;
        container->end = buffer->end();
      }
    } while (err == CborErrorOutOfMemory);
  }

  void push(const std::string &string) {
    auto encoder = static_cast<cbor_writer *>(this)->getEncoder();
    auto container = encoder->getContainer();
    auto buffer = static_cast<cbor_writer *>(this)->getBuffer();
    auto err = CborNoError;
    do {
      const CborEncoder tag(*container);
      err = cbor_encode_text_stringz(container, string.c_str());
      if (err == CborErrorOutOfMemory) {
        auto bytes_needed = static_cast<size_t>(container->data.bytes_needed);
        encoder->roll_back_state(&tag);
        auto offset = container->data.ptr - buffer->begin();
        buffer->realloc(bytes_needed);
        container->data.ptr = buffer->begin() + offset;
        container->end = buffer->end();
      }
    } while (err == CborErrorOutOfMemory);
  }
};

template<typename cbor_writer>
class cbor_writer_base<cbor_writer, cbor_map_encoder> {
 public :
  void insert(const std::string &key, const uint8_t *value, size_t len) {
    auto encoder = static_cast<cbor_writer *>(this)->getEncoder();
    auto container = encoder->getContainer();
    auto buffer = static_cast<cbor_writer *>(this)->getBuffer();
    auto err = CborNoError;
    do {
      const CborEncoder tag(*container);
      err = cbor_encode_text_stringz(container, key.c_str());
      err = cbor_encode_byte_string(container, value, len);
      if (err == CborErrorOutOfMemory) {
        auto bytes_needed = static_cast<size_t>(container->data.bytes_needed);
        encoder->roll_back_state(&tag);
        auto offset = container->data.ptr - buffer->begin();
        buffer->realloc(bytes_needed);
        container->data.ptr = buffer->begin() + offset;
        container->end = buffer->end();
      }
    } while (err == CborErrorOutOfMemory);
  }
  template<size_t len_>
  void insert(const std::string &key, const uint8_t (&value)[len_]) {
    auto encoder = static_cast<cbor_writer *>(this)->getEncoder();
    auto container = encoder->getContainer();
    auto buffer = static_cast<cbor_writer *>(this)->getBuffer();
    auto err = CborNoError;
    do {
      const CborEncoder tag(*container);
      err = cbor_encode_text_stringz(container, key.c_str());
      err = cbor_encode_byte_string(container, value, len_);
      if (err == CborErrorOutOfMemory) {
        auto bytes_needed = static_cast<size_t>(container->data.bytes_needed);
        encoder->roll_back_state(&tag);
        auto offset = container->data.ptr - buffer->begin();
        buffer->realloc(bytes_needed);
        container->data.ptr = buffer->begin() + offset;
        container->end = buffer->end();
      }
    } while (err == CborErrorOutOfMemory);
  }
  void insert(const std::string &key, int64_t value) {
    auto encoder = static_cast<cbor_writer *>(this)->getEncoder();
    auto container = encoder->getContainer();
    auto buffer = static_cast<cbor_writer *>(this)->getBuffer();
    auto err = CborNoError;
    do {
      const CborEncoder tag(*container);
      err = cbor_encode_text_stringz(container, key.c_str());
      err = cbor_encode_int(container, value);
      if (err == CborErrorOutOfMemory) {
        auto bytes_needed = static_cast<size_t>(container->data.bytes_needed);
        encoder->roll_back_state(&tag);
        auto offset = container->data.ptr - buffer->begin();
        buffer->realloc(bytes_needed);
        container->data.ptr = buffer->begin() + offset;
        container->end = buffer->end();
      }
    } while (err == CborErrorOutOfMemory);
  }
  void insert(const std::string &key, const std::string &value) {
    auto encoder = static_cast<cbor_writer *>(this)->getEncoder();
    auto container = encoder->getContainer();
    auto buffer = static_cast<cbor_writer *>(this)->getBuffer();
    auto err = CborNoError;
    do {
      const CborEncoder tag(*container);
      err = cbor_encode_text_stringz(container, key.c_str());
      err = cbor_encode_text_stringz(container, value.c_str());
      if (err == CborErrorOutOfMemory) {
        auto bytes_needed = static_cast<size_t>(container->data.bytes_needed);
        encoder->roll_back_state(&tag);
        auto offset = container->data.ptr - buffer->begin();
        buffer->realloc(bytes_needed);
        container->data.ptr = buffer->begin() + offset;
        container->end = buffer->end();
      }
    } while (err == CborErrorOutOfMemory);
  }
};

template<typename cbor_encoder_Type, typename buffer_type>
class cbor_writer : public cbor_writer_base<cbor_writer<cbor_encoder_Type, buffer_type>, cbor_encoder_Type> {

 public:
  template<typename T = cbor_encoder_Type, typename = typename std::enable_if<std::is_default_constructible<T>{}>::type>
  cbor_writer() : encoder(), buffer() {}
  cbor_writer(const cbor_writer<cbor_encoder_Type, buffer_type> &) = delete;
  template<typename = typename std::enable_if<std::is_move_constructible<cbor_encoder_Type>{}>::type>
  cbor_writer(cbor_writer<cbor_encoder_Type, buffer_type> &&other)
      : encoder(std::move(other.encoder)), buffer(std::move(other.buffer)) {}

  template<typename T = cbor_encoder_Type, typename = typename std::enable_if<std::is_default_constructible<T>{}>::type>
  explicit cbor_writer(buffer_type buffer_) : encoder(), buffer(std::move(buffer_)) {
    cbor_encoder_init(encoder.getEncoder(),
                      buffer.begin(),
                      static_cast<size_t>(buffer.end() - buffer.begin()),
                      0);// TODO check error
  }
  template<typename T = cbor_encoder_Type, typename = typename std::enable_if<std::is_move_constructible<T>{}>::type>
  cbor_writer(T encoder_, buffer_type buffer_) : encoder(std::move(encoder_)), buffer(std::move(buffer_)) {}

  template<typename T, typename = typename std::enable_if<is_array_type<T>{}>::type>
  cbor_writer<T, buffer_type> array(const size_t length) {
    return array(length, is_container_type<cbor_encoder_Type>{});
  }
  template<typename T, typename = typename std::enable_if<is_dictionary_type<T>{}>::type>
  cbor_writer<T, buffer_type> map(const size_t size) {
    return map(size, is_container_type<cbor_encoder_Type>{});
  }

  const cbor_encoder_Type *getEncoder() const {
    return &encoder;
  }
  cbor_encoder_Type *getEncoder() {
    return &encoder;
  }
  const buffer_type *getBuffer() const {
    return &buffer;
  }
  buffer_type *getBuffer() {
    return &buffer;
  }
 private:
  cbor_writer<cbor_array_encoder, buffer_type> array(size_t len, std::false_type);
  cbor_writer<cbor_array_encoder, buffer_type> array(size_t len, std::true_type);
  cbor_writer<cbor_map_encoder, buffer_type> map(size_t len, std::false_type);
  cbor_writer<cbor_map_encoder, buffer_type> map(size_t len, std::true_type);

  cbor_encoder_Type encoder;
  buffer_type buffer;
};

template<typename T, typename B>
cbor_writer<cbor_array_encoder, B> cbor_writer<T, B>::array(const size_t len, std::true_type) {
  auto err = CborNoError;
  CborEncoder container;
  do {
    auto parent = encoder.getContainer();
    CborEncoder tag(*parent);
    err = cbor_encoder_create_array(parent, &container, len);
    if (err == CborErrorOutOfMemory) {
      encoder.roll_back_state(&tag);
      auto offset = parent->data.ptr - buffer.begin();
      buffer.realloc();
      parent->data.ptr = buffer.begin() + offset;
      parent->end = buffer.end();
    }
  } while (err == CborErrorOutOfMemory);
  return cbor_writer<cbor_array_encoder, B>(cbor_array_encoder(encoder.getContainer(), container), buffer);
}

template<typename T, typename B>
cbor_writer<cbor_array_encoder, B> cbor_writer<T, B>::array(const size_t len, std::false_type) {
  auto err = CborNoError;
  CborEncoder container;
  do {
    auto parent = encoder.getEncoder();
    CborEncoder tag(*parent);
    err = cbor_encoder_create_array(parent, &container, len);
    if (err == CborErrorOutOfMemory) {
      encoder.roll_back_state(&tag);
      auto offset = parent->data.ptr - buffer.begin();
      buffer.realloc();
      parent->data.ptr = buffer.begin() + offset;
      parent->end = buffer.end();
    }
  } while (err == CborErrorOutOfMemory);
  return cbor_writer<cbor_array_encoder, B>(cbor_array_encoder(encoder.getEncoder(), container), buffer);
}

template<typename T, typename B>
cbor_writer<cbor_map_encoder, B> cbor_writer<T, B>::map(const size_t len, std::true_type) {
  auto err = CborNoError;
  CborEncoder container;
  do {
    auto parent = encoder.getContainer();
    CborEncoder tag(*parent);
    err = cbor_encoder_create_map(parent, &container, len);
    if (err == CborErrorOutOfMemory) {
      encoder.roll_back_state(&tag);
      auto offset = parent->data.ptr - buffer.begin();
      buffer.realloc();
      parent->data.ptr = buffer.begin() + offset;
      parent->end = buffer.end();
    }
  } while (err == CborErrorOutOfMemory);
  return cbor_writer<cbor_map_encoder, B>(cbor_map_encoder(encoder.getContainer(), container), buffer);
}

template<typename T, typename B>
cbor_writer<cbor_map_encoder, B> cbor_writer<T, B>::map(const size_t len, std::false_type) {
  auto err = CborNoError;
  CborEncoder container;
  do {
    auto parent = encoder.getEncoder();
    CborEncoder tag(*parent);
    err = cbor_encoder_create_map(parent, &container, len);
    if (err == CborErrorOutOfMemory) {
      encoder.roll_back_state(&tag);
      auto offset = parent->data.ptr - buffer.begin();
      buffer.realloc();
      parent->data.ptr = buffer.begin() + offset;
      parent->end = buffer.end();
    }
  } while (err == CborErrorOutOfMemory);
  return cbor_writer<cbor_map_encoder, B>(cbor_map_encoder(encoder.getEncoder(), container), buffer);
}

}// end namespace odn
}// end namespace fc

#endif //ODNFC_CBOR_ENCODER_HPP
