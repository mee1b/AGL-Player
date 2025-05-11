cmake_minimum_required(VERSION 3.16)

project(AGL-Manager VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)

set(PROJECT_SOURCES
        main.cpp
)


add_executable(AGL-Manager
    ${PROJECT_SOURCES}
    topwindow.h topwindow.cpp topwindow.ui
    manager.h manager.cpp manager.ui
)

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

target_include_directories(AGL-Manager PRIVATE plugins/plugBasketball)
find_path(BASK_INCLUDE_DIR basketball.h)
find_library(BASK_LIBRARY libplugBasketball.dll)
if (BASK_INCLUDE_DIR AND BASK_LIBRARY)
    set(BASK_FOUND TRUE)
    include_directories(${BASK_INCLUDE_DIR})
    set(PATH_TO_BASK_LIB ${PATH_TO_BASK_LIB} ${BASK_LIBRARY})
endif()
target_link_libraries(AGL-Manager PRIVATE ${PATH_TO_BASK_LIB})

target_link_libraries(AGL-Manager PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(AGL-Manager PRIVATE Qt${QT_VERSION_MAJOR}::Test)



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
