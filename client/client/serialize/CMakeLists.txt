cmake_minimum_required(VERSION 3.5)
project(serialize)

#link_directories("D:/protobuf/lib")

aux_source_directory(./ SRC)
file(GLOB HEAD ./*.h)
add_library(serialize STATIC ${SRC} ${HEAD})

target_link_libraries(serialize PRIVATE Qt5::Core protobufd)
target_include_directories(serialize PRIVATE ${CMAKE_SOURCE_DIR}/card)
target_include_directories(serialize PRIVATE ${CMAKE_SOURCE_DIR}/strategy)
target_include_directories(serialize PRIVATE ${CMAKE_SOURCE_DIR}/windows)
target_include_directories(serialize PRIVATE ${CMAKE_SOURCE_DIR}/control)
target_include_directories(serialize PRIVATE ${CMAKE_SOURCE_DIR}/thread)
