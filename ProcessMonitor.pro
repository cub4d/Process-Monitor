QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ProcessMonitor
TEMPLATE = app

INCLUDEPATH += $$PWD/include

SOURCES += \
    src/main.cpp \
    src/processmonitor.cpp \
    src/sortfilterproxymodel.cpp \
    src/processinfo.cpp \
    src/processtablemodel.cpp

HEADERS += \
    $$INCLUDEPATH/*.h
