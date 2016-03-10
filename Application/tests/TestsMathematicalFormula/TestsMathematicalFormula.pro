#-------------------------------------------------
#
# Project created by QtCreator 2016-03-10T14:56:19
#
#-------------------------------------------------

QT       += testlib
QMAKE_CC = gcc-4.9
QMAKE_CXX = g++-4.9

QT       -= gui

TARGET = tst_testsmathematicalformula
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += c++14

TEMPLATE = app


SOURCES += tst_testsmathematicalformula.cpp \
            ../../src/MathematicalFormula/MathematicalFormula.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH += ../../

HEADERS += \
    ../../include/MathematicalFormula/MathematicalFormula.h
