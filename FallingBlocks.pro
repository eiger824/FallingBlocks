QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = fallingblocks 
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += src/main.cpp \
       src/gui.cpp \
       src/logger.cpp \
       src/matrix.cpp \
       src/passwordform.cpp \
       src/position.cpp

HEADERS  += include/definitions.hpp \
        include/gui.hpp \
        include/logger.hpp \
        include/matrix.hpp \
        include/passwordform.hpp \
        include/position.hpp
