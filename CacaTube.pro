#-------------------------------------------------
#
# Project created by QtCreator 2015-01-27T23:21:57
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CacaTube
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    youtubeprocess.cpp \
    audioplayer.cpp \
    videoinfo.cpp

HEADERS  += mainwindow.h \
    youtubeprocess.h \
    videoinfo.h \
    audioplayer.h \
    cacatube_const.h

FORMS    += mainwindow.ui

QMAKE_MAC_SDK = macosx


LIBS += -L/Users/cagdascaglak/Desktop/libsandbox/libvlc/compiled_backup/vlc-qt/build/vlc_install_dir/lib
LIBS += -L/Users/cagdascaglak/Desktop/libsandbox/libvlc/compiled_backup/vlc-3.0.0/build/VLC.app/Contents/MacOS/plugins
LIBS += -lvlc -lvlc-qt -lvlccore

INCLUDEPATH += /Users/cagdascaglak/Desktop/libsandbox/libvlc/compiled_backup/vlc-qt/build/vlc_install_dir/include
INCLUDEPATH += /Users/cagdascaglak/Desktop/libsandbox/libvlc/compiled_backup/vlc-3.0.0/build/vlc_install_dir/include

RESOURCES += \
    res.qrc
