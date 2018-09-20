cmake_minimum_required(VERSION 3.10)

################################
## Project
################################
project(odnfc)

set(PROJECT_VERSION_MAJOR "0")
set(PROJECT_VERSION_MINOR "1")
set(PROJECT_VERSION_PATCH "0")
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
set(VERSION ${PROJECT_VERSION})
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

message("\n-- Configuring ${PROJECT_NAME} ${PROJECT_VERSION}...\n")

################################
## Cmake dependencies
################################
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)

include(GNUInstallDirs)
include(cmake/CPM.cmake)

################################
## Compiler Flags
################################
set(CMAKE_CXX_STANDARD 14)

################################
## Dependencies
################################
include(cmake/Findsodium.cmake)
include(cmake/Findrelic.cmake)
include(cmake/Findtinycbor.cmake)
################################
# Library Sources
################################
# src
set(ODN_CRYPTO_SOURCES
        src/binary_policy_tree.cpp
        src/msp.cpp
        src/group.cpp
        src/group_arithmetics.cpp
        src/bilinear_pairing.cpp
        src/relic_util.cpp
        src/cpabe_msk.cpp
        src/cpabe_pk.cpp
        src/cpabe_sk.cpp
        src/cpabe_cipher_text.cpp
        src/odn_abe.cpp
        src/sscipher.cpp
        src/sscipher_enc_stream.cpp
        src/sscipher_dec_stream.cpp
        src/sscipher_session.cpp
        )

################################
# Library Build
################################
add_library(odnfc ${ODN_CRYPTO_SOURCES})
target_include_directories(odnfc
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:include>)
target_link_libraries(odnfc tinycbor relic sodium)

################################
## Install
################################
# 'make install' to the correct locations (provided by GNUInstallDirs).
install(TARGETS odnfc EXPORT odnfc-config
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})  # This is for Windows
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT odnfc-config DESTINATION share/odnfc/cmake)
export(TARGETS odnfc FILE odnfc-config.cmake)

################################
## Test
################################
enable_testing()
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)
