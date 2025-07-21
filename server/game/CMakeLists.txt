cmake_minimum_required(VERSION 3.16.3)
project(game)
set(CMAKE_CXX_STANDARD 17)

aux_source_directory(./ SRC)

add_library(game STATIC ${SRC})
target_link_libraries(game mycrypto common hiredis redis++)