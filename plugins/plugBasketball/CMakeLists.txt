cmake_minimum_required(VERSION 3.16)

project(plugBasketball LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Gui)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui)

add_library(plugBasketball SHARED
  basketball.cpp
  basketball.h
  plugBasketball.json
  engine_func.h
  enums.h
  games.h
  history.h
  structs.h
  namespace.h
  basketball_interface.h
  plugclass.h plugclass.cpp

)

target_link_libraries(plugBasketball PRIVATE Qt${QT_VERSION_MAJOR}::Gui)

target_compile_definitions(plugBasketball PRIVATE PLUGBASKETBALL_LIBRARY)
