cmake_minimum_required(VERSION 3.5)
project(thread)

aux_source_directory(./ SRC)

add_library(thread STATIC ${SRC})

target_link_libraries(thread PRIVATE Qt5::Core)
target_include_directories(thread PRIVATE ${CMAKE_SOURCE_DIR}/player)
target_include_directories(thread PRIVATE ${CMAKE_SOURCE_DIR}/card)
target_include_directories(thread PRIVATE ${CMAKE_SOURCE_DIR}/control)
target_include_directories(thread PRIVATE ${CMAKE_SOURCE_DIR}/strategy)
target_include_directories(thread PRIVATE ${CMAKE_SOURCE_DIR}/windows)
