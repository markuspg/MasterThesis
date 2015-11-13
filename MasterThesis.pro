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
    src/gathread.cpp \
    src/helper_functions.cpp \
    src/matrix.cpp \
    src/problem.cpp \
    src/qap.cpp \
    src/settings.cpp \
    src/tsreferenceset.cpp \
    src/tsthread.cpp \
    src/solution/qap_solution.cpp \
    src/solution/randomkeysolution.cpp \
    src/solution/solution.cpp \
    src/solution/solutionbase.cpp

HEADERS += \
    src/analyzer.h \
    src/gathread.h \
    src/helper_functions.h \
    src/matrix.h \
    src/problem.h \
    src/qap.h \
    src/settings.h \
    src/tsreferenceset.h \
    src/tsthread.h \
    src/solution/qap_solution.h \
    src/solution/randomkeysolution.h \
    src/solution/solution.h \
    src/solution/solutionbase.h

OTHER_FILES += \
    .gitignore \
    LICENSE \
    README.md

QMAKE_CXXFLAGS += -std=c++11
