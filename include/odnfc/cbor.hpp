//
// Created by abel walga on 14/05/2018.
//

#ifndef ODNFC_CBOR_HPP
#define ODNFC_CBOR_HPP
namespace odn {
namespace fc {
template<typename cbor_parser_Type>
struct is_container_type : std::false_type {
  constexpr static bool value = false;
};
template<typename cbor_parser_Type>
struct is_array_type : std::false_type {
  constexpr static bool value = false;
};
template<typename cbor_parser_Type>
struct is_dictionary_type : std::false_type {
  constexpr static bool value = false;
};
}// end namespace fc
}// end namespace odn
#endif //ODNFC_CBOR_HPP
