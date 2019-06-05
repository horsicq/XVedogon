QT += core
QT -= gui

CONFIG += static_plugins

CONFIG += c++11

TARGET = console_source
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    main_console.cpp \
    consoleoutput.cpp

include(../build.pri)
include(../xvdg_utils.pri)

HEADERS += \
    consoleoutput.h
