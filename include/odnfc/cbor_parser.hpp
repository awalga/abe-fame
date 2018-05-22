//
// Created by abel walga on 14/05/2018.
//

#ifndef ODNFC_CBOR_PARSER_HPP
#define ODNFC_CBOR_PARSER_HPP

#include "cbor.hpp"
#include <memory>

namespace odn {
namespace fc {
/**
 *
 * @tparam cbor_reader
 */
template<typename cbor_reader>
class cbor_parser_impl {

 public :

  CborError _decode0(CborValue *value, long long &o) {
    CborError err = cbor_value_get_int64(value, &o);
    return err == CborNoError ? cbor_value_advance(value) : err;
  }

  CborError _decode0(CborValue *value, unsigned char &o) {
    unsigned long long _o;
    CborError err = cbor_value_get_uint64(value, &_o);
    o = _o;// TODO check u ll size
    return err == CborNoError ? cbor_value_advance(value) : err;
  }

  CborError _decode0(CborValue *value, unsigned long &o) {
    unsigned long long _o;
    CborError err = cbor_value_get_uint64(value, &_o);
    o = _o;// TODO check u ll size
    return err == CborNoError ? cbor_value_advance(value) : err;
  }

  CborError _decode0(CborValue *value, std::string &o) {
    size_t length = 0;
    cbor_value_get_string_length(value, &length);
    std::string _str(length, '0');
    CborError err = cbor_value_copy_text_string(value,
                                                const_cast<char *>(_str.data()),
                                                &length,
                                                value);
    o.assign(std::move(_str));
    return err;
  }

  template<typename T>
  CborError _decode0(CborValue *value, T &o) {
    auto _reader = static_cast<cbor_reader *>(this);
    deserialize(*_reader->reader_interface(), o);// TODO propagate loop error
    return CborNoError;
  }

  CborError _decode_array0(CborValue *value, unsigned char *o, size_t size) {
    return cbor_value_copy_byte_string(value, o, &size, value);
  }

  template<typename T>
  CborError _decode_array0(CborValue *value, T *o, size_t size) {
    auto _reader = static_cast<cbor_reader *>(this);
    auto _array_reader = _reader->array_reader();

    for (size_t i = 0; i < size; ++i) {
      _array_reader.pop((o + static_cast<ptrdiff_t >(i)));
    }

    return CborNoError;// TODO propagate loop error
  }

  template<typename T, size_t size>
  CborError _decode_array1(CborValue *value, T(&o)[size]) {
    return _decode_array0(value, o, size);
  }

  template<typename T>
  CborError _dispatch(CborValue *value, T &o, std::false_type) {
    return _decode0(value, o);
  }

  template<typename T>
  CborError _dispatch(CborValue *value, T &o, std::true_type) {
    return _decode_array1(value, o);
  }

  template<typename T>
  CborError _dispatch_decode_vector(CborValue *value, std::vector<T> &o, std::true_type) {
    auto _reader = static_cast<cbor_reader *>(this);
    auto _array_reader = _reader->array_reader();

    for (size_t i = 0, _length = _array_reader.length(); i < _length; ++i) {
      T _temp;
      _array_reader.pop(&_temp);
      o.push_back(std::move(_temp));
    }

    return CborNoError;// TODO propagate loop error
  }

  template<typename T>
  CborError _dispatch_decode_vector(CborValue *value, std::vector<T> &o, std::false_type) {
    auto _reader = static_cast<cbor_reader *>(this);
    auto _array_reader = _reader->array_reader();

    serializer<decltype(_array_reader), std::vector<T>>()(_array_reader, o);// TODO propagate loop error
    return CborNoError;
  }

  template<typename T>
  CborError _decode(CborValue *value, std::vector<T> &o) {
    return _dispatch_decode_vector(value, o, std::is_default_constructible<T>{});
  }

  template<typename K, typename V>
  CborError _dispatch_decode_map(CborValue *value, std::map<K, V> &o, std::true_type) {
    auto _reader = static_cast<cbor_reader *>(this);
    auto _map_reader = _reader->map_reader();

    for (size_t i = 0, _size = _map_reader.size(); i < _size; ++i) {
      K _tempKey;
      V _tempValue;
      _map_reader.get(&_tempKey, &_tempValue);
      o.insert({std::move(_tempKey), std::move(_tempValue)});
    }
    return CborNoError;// TODO propagate loop error
  }

  template<typename K, typename V>
  CborError _dispatch_decode_map(CborValue *value, std::map<K, V> &o, std::false_type) {
    auto _reader = static_cast<cbor_reader *>(this);
    auto _map_reader = _reader->map_reader();

    serializer<decltype(_map_reader), std::map<K, V> >()(_map_reader, o);// TODO propagate loop error

    return CborNoError;
  }

  template<typename K, typename V>
  CborError _decode(CborValue *value, std::map<K, V> &o) {
    return _dispatch_decode_map(value,
                                o,
                                std::is_default_constructible<K>{});// TODO both key and value must be default constructible
  }

  template<typename T>
  CborError _decode(CborValue *value, T &&o) {
    return _dispatch<T>(value,
                        std::forward<T>(o),
                        std::is_array<std::remove_cv_t<typename std::remove_reference<T>::type>>{});
  }

  size_t _array_length(CborValue *value) {
    size_t _size = 0;
    cbor_value_get_array_length(value, &_size);
    return _size;
  }

  size_t _map_length(CborValue *value) {
    size_t _size = 0;
    cbor_value_get_map_length(value, &_size);
    return _size;
  }
};

/**
 *
 * @tparam cbor_reader_type
 * @tparam cbor_container_type
 */
template<typename cbor_reader_type, typename cbor_container_type>
class cbor_reader_base;

/**
 *
 * @tparam cbor_reader_type
 */
template<typename cbor_reader_type>
class cbor_reader_base<cbor_reader_type, cbor_container>
    : public cbor_parser_impl<cbor_reader_base<cbor_reader_type, cbor_container>> {
 public :
  auto array_reader() {
    auto _reader = static_cast<cbor_reader_type *>(this);

    return _reader->array();
  }

  auto map_reader() {
    auto _reader = static_cast<cbor_reader_type *>(this);

    return _reader->map();
  }

  auto reader_interface() {
    return static_cast<cbor_reader_type * >(this);
  }

  template<typename ValueType>
  void pop(ValueType *o) {
    auto _reader = static_cast<cbor_reader_type *>(this);
    auto _array_reader = _reader->array();
    _array_reader.pop(o);
  }
  template<typename ValueType>
  ValueType pop() {
    auto _reader = static_cast<cbor_reader_type *>(this);
    auto _array_reader = _reader->array();
    return _array_reader.template pop<ValueType>();
  }

  template<typename KeyType, typename ValueType>
  void get(KeyType *key, ValueType *value) {
    auto _reader = static_cast<cbor_reader_type *>(this);
    auto _map_reader = _reader->map();
    _map_reader.get(key, value);
  }

  template<typename KeyType, typename ValueType>
  std::pair<KeyType, ValueType> get() {
    auto _reader = static_cast<cbor_reader_type *>(this);
    auto _map_reader = _reader->map();
    return _map_reader.template get<KeyType, ValueType>();
  }
};

/**
 *
 * @tparam cbor_reader_type
 */
template<typename cbor_reader_type>
class cbor_reader_base<cbor_reader_type, cbor_array_container>
    : public cbor_parser_impl<cbor_reader_base<cbor_reader_type, cbor_array_container>> {

 private:
  template<typename ValueType>
  ValueType _pop(std::true_type) {
    ValueType o;
    auto parser = static_cast<cbor_reader_type *>(this)->getParser();
    this->_decode(parser->getContainer(), o);
    return std::move(o);
  }

  template<typename ValueType>
  ValueType _pop(std::false_type) {
    auto _reader = static_cast<cbor_reader_type *>(this);
    return adl_serializer<decltype(*_reader), ValueType>()(*_reader);
  }

 public :
  auto array_reader() {
    auto _reader = static_cast<cbor_reader_type *>(this);

    return _reader->array();
  }

  auto map_reader() {
    auto _reader = static_cast<cbor_reader_type *>(this);

    return _reader->map();
  }

  auto reader_interface() {
    return static_cast<cbor_reader_type * >(this);
  }

  template<typename ValueType>
  void pop(ValueType *o) {
    auto parser = static_cast<cbor_reader_type *>(this)->getParser();
    this->_decode(parser->getContainer(), *o);
  }

  template<typename ValueType>
  ValueType pop() {
    return _pop<ValueType>(std::is_default_constructible<ValueType>{});
  }

  size_t length() {
    auto parser = static_cast<cbor_reader_type *>(this)->getParser();
    return this->_array_length(parser->getParent());
  }
};

/**
 *
 * @tparam cbor_reader_type
 */
template<typename cbor_reader_type>
class cbor_reader_base<cbor_reader_type, cbor_map_container>
    : public cbor_parser_impl<cbor_reader_base<cbor_reader_type, cbor_map_container>> {
 private:
  template<typename T>
  T _get_decoded(std::true_type) {
    auto parser = static_cast<cbor_reader_type *>(this)->getParser();
    T o;
    this->_decode(parser->getContainer(), o);
    return o;
  }

  template<typename T>
  T _get_decoded(std::false_type) {
    auto _reader = static_cast<cbor_reader_type *>(this);
    return adl_serializer<decltype(*_reader), T>()(*_reader);
  }

  template<typename KeyType, typename ValueType>
  ValueType _get(KeyType *key) {
    auto parser = static_cast<cbor_reader_type *>(this)->getParser();
    this->_decode(parser->getContainer(), *key);
    return _get_decoded<ValueType>(std::is_default_constructible<ValueType>{});
  }

  template<typename KeyType, typename ValueType>
  std::pair<KeyType, ValueType> _get() {
    auto parser = static_cast<cbor_reader_type *>(this)->getParser();
    KeyType key = _get_decoded<KeyType>(std::is_default_constructible<KeyType>{});
    ValueType value = _get_decoded<ValueType>(std::is_default_constructible<ValueType>{});
    return {key, value};
  }

 public :
  auto array_reader() {
    auto _reader = static_cast<cbor_reader_type *>(this);

    return _reader->array();
  }

  auto map_reader() {
    auto _reader = static_cast<cbor_reader_type *>(this);

    return _reader->map();
  }

  auto reader_interface() {
    return static_cast<cbor_reader_type * >(this);
  }

  template<typename KeyType, typename ValueType>
  void get(KeyType *key, ValueType *value) {
    auto parser = static_cast<cbor_reader_type *>(this)->getParser();
    this->_decode(parser->getContainer(), *key);
    this->_decode(parser->getContainer(), *value);
  }

  template<typename KeyType, typename ValueType>
  std::pair<KeyType, ValueType> get() {
    return _get<KeyType, ValueType>();
  }

  size_t size() {
    auto parser = static_cast<cbor_reader_type *>(this)->getParser();
    return this->_map_length(parser->getParent());
  }
};

using namespace ::std;
/**
 *
 * @tparam cbor_container_type
 * @tparam buffer_type
 */
template<typename cbor_container_type, typename buffer_type>
class cbor_reader : public cbor_reader_base<cbor_reader<cbor_container_type, buffer_type>, cbor_container_type> {
 public :
  cbor_reader() : parser(), cursor(make_unique<cbor_parser_cursor>()), buffer() {}
  cbor_reader(const cbor_reader<cbor_container_type, buffer_type> &) = delete;
  cbor_reader(cbor_reader<cbor_container_type, buffer_type> &&other) noexcept
      : parser(std::move(other.parser)), cursor(std::move(other.cursor)), buffer(std::move(other.buffer)) {}

  explicit cbor_reader(buffer_type &&buffer_) : parser(), cursor(make_unique<cbor_parser_cursor>()), buffer(buffer_) {
    cbor_parser_init(buffer.begin(),
                     static_cast<size_t >(buffer.end() - buffer.begin()),
                     0,
                     &parser,
                     cursor->getContainer());
  }

  explicit cbor_reader(const buffer_type &buffer_) : parser(), cursor(make_unique<cbor_parser_cursor>()), buffer(buffer_) {
    cbor_parser_init(buffer.begin(),
                     static_cast<size_t >(buffer.end() - buffer.begin()),
                     0,
                     &parser,
                     cursor->getContainer());
  }

  cbor_reader(CborParser parser_, unique_ptr<cbor_parser_cursor> cursor_, buffer_type buffer_) noexcept
      : parser(parser_), cursor(std::move(cursor_)), buffer(std::move(buffer_)) {

  }

  cbor_reader<cbor_array_container, buffer_type> array() {
    CborValue array_reader;
    CborValue *parent = cursor->getContainer();
    cbor_value_enter_container(parent, &array_reader);// TODO check error

    return cbor_reader<cbor_array_container, buffer_type>(parser, make_unique<cbor_parser_cursor>(array_reader, parent), buffer);
  }

  cbor_reader<cbor_map_container, buffer_type> map() {
    CborValue map_reader;
    CborValue *parent = cursor->getContainer();
    cbor_value_enter_container(parent, &map_reader);// TODO check error
    return cbor_reader<cbor_map_container, buffer_type>(parser, make_unique<cbor_parser_cursor>(map_reader, parent), buffer);
  }

  const cbor_parser_cursor *getParser() const {
    return cursor.get();
  }

  cbor_parser_cursor *getParser() {
    return cursor.get();
  }

  const buffer_type *getBuffer() const {
    return &buffer;
  }

  buffer_type *getBuffer() {
    return &buffer;
  }

 private:
  CborParser parser;
  unique_ptr<cbor_parser_cursor> cursor;
  buffer_type buffer;
};
template<typename bufferType>
using cbor_reader_root = cbor_reader<cbor_container, bufferType>;
}// end namespace fc
}// end namespace odn
#endif //ODNFC_CBOR_PARSER_HPP
