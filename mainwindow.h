#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QThread>
#include <iostream>
#include <QStringList>
#include <QStringListModel>
#include <QEventLoop>
#include <QMediaPlayer>
#include <QFileDialog>

#include "videoinfo.h"
#include "audioplayer.h"
#include "utils.h"

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

    void on_volume_slider_sliderMoved(int position);
    void on_volume_slider_sliderPressed();

    void on_add_playlist_button_clicked();
    void on_remove_playlist_button_clicked();
    void on_playlist_clicked(const QModelIndex &index);

    void on_tool_open_triggered();
    void on_tool_save_triggered();
    void on_tool_saveas_triggered();

signals:
    void stop_button_clicked();
    void pause_button_clicked();

private:
    Ui::MainWindow *ui;
    QList<VideoInfo> *list;
    QList<VideoInfo> *playlist;
    AudioPlayer *audio_player;
    QStringListModel *model;
    QStringListModel *model_playlist;
    QStringList *stringlist_playlist;
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
