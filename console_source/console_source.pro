QT += core
QT -= gui

CONFIG(debug, debug|release) {
    CONFIG += static_plugins
}

CONFIG += c++11

TARGET = xvdgc
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    main_console.cpp \
    consoleoutput.cpp

include(../build.pri)

HEADERS += \
    consoleoutput.h
