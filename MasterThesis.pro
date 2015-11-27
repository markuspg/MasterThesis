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
    src/helper_functions.cpp \
    src/matrix.cpp \
    src/measure.cpp \
    src/settings.cpp \
    src/analyzer/analyzer.cpp \
    src/analyzer/gathread.cpp \
    src/analyzer/tsreferenceset.cpp \
    src/analyzer/tsthread.cpp \
    src/problem/problem.cpp \
    src/problem/qap.cpp \
    src/problem/salbp.cpp \
    src/problem/task.cpp \
    src/problem/task_storage.cpp \
    src/solution/perm_solution.cpp \
    src/solution/randomkeysolution.cpp \
    src/solution/solution.cpp \
    src/solution/solutionbase.cpp

HEADERS += \
    src/helper_functions.h \
    src/matrix.h \
    src/measure.h \
    src/settings.h \
    src/analyzer/analyzer.h \
    src/analyzer/gathread.h \
    src/analyzer/tsreferenceset.h \
    src/analyzer/tsthread.h \
    src/problem/problem.h \
    src/problem/qap.h \
    src/problem/salbp.h \
    src/problem/task.h \
    src/problem/task_storage.h \
    src/solution/perm_solution.h \
    src/solution/randomkeysolution.h \
    src/solution/solution.h \
    src/solution/solutionbase.h

OTHER_FILES += \
    CMakeLists.txt \
    .gitignore \
    LICENSE \
    MasterThesis.kdev4 \
    README.md

QMAKE_CXXFLAGS += -std=c++14
