cmake_minimum_required(VERSION 3.14)

project(Local_File_Search_Engine LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets)

add_executable(Local_File_Search_Engine
  src/main.cpp
  src/mainwindow.cpp
  include/mainwindow.h
  #
  include/json_converter.h
  src/json_converter.cpp
  #
  include/inverted_index.h
  src/inverted_index.cpp
  #
  include/search_server.h
  src/search_server.cpp
)

#Команды для копирования файлов и папок при сборке
add_custom_command(TARGET Local_File_Search_Engine POST_BUILD
    #Копирование файлов json
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/config.json
            ${CMAKE_SOURCE_DIR}/requests.json
            $<TARGET_FILE_DIR:Local_File_Search_Engine>
    #Копирование папки с txt файлами по которым нужно осуществить поиск
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/resources
            $<TARGET_FILE_DIR:Local_File_Search_Engine>/resources)

#Добавление директории include
target_include_directories(Local_File_Search_Engine PRIVATE include)

target_link_libraries(Local_File_Search_Engine PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets)

#Добавление директории тестов
add_subdirectory(test)

install(TARGETS Local_File_Search_Engine
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
