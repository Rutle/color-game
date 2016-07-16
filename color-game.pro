#-------------------------------------------------
#
# Project created by QtCreator 2016-07-12T12:22:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = color-game
TEMPLATE = app
CONFIG += c++11


SOURCES += main.cpp\
        dialog.cpp \
    buttonswidget.cpp

HEADERS  += dialog.hh \
    buttonswidget.hh

FORMS    += dialog.ui
