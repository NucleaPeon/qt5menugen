#-------------------------------------------------
#
# Project created by QtCreator 2023-11-24T11:08:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ShortcutTester
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

macx { 

    LIBS += -framework Cocoa

    QT += macextras

    OBJECTIVE_SOURCES += cocoainitializer.mm
    OBJECTIVE_HEADERS += cocoainitializer.h

}


FORMS    += mainwindow.ui

unix|win32: LIBS += -L/usr/local/lib -lqt5menugen

INCLUDEPATH += /usr/local/include
DEPENDPATH += /usr/local/lib

OTHER_FILES += \
    menu.json

RESOURCES += \
    main.qrc
