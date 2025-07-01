cmake_minimum_required(VERSION 3.16)

project(Talk LANGUAGES C)

set(TALK_SOURCES
    fsapi.c
    fsapi.h
    ScreenReaderDriver.h
    ScreenReaderDriverJAWS.cpp
    ScreenReaderDriverJAWS.h
    ScreenReaderDriverNVDA.cpp
    ScreenReaderDriverNVDA.h
    ScreenReaderDriverSA.cpp
    ScreenReaderDriverSA.h
    ScreenReaderDriverSAPI.cpp
    ScreenReaderDriverSAPI.h
    ScreenReaderDriverSNova.cpp
    ScreenReaderDriverSNova.h
    ScreenReaderDriverWE.cpp
    ScreenReaderDriverWE.h
    ScreenReaderDriverZT.cpp
    ScreenReaderDriverZT.h
    Tolk.cpp
    Tolk.h
    TolkVersion.h
    wineyes.c
    wineyes.h
    zt.c
    zt.h
)

add_library(${PROJECT_NAME} SHARED ${TALK_SOURCES})
target_compile_definitions(${PROJECT_NAME} PRIVATE _EXPORTING UNICODE)
