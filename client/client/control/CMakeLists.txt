cmake_minimum_required(VERSION 3.5)
project(control)

set(PROJECT_SOURCE
       datamanager.cpp
       datamanager.h
)

aux_source_directory(./ SRC)
add_library(control STATIC ${SRC})

target_link_libraries(control PRIVATE Qt5::Core
    Qt5::Multimedia
)

target_include_directories(control PRIVATE ${CMAKE_SOURCE_DIR}/card)
target_include_directories(control PRIVATE ${CMAKE_SOURCE_DIR}/strategy)
target_include_directories(control PRIVATE ${CMAKE_SOURCE_DIR}/windows)
target_include_directories(control PRIVATE ${CMAKE_SOURCE_DIR}/thread)
target_include_directories(control PRIVATE ${CMAKE_SOURCE_DIR}/player)

