cmake_minimum_required(VERSION 3.5)
project(tcp)
aux_source_directory(./ SRC)
file(GLOB HEAD ./*.h)
add_library(tcp STATIC ${SRC} ${HEAD})


target_include_directories(tcp PRIVATE ${CMAKE_SOURCE_DIR}/card)
target_include_directories(tcp PRIVATE ${CMAKE_SOURCE_DIR}/player)
target_include_directories(tcp PRIVATE ${CMAKE_SOURCE_DIR}/control)
target_include_directories(tcp PRIVATE ${CMAKE_SOURCE_DIR}/windows)
target_include_directories(tcp PRIVATE ${CMAKE_SOURCE_DIR}/thread)

target_link_libraries(tcp PRIVATE Qt5::Core)
