cmake_minimum_required(VERSION 3.16.3)
project(common)
set(CMAKE_CXX_STANDARD 17)
include_directories(/usr/local/include/json)
link_directories(/usr/local/lib)
aux_source_directory(./ SRC)

add_library(common STATIC ${SRC})
target_link_libraries(common PUBLIC jsoncpp)