#-------------------------------------------------
#
# Project created by QtCreator 2015-01-27T23:21:57
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CacaTube
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    youtubeprocess.cpp \
    videoinfo.cpp \
    audioplayer.cpp

HEADERS  += mainwindow.h \
    youtubeprocess.h \
    videoinfo.h \
    audioplayer.h \
    cacatube_const.h

FORMS    += mainwindow.ui

LIBS += -L/Users/cagdascaglak/Desktop/libsandbox/vlc-qt/build/install_dir/lib
LIBS += -L/Users/cagdascaglak/Desktop/libsandbox/vlc-qt/build/install_dir/plugins
LIBS+= -L//Users/cagdascaglak/Desktop/libquvi/build/inst/lib
LIBS += -lvlc -lvlc-qt -lvlccore -lquvi

INCLUDEPATH += /Users/cagdascaglak/Desktop/libsandbox/vlc-qt/build/install_dir/include
INCLUDEPATH += /Applications/VLC-2.1.5.app/Contents/MacOS/include
INCLUDEPATH += /Users/cagdascaglak/Desktop/libquvi/build/inst/include
