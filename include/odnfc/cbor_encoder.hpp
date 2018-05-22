//
// Created by abel walga on 13/05/2018.
//

#ifndef ODNFC_CBOR_ENCODER_HPP
#define ODNFC_CBOR_ENCODER_HPP

#include "cbor.hpp"
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
#include <map>

namespace odn {
namespace fc {

template<typename cbor_writer>
class cbor_encoder_impl {

 public :
  CborError _encode0(CborEncoder *encoder, unsigned char &o) {
    return cbor_encode_uint(encoder, o);
  }

  CborError _encode0(CborEncoder *encoder, const unsigned char &o) {
    return cbor_encode_uint(encoder, o);
  }

  CborError _encode0(CborEncoder *encoder, long long &o) {
    return cbor_encode_int(encoder, o);
  }

  CborError _encode0(CborEncoder *encoder, const long long &o) {
    return cbor_encode_int(encoder, o);
  }

  CborError _encode0(CborEncoder *encoder, unsigned long &o) {
    return cbor_encode_uint(encoder, o);
  }

  CborError _encode0(CborEncoder *encoder, const unsigned long &o) {
    return cbor_encode_uint(encoder, o);
  }

  CborError _encode0(CborEncoder *encoder, const std::string &o) {
    return cbor_encode_text_stringz(encoder, o.c_str());
  }

  CborError _encode0(CborEncoder *encoder, std::string &o) {
    return cbor_encode_text_stringz(encoder, o.c_str());
  }

  template<typename T>
  CborError _encode0(CborEncoder *encoder, T &&o) {
    auto _writer = static_cast<cbor_writer *>(this);
    serialize(*_writer->writer_interface(), const_cast<const T &>(o));// TODO propagate error
    return CborNoError;
  }

  CborError _encode_array0(CborEncoder *encoder, const unsigned char *o, size_t size) {
    return cbor_encode_byte_string(encoder, o, size);
  }

  CborError _encode_array0(CborEncoder *encoder, unsigned char *o, size_t size) {
    return cbor_encode_byte_string(encoder, o, size);
  }

  template<typename T>
  CborError _encode_array0(CborEncoder *encoder, const T *o, size_t size) {
    auto _writer = static_cast<cbor_writer *>(this);
    auto _array_writer = _writer->array_writer(size);
    for (size_t i = 0; i < size; ++i) {
      _array_writer.push(*(o + static_cast<ptrdiff_t >(i)));
    }
    return CborNoError;// TODO propagate error
  }

  template<class T, size_t size>
  CborError _encode_array_1(CborEncoder *encoder, const T (&o)[size]) {
    return _encode_array0(encoder, o, size);
  }

  template<typename T>
  CborError _dispatch(CborEncoder *encoder, T &&o, std::false_type) {
    return _encode0(encoder, o);
  }

  template<typename T>
  CborError _dispatch(CborEncoder *encoder, T &&o, std::true_type) {
    return _encode_array_1(encoder, std::forward<T>(o));
  }

  template<typename T>
  CborError _encode(CborEncoder *encoder, const std::vector<T> &o) {
    auto _writer = static_cast<cbor_writer *>(this);
    auto _array_writer = _writer->array_writer(o.size());
    for (auto &_element : o) {
      _array_writer.push(_element);
    }
    return CborNoError;// TODO propagate error
  }

  template<typename T>
  CborError _encode(CborEncoder *encoder, std::vector<T> &o) {
    auto _writer = static_cast<cbor_writer *>(this);
    auto _array_writer = _writer->array_writer(o.size());
    for (auto &_element : o) {
      _array_writer.push(_element);
    }
    return CborNoError;// TODO propagate error
  }

  template<typename K, typename V>
  CborError _encode(CborEncoder *encoder, const std::map<K, V> &o) {
    auto _writer = static_cast<cbor_writer *>(this);
    auto _map_writer = _writer->map_writer(o.size());
    for (auto &_element : o) {
      _map_writer.insert(_element.first, _element.second);
    }
    return CborNoError;// TODO propagate error
  }

  template<typename K, typename V>
  CborError _encode(CborEncoder *encoder, std::map<K, V> &o) {
    auto _writer = static_cast<cbor_writer *>(this);
    auto _map_writer = _writer->map_writer(o.size());
    for (auto &_element : o) {
      _map_writer.insert(_element.first, _element.second);
    }
    return CborNoError;// TODO propagate error
  }

  template<typename T>
  CborError _encode(CborEncoder *encoder, T &&o) {
    return _dispatch<T>(encoder,
                        std::forward<T>(o),
                        std::is_array<std::remove_cv_t<typename std::remove_reference<T>::type>>{});
  }
};
/**
 *
 * @tparam cbor_writer
 * @tparam cbor_container_type
 */
template<typename cbor_writer, typename cbor_container_type>
class cbor_writer_base;
template<typename cbor_writer>
class cbor_writer_base<cbor_writer, cbor_container>
    : public cbor_encoder_impl<cbor_writer_base<cbor_writer, cbor_container>> {
 public :
  auto array_writer(const size_t length = CborIndefiniteLength) {
    return static_cast<cbor_writer *>(this)->array(length);
  }

  auto map_writer(const size_t length = CborIndefiniteLength) {
    return static_cast<cbor_writer *>(this)->map(length);
  }

  auto writer_interface() {
    return static_cast<cbor_writer *> (this);
  }

  template<typename ValueType>
  void push(ValueType &&data) {
    auto _writer = static_cast<cbor_writer *>(this);
    auto _array_writer = _writer->array(1);
    _array_writer.push(std::forward<ValueType>(data));
  }

  template<typename KeyType, typename ValueType>
  void insert(KeyType &&key, ValueType &&value) {
    auto _writer = static_cast<cbor_writer *>(this);
    auto _map_writer = _writer->map(1);
    _map_writer.insert(std::forward<ValueType>(key), std::forward<ValueType>(value));
  }
};

/**
 *
 * @tparam cbor_writer
 */
template<typename cbor_writer>
class cbor_writer_base<cbor_writer, cbor_array_container>
    : public cbor_encoder_impl<cbor_writer_base<cbor_writer, cbor_array_container>> {

 public:
  auto array_writer(const size_t length = CborIndefiniteLength) {
    return static_cast<cbor_writer *>(this)->array(length);
  }

  auto map_writer(const size_t length = CborIndefiniteLength) {
    return static_cast<cbor_writer *>(this)->map(length);
  }

  auto writer_interface() {
    return static_cast<cbor_writer *>(this);
  }

  template<typename ValueType>
  void push(ValueType &&data) {
    auto encoder = static_cast<cbor_writer *>(this)->getEncoder();
    auto container = encoder->getContainer();
    auto buffer = static_cast<cbor_writer *>(this)->getBuffer();
    CborError err;
    do {
      const CborEncoder tag(*container);
      err = this->_encode(container, data);
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

/**
 *
 * @tparam cbor_writer
 */
template<typename cbor_writer>
class cbor_writer_base<cbor_writer, cbor_map_container>
    : public cbor_encoder_impl<cbor_writer_base<cbor_writer, cbor_map_container>> {

 public :
  auto array_writer(const size_t length = CborIndefiniteLength) {
    return static_cast<cbor_writer *>(this)->array(length);
  }

  auto map_writer(const size_t length = CborIndefiniteLength) {
    return static_cast<cbor_writer *>(this)->map(length);
  }

  auto writer_interface() {
    return static_cast<cbor_writer *> (this);
  }

  template<typename KeyType, typename ValueType>
  void insert(KeyType &&key, ValueType &&value) {
    auto encoder = static_cast<cbor_writer *>(this)->getEncoder();
    auto container = encoder->getContainer();
    auto buffer = static_cast<cbor_writer *>(this)->getBuffer();
    CborError err;
    do {
      const CborEncoder tag(*container);
      err = this->_encode(container, key);
      err = this->_encode(container, value);// compute needed bytes for both insert TODO if not oom interrupt
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

/**
 *
 * @tparam cbor_container_type
 * @tparam buffer_type
 */
template<typename cbor_container_type, typename buffer_type>
class cbor_writer : public cbor_writer_base<cbor_writer<cbor_container_type, buffer_type>, cbor_container_type> {

 public:
  cbor_writer() : encoder(), buffer() {}
  cbor_writer(const cbor_writer<cbor_container_type, buffer_type> &) = delete;
  cbor_writer(cbor_writer<cbor_container_type, buffer_type> &&other) noexcept
      : encoder(std::move(other.encoder)), buffer(std::move(other.buffer)) {}

  explicit cbor_writer(buffer_type &&buffer_) : encoder(), buffer(buffer_) {
    cbor_encoder_init(encoder.getContainer(),
                      buffer.begin(),
                      static_cast<size_t>(buffer.end() - buffer.begin()),
                      0);// TODO check error
  }

  explicit cbor_writer(const buffer_type &buffer_) : encoder(), buffer(buffer_) {
    cbor_encoder_init(encoder.getContainer(),
                      buffer.begin(),
                      static_cast<size_t>(buffer.end() - buffer.begin()),
                      0);// TODO check error
  }

  cbor_writer(cbor_encoder_cursor encoder_, buffer_type buffer_) noexcept
      : encoder(std::move(encoder_)), buffer(std::move(buffer_)) {}

  cbor_writer<cbor_array_container, buffer_type> array(const size_t length = CborIndefiniteLength) {
    CborError err;
    CborEncoder container;
    do {
      auto parent = encoder.getContainer();
      CborEncoder tag(*parent);
      err = cbor_encoder_create_array(parent, &container, length);
      if (err == CborErrorOutOfMemory) {
        encoder.roll_back_state(&tag);
        auto offset = parent->data.ptr - buffer.begin();
        buffer.realloc();
        parent->data.ptr = buffer.begin() + offset;
        parent->end = buffer.end();
      }
    } while (err == CborErrorOutOfMemory);
    return cbor_writer<cbor_array_container, buffer_type>(cbor_encoder_cursor(encoder.getContainer(), container),
                                                          buffer);
  }

  cbor_writer<cbor_map_container, buffer_type> map(const size_t size = CborIndefiniteLength) {
    CborError err;
    CborEncoder container;
    do {
      auto parent = encoder.getContainer();
      CborEncoder tag(*parent);
      err = cbor_encoder_create_map(parent, &container, size);
      if (err == CborErrorOutOfMemory) {
        encoder.roll_back_state(&tag);
        auto offset = parent->data.ptr - buffer.begin();
        buffer.realloc();
        parent->data.ptr = buffer.begin() + offset;
        parent->end = buffer.end();
      }
    } while (err == CborErrorOutOfMemory);
    return cbor_writer<cbor_map_container, buffer_type>(cbor_encoder_cursor(encoder.getContainer(), container), buffer);
  }

  const cbor_encoder_cursor *getEncoder() const {
    return &encoder;
  }

  cbor_encoder_cursor *getEncoder() {
    return &encoder;
  }

  const buffer_type *getBuffer() const {
    return &buffer;
  }

  buffer_type *getBuffer() {
    return &buffer;
  }

 private:
  cbor_encoder_cursor encoder;
  buffer_type buffer;
};
template<typename bufferType>
using cbor_writer_root = cbor_writer<cbor_container, bufferType>;
}// end namespace odn
}// end namespace fc

#endif //ODNFC_CBOR_ENCODER_HPP
