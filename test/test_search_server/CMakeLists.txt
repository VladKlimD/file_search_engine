cmake_minimum_required(VERSION 3.5)

project(test_search_server LANGUAGES CXX)

enable_testing()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(test_search_server
    tst_test_search_server.cpp
    #
    ../../src/inverted_index.cpp
    #
    ../../src/json_converter.cpp
    #
    ../../src/inverted_index.cpp
    #
    ../../src/search_server.cpp)

include_directories(../../include)
add_test(NAME test_search_server COMMAND test_search_server)

target_link_libraries(test_search_server PRIVATE Qt${QT_VERSION_MAJOR}::Test)

