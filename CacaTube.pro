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

#LIBS += -L/Users/cagdascaglak/Desktop/libsandbox/vlc-qt/build/install_dir/lib
#LIBS += -L/Users/cagdascaglak/Desktop/libsandbox/vlc-qt/build/install_dir/plugins
#LIBS+= -L/Users/cagdascaglak/Desktop/libsandbox/libquvi/build/inst/lib
#LIBS += -lvlc -lvlc-qt -lvlccore -lquvi

#INCLUDEPATH += /Users/cagdascaglak/Desktop/libsandbox/vlc-qt/build/install_dir/include
#INCLUDEPATH += /Users/cagdascaglak/Desktop/libsandbox/libquvi/build/inst/include
#INCLUDEPATH += /Applications/VLC-2.1.5.app/Contents/MacOS/include


LIBS += -L/Users/cagdascaglak/Desktop/libsandbox/libvlc/compiled_backup/vlc-qt/build/vlc_install_dir/lib
LIBS += -L/Users/cagdascaglak/Desktop/libsandbox/libvlc/compiled_backup/vlc-3.0.0/build/VLC.app/Contents/MacOS/plugins
LIBS+= -L/Users/cagdascaglak/Desktop/libsandbox/libquvi/build/inst/lib
LIBS += -lvlc -lvlc-qt -lvlccore -lquvi

INCLUDEPATH += /Users/cagdascaglak/Desktop/libsandbox/libvlc/compiled_backup/vlc-qt/build/vlc_install_dir/include
INCLUDEPATH += /Users/cagdascaglak/Desktop/libsandbox/libquvi/build/inst/include
INCLUDEPATH += /Users/cagdascaglak/Desktop/libsandbox/libvlc/compiled_backup/vlc-3.0.0/build/vlc_install_dir/include
