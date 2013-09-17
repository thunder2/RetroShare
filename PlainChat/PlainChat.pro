#-------------------------------------------------
#
# Project created by QtCreator 2013-07-03T15:24:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PlainChat
TEMPLATE = app


SOURCES += main.cpp\
        PlainChat.cpp \
    RsMessageInputWidget.cpp

HEADERS  += PlainChat.h \
    RsMessageInputWidget.h

FORMS    += PlainChat.ui \
    RsMessageInputWidget.ui

INCLUDEPATH += ../supportlibs/pegmarkdown
LIBS += ../supportlibs/pegmarkdown/lib/libpegmarkdown.a
