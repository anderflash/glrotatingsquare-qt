QT += core
QT -= gui
QT += widgets

CONFIG += c++11

TARGET = rotatingSquare
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    openglwindow.cpp \
    rotatingsquarewindow.cpp

HEADERS += \
    openglwindow.h \
    rotatingsquarewindow.h
