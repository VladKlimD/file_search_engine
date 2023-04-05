cmake_minimum_required(VERSION 3.5)

project(test_inverted_index LANGUAGES CXX)

enable_testing()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(test_inverted_index
    tst_test_inverted_index.cpp
    #
    ../../src/inverted_index.cpp
    #
    ../../src/json_converter.cpp
    #
    ../../src/inverted_index.cpp
    #
    ../../src/search_server.cpp)

include_directories(../../include)
add_test(NAME test_inverted_index COMMAND test_inverted_index)

target_link_libraries(test_inverted_index PRIVATE Qt${QT_VERSION_MAJOR}::Test)
