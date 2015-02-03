#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QThread>
#include <iostream>
#include <QStringList>
#include <QStringListModel>
#include <QLinkedList>
#include <QEventLoop>
#include <QMediaPlayer>

#include <videoinfo.h>
#include "audioplayer.h"

#include <QTimer>

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

    void on_play_button_clicked();
    void on_pause_button_clicked();

    void on_title_list_clicked(const QModelIndex &index);
    void on_stop_button_clicked();

    void update_time_line();

    void on_next_button_clicked();
    void on_prev_button_clicked();

signals:
    void stop_button_clicked();
    void pause_button_clicked();

private:
    Ui::MainWindow *ui;
    QList<VideoInfo> *list;
    AudioPlayer *audio_player;
    QStringListModel *model;
    VideoInfo video_info;
    QLinkedList<QString> *token_chain;

    int selected_index;
    int prev_index;
    int list_size;

    int pageNumber;
    int result_sum;
    int start;
    int end;

    QThread *thread;

};

#endif // MAINWINDOW_H
