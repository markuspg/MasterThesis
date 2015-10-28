#---------------------------------------------------------
#
# Project created by Markus Prasser on 2015-10-28T12:40:00
#
#---------------------------------------------------------

QT       -= core gui

TARGET = MasterThesis
CONFIG   -= app_bundle console

TEMPLATE = app

SOURCES += src/main.cpp \
    src/analyzer.cpp \
    src/helper_functions.cpp \
    src/problem.cpp \
    src/settings.cpp \
    src/qap.cpp

HEADERS += \
    src/analyzer.h \
    src/helper_functions.h \
    src/problem.h \
    src/settings.h \
    src/qap.h

OTHER_FILES += \
    .gitignore \
    LICENSE \
    README.md

QMAKE_CXXFLAGS += -std=c++11
