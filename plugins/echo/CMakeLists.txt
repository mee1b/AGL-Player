cmake_minimum_required(VERSION 3.16)

project(echo LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_library(echo SHARED
  echo.cpp
  echo.h
  echo.json
)

set_target_properties(echo PROPERTIES PREFIX "")

target_link_libraries(echo PRIVATE Qt${QT_VERSION_MAJOR}::Core)

target_compile_definitions(echo PRIVATE ECHO_LIBRARY)

add_custom_command(TARGET echo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
        $<TARGET_FILE_DIR:AGL-Manager>/plugins
    COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:echo>
        $<TARGET_FILE_DIR:AGL-Manager>/plugins
)
