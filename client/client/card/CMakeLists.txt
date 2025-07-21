cmake_minimum_required(VERSION 3.5)
project(card)

aux_source_directory(./ SRC)
add_library(card STATIC ${SRC})

target_link_libraries(card PRIVATE Qt5::Core)
