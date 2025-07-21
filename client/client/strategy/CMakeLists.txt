cmake_minimum_required(VERSION 3.5)
project(strategy)
aux_source_directory(./ SRC)
add_library(strategy STATIC ${SRC})

target_include_directories(strategy PRIVATE ${CMAKE_SOURCE_DIR}/card)
target_include_directories(strategy PRIVATE ${CMAKE_SOURCE_DIR}/player)
target_include_directories(strategy PRIVATE ${CMAKE_SOURCE_DIR}/control)
target_include_directories(strategy PRIVATE ${CMAKE_SOURCE_DIR}/windows)
target_include_directories(strategy PRIVATE ${CMAKE_SOURCE_DIR}/thread)

target_link_libraries(strategy PRIVATE Qt5::Core)
