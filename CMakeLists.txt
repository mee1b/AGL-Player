cmake_minimum_required(VERSION 3.16)

project(AGL-Manager VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)

set(PROJECT_SOURCES
        main.cpp
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  message(STATUS "Using Visual Studio C++ compiler, add Talk.dll")
  # Add Visual Studio specific configurations here
  add_subdirectory(talk)
endif()

add_executable(AGL-Manager
    ${PROJECT_SOURCES}
    talkwrap.h
    talkwrap.cpp
    topwindow.h
    topwindow.cpp
    topwindow.ui
    manager.h
    manager.cpp
    manager.ui
    GameInterface.h
    resources.qrc
    NVDAController.h
    NVDAController.cpp
    utilities.h
    utilities.cpp
    TODO.txt
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_definitions(AGL-Manager PRIVATE USE_TALK_DLL)
    target_link_libraries(AGL-Manager PRIVATE Talk)
endif()


add_subdirectory(plugins)

target_include_directories(AGL-Manager PRIVATE plugins/echo)


find_path(ECHO_INCLUDE_DIR echo.h)
find_library(ECHO_LIBRARY libecho.dll)
if (ECHO_INCLUDE_DIR AND ECHO_LIBRARY)
    set(ECHO_FOUND TRUE)
    include_directories(${ECHO_INCLUDE_DIR})
    set(PATH_TO_MY_LIB ${PATH_TO_MY_LIB} ${ECHO_LIBRARY})
endif()
target_link_libraries(AGL-Manager PRIVATE ${PATH_TO_MY_LIB})

target_include_directories(AGL-Manager PRIVATE plugins/Basketball)
find_path(BASKETBALL_INCLUDE_DIR basketball.h)
find_library(BASKETBALL_LIBRARY libBasketball.dll)
if (BASKETBALL_INCLUDE_DIR AND BASKETBALL_LIBRARY)
    set(BASK_FOUND TRUE)
    include_directories(${BASKETBALL_INCLUDE_DIR})
    set(PATH_TO_BASKETBALL_LIB ${PATH_TO_BASKETBALL_LIB} ${BASKETBALL_LIBRARY})
endif()
target_link_libraries(AGL-Manager PRIVATE ${PATH_TO_BASKETBALL_LIB})

target_link_libraries(AGL-Manager PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(AGL-Manager PRIVATE Qt${QT_VERSION_MAJOR}::Test)
target_link_libraries(AGL-Manager PRIVATE Qt${QT_VERSION_MAJOR}::Gui)


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.AGL-Manager)
endif()
set_target_properties(AGL-Manager PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS AGL-Manager
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(AGL-Manager)
endif()
