#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <iostream>
#include <QStringList>
#include <QStringListModel>

#include <videoinfo.h>
#include "audioplayer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_search_bar_returnPressed();
    void on_title_list_doubleClicked(const QModelIndex &index);

    void on_play_button_clicked();

    void on_pause_button_clicked();

private:
    Ui::MainWindow *ui;
    QList<VideoInfo> *list;
    AudioPlayer *audio_player;
};

#endif // MAINWINDOW_H
