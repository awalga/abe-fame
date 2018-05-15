//
// Created by abel walga on 11/05/2018.
//
#include "gtest/gtest.h"
#include "odnfc/cbor_encoder.hpp"
#include "odnfc/cbor_parser.hpp"
#include "odnfc/memory_buffer.hpp"
#include <string>

template<typename T>
using cbor_reader_memory = ::odn::fc::cbor_reader<T, ::odn::fc::memory_buffer>;
template<typename T>
using cbor_writer_memory = ::odn::fc::cbor_writer<T, ::odn::fc::memory_buffer>;

TEST (serialization, encode_array_of_int) {
  using namespace ::odn::fc;

  memory_buffer buffer;
  cbor_writer_memory<cbor_encoder> encoder(buffer);
  {
    auto array_encoder = encoder.array<cbor_array_encoder>(3);

    array_encoder.push(int64_t(12492840898));
    array_encoder.push(int64_t(24566390L));
    array_encoder.push(int64_t(3455508456L));
  }

  // check memory buffer size

  // decode
  cbor_reader_memory<cbor_parser> parser(buffer);
  {
    auto array_parser(parser.array<cbor_array_parser>(3));
    EXPECT_EQ(array_parser.pop_int(), 12492840898);
    EXPECT_EQ(array_parser.pop_int(), 24566390);
    EXPECT_EQ(array_parser.pop_int(), 3455508456);
  }
}

TEST (serialization, encode_array_of_string) {
  using namespace ::odn::fc;

  memory_buffer buffer;
  cbor_writer_memory<cbor_encoder> encoder(buffer);
  {
    auto array_encoder = encoder.array<cbor_array_encoder>(3);

    array_encoder.push(std::string("aword"));
    array_encoder.push("anotherword");
    array_encoder.push("mylastword");
  }

  // check memory buffer size

  // decode
  cbor_reader_memory<cbor_parser> parser(buffer);
  {
    auto array_parser = parser.array<cbor_array_parser>(3);
    EXPECT_EQ(array_parser.pop_string(), "aword");
    EXPECT_EQ(array_parser.pop_string(), "anotherword");
    EXPECT_EQ(array_parser.pop_string(), "mylastword");
  }
}

TEST (serialization, encode_array_of_bytes) {
  using namespace ::odn::fc;

  memory_buffer buffer;
  cbor_writer_memory<cbor_encoder> encoder(buffer);
  {
    auto array_encoder = encoder.array<cbor_array_encoder>(3);
    uint8_t buffer0[] = {'a', 'w', 'o', 'r', 'd'};
    array_encoder.push(buffer0, 5);
    uint8_t buffer1[] = {'a', 'n', 'o', 't', 'h', 'e', 'r', 'w', 'o', 'r', 'd'};
    array_encoder.push(buffer1, 11);
    uint8_t buffer2[] = {'l', 'a', 's', 't', 'w', 'o', 'r', 'd'};
    array_encoder.push(buffer2, 8);
  }

  // check memory buffer size

  // decode
  cbor_reader_memory<cbor_parser> parser(buffer);
  {
    auto array_parser = parser.array<cbor_array_parser>(3);
    uint8_t value0[5];
    array_parser.pop(value0, 5);
    EXPECT_EQ(std::string(value0, value0 + 5), "aword");

    uint8_t value1[11];
    array_parser.pop(value1);
    EXPECT_EQ(std::string(value1, value1 + 11), "anotherword");

    uint8_t value2[8];
    array_parser.pop(value2, 8);
    EXPECT_EQ(std::string(value2, value2 + 8), "lastword");
  }
}

TEST (serialization, encode_map) {
  using namespace ::odn::fc;

  memory_buffer buffer;
  cbor_writer_memory<cbor_encoder> encoder(buffer);
  {
    cbor_writer<cbor_map_encoder, memory_buffer> map_encoder = encoder.map<cbor_map_encoder>(4);
    uint8_t buffer0[] = {'a', 'w', 'o', 'r', 'd'};
    map_encoder.insert("aword", buffer0, 5);
    uint8_t buffer1[] = {'a', 'n', 'o', 't', 'h', 'e', 'r', 'w', 'o', 'r', 'd'};
    map_encoder.insert("buffer1", buffer1);
    map_encoder.insert("intvalue", int64_t(8894399734));
    map_encoder.insert("stringvalue", "mappedstring");
  }

  // check memory buffer size

  // decode
  cbor_reader_memory<cbor_parser> parser(buffer);
  {
    auto map_parser = parser.map<cbor_map_parser>(4);
    std::string key0;
    uint8_t value0[5];
    map_parser.get(&key0, value0, 5);
    EXPECT_EQ(key0, "aword");
    EXPECT_EQ(std::string(value0, value0 + 5), "aword");

    std::string key1;
    uint8_t value1[11];
    map_parser.get(&key1, value1);
    EXPECT_EQ(key1, "buffer1");
    EXPECT_EQ(std::string(value1, value1 + 11), "anotherword");

    std::string key2;
    int64_t value_int;
    map_parser.get(&key2, &value_int);
    EXPECT_EQ(value_int, int64_t(8894399734));
    EXPECT_EQ(key2, "intvalue");

    std::string key3;
    std::string value_string;
    map_parser.get(&key3, &value_string);
    EXPECT_EQ(value_string, "mappedstring");
    EXPECT_EQ(key3, "stringvalue");
  }
}