//
// Created by abel walga on 14/05/2018.
//

#ifndef ODNFC_CBOR_PARSER_HPP
#define ODNFC_CBOR_PARSER_HPP

#include "cbor.hpp"
#include "cbor.h"

namespace odn {
namespace fc {
class cbor_map_parser {
 public :
  cbor_map_parser() = delete;
  cbor_map_parser(CborValue reader_, CborValue map_reader) noexcept
      : reader(reader_), map_reader(map_reader) {
  }
  cbor_map_parser(const cbor_map_parser &) = delete;
  cbor_map_parser(cbor_map_parser &&other) noexcept
      : reader(other.reader), map_reader(other.map_reader) {}

  cbor_map_parser &operator=(const cbor_map_parser &) = delete;
  cbor_map_parser &operator=(cbor_map_parser &&other) {
    using namespace ::std;
    swap(reader, other.reader);
    swap(map_reader, other.map_reader);
    return *this;
  }

  ~cbor_map_parser() {
    while (!cbor_value_at_end(&map_reader)) {
      cbor_value_advance(&map_reader);
    }

    cbor_value_leave_container(&reader, &map_reader);
  }

  CborValue *getReader() {
    return &reader;
  }

  CborValue *getContainer() {
    return &map_reader;
  }
 private :

  CborValue reader, map_reader;
};

class cbor_array_parser {
 public :
  cbor_array_parser() = delete;
  cbor_array_parser(CborValue reader_, CborValue array_reader_) noexcept
      : reader(reader_), array_reader(array_reader_) {
  }
  cbor_array_parser(const cbor_array_parser &) = delete;
  cbor_array_parser(cbor_array_parser &&other) noexcept
      : reader(other.reader), array_reader(other.array_reader) {}

  cbor_array_parser &operator=(const cbor_array_parser &) = delete;
  cbor_array_parser &operator=(cbor_array_parser &&other) {
    using namespace ::std;
    swap(reader, other.reader);
    swap(array_reader, other.array_reader);
    return *this;
  }

  ~cbor_array_parser() {
    while (!cbor_value_at_end(&array_reader)) {
      cbor_value_advance(&array_reader);
    }

    cbor_value_leave_container(&reader, &array_reader);
  }
  CborValue *getReader() {
    return &reader;
  }
  CborValue *getContainer() {
    return &array_reader;
  }

 private :
  CborValue reader, array_reader;
};

class cbor_parser {
 public :

  cbor_parser() noexcept : parser(), reader() {}
  cbor_parser(CborParser parser_, CborValue reader_) noexcept
      : parser(std::move(parser_)), reader(std::move(reader_)) {}
  cbor_parser(const cbor_parser &) = delete;
  cbor_parser(cbor_parser &&other) noexcept : parser(std::move(other.parser)), reader(std::move(other.reader)) {}

  CborValue *getReader() {
    return &reader;
  }

  CborParser *getParser() {
    return &parser;
  }

  ~cbor_parser() {}
 private:
  CborParser parser;
  CborValue reader;
};

template<>
struct is_array_type<cbor_array_parser> : std::true_type {
  constexpr static bool value = true;
  typedef cbor_array_parser type;
};
template<>
struct is_dictionary_type<cbor_map_parser> : std::true_type {
  constexpr static bool value = true;
  typedef cbor_map_parser type;
};

template <typename cbor_reader, typename cbor_parser_type>
class cbor_reader_base {};

template<typename cbor_reader>
class cbor_reader_base<cbor_reader, cbor_array_parser> {
 public :
  template<size_t len_>
  void pop(uint8_t (&data)[len_]) {
    auto parser = static_cast<cbor_reader *>(this)->getParser();
    auto buflen_ = len_;
    cbor_value_copy_byte_string(parser->getContainer(), data, &buflen_, parser->getContainer());
  }

  void pop(uint8_t *data, size_t len) {
    auto parser = static_cast<cbor_reader *>(this)->getParser();

    auto buflen_ = len;
    cbor_value_copy_byte_string(parser->getContainer(), data, &buflen_, parser->getContainer());
  }

  int64_t pop_int() {
    int64_t value;
    auto parser = static_cast<cbor_reader *>(this)->getParser();

    cbor_value_get_int64(parser->getContainer(), &value);
    cbor_value_advance(parser->getContainer());
    return value;
  }

  std::string pop_string() {
    auto parser = static_cast<cbor_reader *>(this)->getParser();

    size_t length = 0;
    cbor_value_get_string_length(parser->getContainer(), &length);
    std::string str_(length, '0');
    cbor_value_copy_text_string(parser->getContainer(),
                                const_cast<char *>(str_.data()),
                                &length,
                                parser->getContainer());
    return std::move(str_);
  }
};

template<typename cbor_reader>
class cbor_reader_base<cbor_reader, cbor_map_parser> {
 public :
  template<size_t len_>
  void get(std::string *key, uint8_t (&value)[len_]) {
    auto parser = static_cast<cbor_reader *>(this)->getParser();
    auto container = parser->getContainer();

    size_t length = 0;
    cbor_value_get_string_length(container, &length);
    std::string str_(length, '0');
    cbor_value_copy_text_string(container,
                                const_cast<char *>(str_.data()),
                                &length,
                                container);
    key->assign(std::move(str_));
    auto buflen_ = len_;
    cbor_value_copy_byte_string(container, value, &buflen_, container);
  }

  void get(std::string *key, uint8_t *value, const size_t len_) {
    auto parser = static_cast<cbor_reader *>(this)->getParser();
    auto container = parser->getContainer();

    size_t length = 0;
    cbor_value_get_string_length(container, &length);
    std::string str_(length, '0');
    cbor_value_copy_text_string(container,
                                const_cast<char *>(str_.data()),
                                &length,
                                container);
    key->assign(std::move(str_));
    auto buflen_ = len_;
    cbor_value_copy_byte_string(container, value, &buflen_, container);
  }

  void get(std::string *key, int64_t *value) {
    auto parser = static_cast<cbor_reader *>(this)->getParser();
    auto container = parser->getContainer();

    size_t length = 0;
    cbor_value_get_string_length(container, &length);
    std::string str_(length, '0');
    cbor_value_copy_text_string(container,
                                const_cast<char *>(str_.data()),
                                &length,
                                container);
    key->assign(std::move(str_));
    cbor_value_get_int64(container, value);
    cbor_value_advance(container);
  }
  void get(std::string *key, std::string *value) {
    auto parser = static_cast<cbor_reader *>(this)->getParser();
    auto container = parser->getContainer();

    size_t length_key = 0;
    cbor_value_get_string_length(container, &length_key);
    std::string str_key(length_key, '0');
    cbor_value_copy_text_string(container,
                                const_cast<char *>(str_key.data()),
                                &length_key,
                                container);
    key->assign(std::move(str_key));

    size_t length_value = 0;
    cbor_value_get_string_length(container, &length_value);
    std::string str_value(length_value, '0');
    cbor_value_copy_text_string(container,
                                const_cast<char *>(str_value.data()),
                                &length_value,
                                container);
    value->assign(std::move(str_value));
  }
  template<size_t len_>
  void get(const std::string &, uint8_t (&)[len_]);
  void get(const std::string &, uint8_t *, size_t);
  void get(const std::string &, int64_t *);
  void get(const std::string &, std::string *);
};

template<typename cbor_parser_Type, typename buffer_Type>
class cbor_reader : public cbor_reader_base<cbor_reader<cbor_parser_Type, buffer_Type>, cbor_parser_Type> {

 public:
  typedef cbor_parser_Type parser_type;

  template<typename T = cbor_parser_Type, typename = typename std::enable_if<std::is_default_constructible<T>{}>::type>
  cbor_reader() : parser(), buffer() {}
  cbor_reader(const cbor_reader<cbor_parser_Type, buffer_Type> &) = delete;
  template<typename = typename std::enable_if<std::is_move_constructible<cbor_parser_Type>{}>::type>
  cbor_reader(cbor_reader<cbor_parser_Type, buffer_Type> &&other)
      : parser(std::move(other.parser)), buffer(std::move(other.buffer)) {}

  template<typename T = cbor_parser_Type, typename = typename std::enable_if<std::is_default_constructible<T>{}>::type>
  explicit cbor_reader(buffer_Type buffer_) : parser(), buffer(std::move(buffer_)) {
    cbor_parser_init(buffer.begin(),
                     static_cast<size_t >(buffer.end() - buffer.begin()),
                     0,
                     parser.getParser(),
                     parser.getReader());
  }
  template<typename T = cbor_parser_Type, typename = typename std::enable_if<std::is_move_constructible<T>::value>::type>
  cbor_reader(T encoder_, buffer_Type buffer_) : parser(std::move(encoder_)), buffer(std::move(buffer_)) {}

  template<typename T, typename = typename std::enable_if<is_array_type<T>::value>::type>
  cbor_reader<T, buffer_Type> array(const size_t length = CborIndefiniteLength) {
    return array(length, is_container_type<cbor_parser_Type>{});
  }
  template<typename T, typename = typename std::enable_if<is_dictionary_type<T>::value>::type>
  cbor_reader<T, buffer_Type> map(const size_t length = CborIndefiniteLength) {
    return map(length, is_container_type<cbor_parser_Type>{});
  }

  const cbor_parser_Type *getParser() const {
    return &parser;
  }

  cbor_parser_Type *getParser() {
    return &parser;
  }
 private:
  cbor_reader<cbor_array_parser, buffer_Type> array(size_t, std::false_type);
  cbor_reader<cbor_array_parser, buffer_Type> array(size_t, std::true_type);
  cbor_reader<cbor_map_parser, buffer_Type> map(size_t, std::false_type);
  cbor_reader<cbor_map_parser, buffer_Type> map(size_t, std::true_type);

  cbor_parser_Type parser;
  buffer_Type buffer;
};

template<typename T, typename B>
cbor_reader<cbor_array_parser, B> cbor_reader<T, B>::array(size_t len, std::false_type) {
  CborValue array_reader;
  CborValue *parent = parser.getReader();
  cbor_value_enter_container(parent, &array_reader);// TODO exception
  return cbor_reader<cbor_array_parser, B>(cbor_array_parser(*parent, array_reader), buffer);
}

template<typename T, typename B>
cbor_reader<cbor_array_parser, B> cbor_reader<T, B>::array(size_t len, std::true_type) {
  CborValue array_reader;
  CborValue *parent = parser.getContainer();
  cbor_value_enter_container(parent, &array_reader);// TODO exception
  return cbor_reader<cbor_array_parser, B>(cbor_array_parser(*parent, array_reader), buffer);
}

template<typename T, typename B>
cbor_reader<cbor_map_parser, B> cbor_reader<T, B>::map(size_t len, std::false_type) {
  CborValue map_reader;
  CborValue *parent = parser.getReader();
  cbor_value_enter_container(parent, &map_reader);// TODO exception
  return cbor_reader<cbor_map_parser, B>(cbor_map_parser(*parent, map_reader), buffer);
}

template<typename T, typename B>
cbor_reader<cbor_map_parser, B> cbor_reader<T, B>::map(size_t len, std::true_type) {
  CborValue map_reader;
  CborValue *parent = parser.getContainer();
  cbor_value_enter_container(parent, &map_reader);// TODO exception
  return cbor_reader<cbor_map_parser, B>(cbor_map_parser(*parent, map_reader), buffer);
}
}// end namespace fc
}// end namespace odn
#endif //ODNFC_CBOR_PARSER_HPP
