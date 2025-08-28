cmake_minimum_required(VERSION 3.16)

project(Basketball LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Gui)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)

add_library(Basketball SHARED
  basketball.cpp
  basketball.h
  Basketball.json
  enums.h
  structs.h
  namespaces.h
)

set_target_properties(Basketball PROPERTIES PREFIX "")

target_link_libraries(Basketball PRIVATE Qt${QT_VERSION_MAJOR}::Gui)
target_link_libraries(Basketball PRIVATE Qt${QT_VERSION_MAJOR}::Test)

target_compile_definitions(Basketball PRIVATE BASKETBALL_LIBRARY)

add_custom_command(TARGET Basketball POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
        $<TARGET_FILE_DIR:AGL-Player>/plugins
    COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:Basketball>
        $<TARGET_FILE_DIR:AGL-Player>/plugins
)
