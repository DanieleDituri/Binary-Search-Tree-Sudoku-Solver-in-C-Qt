#-------------------------------------------------
#
# Project created by QtCreator 2024-07-12T13:23:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sudoku
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    myvalidator.cpp

HEADERS  += mainwindow.h \
    myvalidator.h

FORMS    += mainwindow.ui
