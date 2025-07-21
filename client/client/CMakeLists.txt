cmake_minimum_required(VERSION 3.5)
project(client LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# 必须先打开自动处理机制，且这几个都要开
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 先找 Qt5（明确版本），必须带 Widgets（否则AUTOMOC无法工作）
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Multimedia)

include_directories(
        ${PROJECT_SOURCE_DIR}/card
        ${PROJECT_SOURCE_DIR}/control
        ${PROJECT_SOURCE_DIR}/player
        ${PROJECT_SOURCE_DIR}/stratey
        ${PROJECT_SOURCE_DIR}/thread
        ${PROJECT_SOURCE_DIR}/windows
        ${PROJECT_SOURCE_DIR}/tcp
        ${PROJECT_SOURCE_DIR}/serialize
        ${PROJECT_SOURCE_DIR}/crypto
        "C:\\openssl\\w64\\OpenSSL-Win64\\include"

        "D:\\protobuf\\include"
)
#"D:\\protobuf\\include"   "D:\\c++\\Protobuf\\include"

# 先找 Qt5 后再添加子目录，确保子目录能用到 Qt5 的变量和 target
add_subdirectory(card)
add_subdirectory(control)
add_subdirectory(player)
add_subdirectory(strategy)
add_subdirectory(windows)
add_subdirectory(thread)
add_subdirectory(tcp)
add_subdirectory(serialize)
add_subdirectory(crypto)

link_directories("D:\\protobuf\\lib")
#link_directories("C:\\Program Files (x86)\\protobuf\\lib")

#添加资源文件
qt5_add_resources(RES res.qrc)

#添加图标资源文件
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    enable_language("RC")
    set(ICON_RES ${CMAKE_CURRENT_SOURCE_DIR}/ddz.rc)
endif()

if(ANDROID)
  add_library(client SHARED main.cpp)
else()
  add_executable(client
      main.cpp
      ${RES}
      ${ICON_RES}
)
endif()

# 链接 Qt5 及其他模块
target_link_libraries(client PRIVATE
    Qt5::Core
    Qt5::Widgets
    Qt5::Multimedia
    windows control player strategy card thread tcp serialize crypto ws2_32 protobufd
)
