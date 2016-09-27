QT += core
QT -= gui

CONFIG += c++11

TARGET = MyCompiler
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    compiler.cpp \
    filereader.cpp \
    filewriter.cpp \
    token.cpp \
    line.cpp \
    tokenlist.cpp \
    linelist.cpp

HEADERS += \
    compiler.h \
    filereader.h \
    filewriter.h \
    token.h \
    line.h \
    tokenlist.h \
    linelist.h
