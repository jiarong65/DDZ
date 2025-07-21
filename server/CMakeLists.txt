cmake_minimum_required(VERSION 3.16.3)
project(server)

set(CMAKE_CXX_STANDARD 17)

#包含头文件
include_directories(
        ${PROJECT_SOURCE_DIR}/common
        ${PROJECT_SOURCE_DIR}/http
        ${PROJECT_SOURCE_DIR}/reactor
        ${PROJECT_SOURCE_DIR}/tcp
        ${PROJECT_SOURCE_DIR}/thread
        ${PROJECT_SOURCE_DIR}/serialize
        ${PROJECT_SOURCE_DIR}/crypto
        ${PROJECT_SOURCE_DIR}/game
        ${PROJECT_SOURCE_DIR}/database
        /usr/local/mysql/include
        ~/package/openssl-1.1.1c/include
)

#添加子目录
add_subdirectory(common)
add_subdirectory(http)
add_subdirectory(reactor)
add_subdirectory(tcp)
add_subdirectory(thread)
add_subdirectory(crypto)
add_subdirectory(serialize)
add_subdirectory(game)
add_subdirectory(database)

link_directories(
        /home/jiarong/package/openssl-1.1.1c/
        /home/jiarong/package/protobuf-3.21.12/src/.libs
        /usr/local/mysql/lib
)

link_libraries(tcp http reactor thread common serialize mycrypto crypto game database)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_executable(server main.cpp)
