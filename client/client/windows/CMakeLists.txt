cmake_minimum_required(VERSION 3.5)
project(windows)

set(PROJECT_SOURCE
       login.cpp
       login.h
       login.ui
)

aux_source_directory(./ SRC)
add_library(windows STATIC ${SRC})

target_link_libraries(windows PRIVATE Qt5::Core
    Qt5::Widgets
    Qt5::Multimedia
)
target_include_directories(windows PRIVATE ${CMAKE_SOURCE_DIR}/card)
target_include_directories(windows PRIVATE ${CMAKE_SOURCE_DIR}/player)
target_include_directories(windows PRIVATE ${CMAKE_SOURCE_DIR}/control)
target_include_directories(windows PRIVATE ${CMAKE_SOURCE_DIR}/strategy)
target_include_directories(windows PRIVATE ${CMAKE_SOURCE_DIR}/thread)
