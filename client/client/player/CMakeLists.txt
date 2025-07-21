cmake_minimum_required(VERSION 3.5)
project(player)

aux_source_directory(./ SRC)
add_library(player STATIC ${SRC})

target_link_libraries(player PRIVATE Qt5::Core)
target_include_directories(player PRIVATE ${CMAKE_SOURCE_DIR}/card)
target_include_directories(player PRIVATE ${CMAKE_SOURCE_DIR}/strategy)
target_include_directories(player PRIVATE ${CMAKE_SOURCE_DIR}/windows)
target_include_directories(player PRIVATE ${CMAKE_SOURCE_DIR}/control)
target_include_directories(player PRIVATE ${CMAKE_SOURCE_DIR}/thread)
