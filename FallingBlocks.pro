QT       += core gui dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = fallingblocks 
TEMPLATE = app

target.path = /usr/bin

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += src/main.cpp \
       src/gui.cpp \
       src/logger.cpp \
       src/matrix.cpp \
       src/position.cpp

HEADERS  += include/definitions.hpp \
        include/gui.hpp \
        include/logger.hpp \
        include/matrix.hpp \
        include/position.hpp

DBUS_INTERFACES += \
    interface/se.mydns.mysland.DBusDaemon.xml

DISTFILES += \
    interface/se.mydns.mysland.DBusDaemon.xml

LIBS += -lglog
