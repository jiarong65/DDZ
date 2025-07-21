cmake_minimum_required(VERSION 3.5)
project(crypto)
#include_directories("C:\\openssl\\w64\\OpenSSL-Win64\\include")
aux_source_directory(./ SRC)
file(GLOB HEAD ./*.h)

add_library(crypto STATIC ${SRC} ${HEAD})


target_include_directories(crypto PRIVATE ${CMAKE_SOURCE_DIR}/card)
target_include_directories(crypto PRIVATE ${CMAKE_SOURCE_DIR}/player)
target_include_directories(crypto PRIVATE ${CMAKE_SOURCE_DIR}/control)
target_include_directories(crypto PRIVATE ${CMAKE_SOURCE_DIR}/windows)
target_include_directories(crypto PRIVATE ${CMAKE_SOURCE_DIR}/thread)

target_link_libraries(crypto PRIVATE Qt5::Core
    "C:/openssl/w64/OpenSSL-Win64/lib/libcrypto.lib")
