QMAKE_CXXFLAGS += -Werror
CONFIG += debug
HEADERS       = window.h R2Conv.h R2Graph.h PointDeq.h
SOURCES       = main.cpp \
                window.cpp \
                R2Conv.cpp \
                PointDeq.cpp
QT += widgets
