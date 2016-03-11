#-------------------------------------------------
#
# Project created by QtCreator 2016-03-11T11:25:30
#
#-------------------------------------------------

QT       += testlib
QMAKE_CC = gcc-4.9
QMAKE_CXX = g++-4.9

QT       -= gui

TARGET = tst_testsprofiling
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += c++14

TEMPLATE = app


SOURCES += tst_testsprofiling.cpp \
            ../../src/Profilage/CalculDonneesProfilage.cpp \
            ../../src/MathematicalFormula/MathematicalFormula.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"


INCLUDEPATH += ../../
