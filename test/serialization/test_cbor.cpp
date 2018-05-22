//
// Created by abel walga on 11/05/2018.
//
#include "gtest/gtest.h"
#include "odnfc/cbor_encoder.hpp"
#include "odnfc/cbor_parser.hpp"
#include "odnfc/memory_buffer.hpp"
#include <string>

using cbor_reader_memory = ::odn::fc::cbor_reader_root<::odn::fc::memory_buffer>;
using cbor_writer_memory = ::odn::fc::cbor_writer_root<::odn::fc::memory_buffer>;

TEST (serialization, encode_array_of_int) {
  using namespace ::odn::fc;

  memory_buffer buffer;
  cbor_writer_memory encoder(buffer);
  {
    auto array_encoder = encoder.array(3);

    array_encoder.push<int64_t>(int64_t(12492840898));
    array_encoder.push<int64_t>(int64_t(24566390L));
    array_encoder.push<int64_t>(int64_t(3455508456L));
  }

  // check memory buffer size

  // decode
  cbor_reader_memory parser(buffer);
  {
    auto array_parser = parser.array();
    EXPECT_EQ(array_parser.length(), 3);
    EXPECT_EQ(array_parser.pop<int64_t>(), 12492840898);
    EXPECT_EQ(array_parser.pop<int64_t>(), 24566390);
    EXPECT_EQ(array_parser.pop<int64_t>(), 3455508456);
  }
}

TEST (serialization, encode_array_of_string) {
  using namespace ::odn::fc;

  memory_buffer buffer;
  cbor_writer_memory encoder(buffer);
  {
    auto array_encoder = encoder.array(3);
    array_encoder.push<std::string>(std::string("aword"));
    array_encoder.push<std::string>("anotherword");
    array_encoder.push<std::string>("mylastword");
  }

  // check memory buffer size

  // decode
  cbor_reader_memory parser(buffer);

  {
    auto array_parser = parser.array();

    EXPECT_EQ(array_parser.length(), 3);
    EXPECT_EQ(array_parser.pop<std::string>(), "aword");
    EXPECT_EQ(array_parser.pop<std::string>(), "anotherword");
    EXPECT_EQ(array_parser.pop<std::string>(), "mylastword");
  }
}

TEST (serialization, encode_array_of_bytes) {
  using namespace ::odn::fc;

  memory_buffer buffer;
  cbor_writer_memory encoder(buffer);
  {
    auto array_encoder = encoder.array(3);
    uint8_t buffer0[] = {'a', 'w', 'o', 'r', 'd'};
    array_encoder.push(buffer0);
    uint8_t buffer1[] = {'a', 'n', 'o', 't', 'h', 'e', 'r', 'w', 'o', 'r', 'd'};
    array_encoder.push(buffer1);
    uint8_t buffer2[] = {'l', 'a', 's', 't', 'w', 'o', 'r', 'd'};
    array_encoder.push(buffer2);
  }

  // check memory buffer size

  // decode
  cbor_reader_memory parser(buffer);
  {
    auto array_parser = parser.array();

    EXPECT_EQ(array_parser.length(), 3);
    uint8_t value0[5];
    array_parser.pop<decltype(value0)>(&value0);
    EXPECT_EQ(std::string(value0, value0 + 5), "aword");

    uint8_t value1[11];
    array_parser.pop<uint8_t[11]>(&value1);
    EXPECT_EQ(std::string(value1, value1 + 11), "anotherword");

    uint8_t value2[8];
    array_parser.pop(&value2);
    EXPECT_EQ(std::string(value2, value2 + 8), "lastword");
  }
}

TEST (serialization, encode_pair) {
  using namespace ::odn::fc;

  memory_buffer buffer;
  cbor_writer_memory encoder(buffer);
  {
    auto map_encoder = encoder.map(4);
    uint8_t buffer0[] = {'a', 'w', 'o', 'r', 'd'};
    map_encoder.insert<std::string, uint8_t (&)[5]>("aword", buffer0);
    uint8_t buffer1[] = {'a', 'n', 'o', 't', 'h', 'e', 'r', 'w', 'o', 'r', 'd'};
    map_encoder.insert<std::string, uint8_t (&)[11]>("buffer1", buffer1);
    map_encoder.insert<std::string, int64_t>("intvalue", int64_t(8894399734));
    map_encoder.insert<std::string, std::string>("stringvalue", "mappedstring");
  }

  // check memory buffer size

  // decode
  cbor_reader_memory parser(buffer);
  {
    auto map_parser = parser.map();

    EXPECT_EQ(map_parser.size(), 4);
    std::string key0;
    uint8_t value0[5];
    map_parser.get(&key0, &value0);
    EXPECT_EQ(key0, "aword");
    EXPECT_EQ(std::string(value0, value0 + 5), "aword");

    std::string key1;
    uint8_t value1[11];
    map_parser.get(&key1, &value1);
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

#include <vector>
TEST (serialization, encode_vector) {
  using namespace ::odn::fc;

  memory_buffer buffer;
  cbor_writer_memory encoder(buffer);
  {
    std::vector<int64_t> int_vector{12492840898L, 24566390L, 3455508456L};
    encoder.push(int_vector);
  }

  // decode
  cbor_reader_memory parser(buffer);
  {
    std::vector<int64_t> int_vector_decoded;
    parser.pop(&int_vector_decoded);

    EXPECT_EQ(int_vector_decoded, std::vector<int64_t>({12492840898L, 24566390L, 3455508456L}));
  }
}

#include <map>
TEST (serialization, encode_map) {
  using namespace ::odn::fc;

  memory_buffer buffer;
  cbor_writer_memory encoder(buffer);
  std::map<std::string, int64_t> dict;
  {
    dict.insert({std::string("12492840898L"), 12492840898L});
    dict.insert({std::string("24566390L"), 24566390L});
    dict.insert({std::string("3455508456L"), 3455508456L});
    encoder.push(dict);
  }

  // decode
  cbor_reader_memory parser(buffer);
  {
    std::map<std::string, int64_t> dict_decoded;
    parser.pop(&dict_decoded);

    EXPECT_EQ(dict_decoded, dict);
  }
}