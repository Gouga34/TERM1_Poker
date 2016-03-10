#-------------------------------------------------
#
# Project created by QtCreator 2016-03-09T09:48:43
#
#-------------------------------------------------

QT       += widgets testlib
QMAKE_CC = gcc-4.9
QMAKE_CXX = g++-4.9

TARGET = tst_testsassessor
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += c++14

TEMPLATE = app


SOURCES += tst_testsassessor.cpp \
    ../../src/Assessor/Assessor.cpp \
    ../../src/Assessor/PlayerCards.cpp \
    ../../src/Jeu/Carte.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH += ../../
