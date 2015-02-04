#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <youtubeprocess.h>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    prev_index = -1;

    audio_player = new AudioPlayer();


    thread = new QThread();
    thread->setObjectName("Play thread");
    audio_player->moveToThread(thread);

    connect(thread, SIGNAL(started()), audio_player, SLOT(play()));
    connect(audio_player, SIGNAL(is_pplaying()), this, SLOT(update_time_line()));
    connect(this, SIGNAL(stop_button_clicked()), audio_player, SLOT(stop()));
    connect(this, SIGNAL(pause_button_clicked()), this, SLOT(update_time_line()));

    connect(audio_player, SIGNAL(is_pstop()), thread, SLOT(quit()));
    connect(this, SIGNAL(destroyed()), thread, SLOT(deleteLater()));

    ui->next_button->setEnabled(false);
    ui->prev_button->setEnabled(false);

    pageNumber = 0;
    result_sum = start = end = 0;

    ui->volume_per->setText("%" + QString::number(50));
    ui->volume_slider->setValue(50);
    audio_player->setVolume(50);

    model = new QStringListModel();
    model_playlist = new QStringListModel();
    stringlist_playlist = new QStringList();
    playlist = new QList<VideoInfo>();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete thread;
    if (list == 0) {
        delete list;
        delete token_chain;
        delete model;
        cout << "Deleted" << endl;
    }

    delete model_playlist;
    delete stringlist_playlist;
}

void MainWindow::on_search_bar_returnPressed()
{

    pageNumber = 0;

    QEventLoop loop;
    QNetworkAccessManager *access = new QNetworkAccessManager();

    YoutubeProcess *process = new YoutubeProcess(this, access);
    process->setQ(ui->search_bar->text());
    process->max_result = 50;

    QNetworkReply *reply = process->executeSearch();
    QObject::connect(access, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
    loop.exec();

    QByteArray bytes = reply->readAll();
    QString string(bytes);

    delete process;
    delete access;

    list = new QList<VideoInfo>();

    YoutubeProcess::parse_search_json(list, string);

    QStringList strlist;
    for (int i = 0; i < list->size(); ++i) {
        VideoInfo v = list->at(i);
        strlist << v.getTitle();
    }

    model->setStringList(strlist);
    ui->title_list->setModel(model);

    ui->next_button->setEnabled(true);

    end = list->size();
    token_chain = new QLinkedList<QString>();
    token_chain->push_back(0);
    token_chain->push_back(YoutubeProcess::next_page_token);

    //model_playlist->removeRows(0, stringlist_playlist->size());
}

void MainWindow::on_next_button_clicked()
{
    pageNumber++;

    cout << "Page: " << pageNumber << endl;

    ui->prev_button->setEnabled(true);

    QEventLoop loop;
    QNetworkAccessManager *access = new QNetworkAccessManager();

    YoutubeProcess *process = new YoutubeProcess(this, access);
    process->max_result = 50;

    QNetworkReply *reply = process->executeSearch(token_chain->last());
    QObject::connect(access, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
    loop.exec();

    QByteArray bytes = reply->readAll();
    QString string(bytes);

    delete process;
    delete access;

    YoutubeProcess::parse_search_json(list, string);

    start = end;

    if ((pageNumber + 1) * YoutubeProcess::max_result < list->size()) {
        end = (pageNumber + 1) * YoutubeProcess::max_result;
    } else {
        end = list->size();
    }

    QStringList strlist;
    for (int i = start; i < end; ++i) {
        VideoInfo v = list->at(i);
        strlist << v.getTitle();
    }

    model->setStringList(strlist);
    ui->title_list->setModel(model);

    cout << "Size: " << list->size() << endl;
    token_chain->push_back(YoutubeProcess::next_page_token);

    //model_playlist->removeRows(0, stringlist_playlist->size());

}

void MainWindow::on_prev_button_clicked()
{
    pageNumber--;

    cout << "Page: " << pageNumber << endl;

    if (pageNumber < 1) {
        ui->prev_button->setEnabled(false);
    }

    token_chain->pop_back();

    for (int i = start; i < end; ++i) {
        list->removeLast();
    }

    end = start;

    if ((start - YoutubeProcess::max_result) > 0) {
        start -= YoutubeProcess::max_result;
    } else {
        start = 0;
    }


    QStringList strlist;
    for (int i = start; i < end; ++i) {
        VideoInfo v = list->at(i);
        strlist << v.getTitle();
    }

    model->setStringList(strlist);
    ui->title_list->setModel(model);
    cout << "Size: " << list->size() << endl;

    //model_playlist->removeRows(0, stringlist_playlist->size());
}

void MainWindow::on_play_button_clicked()
{
    audio_player->setRawURL(video_info.getVideoId());
    //audio_player->setRawURL("https://www.youtube.com/watch?v=URLRdcnU6Hk");

    if (prev_index != -1 && prev_index != selected_index) {
        audio_player->setIsPlaying(false);
        audio_player->setIsPaused(true);
    }

    prev_index = selected_index;

    if (thread->isFinished()) {
        cout << "Finished in play scope." << endl;
    }

    if (!audio_player->isPlaying()) {
        ui->loading_status->setText("Loading...");
        thread->start();
        ui->play_button->setEnabled(false);
        cout << "thread start" << endl;
    }

}

void MainWindow::on_stop_button_clicked()
{
    ui->play_button->setEnabled(true);
    emit stop_button_clicked();
}

void MainWindow::on_pause_button_clicked()
{
    audio_player->pause();

    //if (audio_player->isPlaying()) {
    //emit pause_button_clicked();
    //}

}

void MainWindow::on_title_list_clicked(const QModelIndex &index)
{
    selected_index = index.row() + (pageNumber * YoutubeProcess::max_result);
    video_info = list->at(selected_index);
    cout << video_info.getVideoId().toStdString() << endl;

}
void MainWindow::update_time_line() {

    double duration = audio_player->getDuration();
    cout << "Duration: " << duration << "ms" << " -- " << duration/1000 << " sec" << endl;
    ui->time_line->setMaximum(duration);
    cout << "Slider max: " << ui->time_line->maximum() << endl;

    while (audio_player->isPlaying()) {
        ui->loading_status->setText("Playing...");
        ui->time_line->setValue(audio_player->getCurrentPosition() * duration);
        QApplication::processEvents();
    }

    //QApplication::sendPostedEvents();

    ui->play_button->setEnabled(true);
    ui->time_line->setValue(0);
    cout << "Finished" << endl;

}


void MainWindow::on_volume_slider_sliderMoved(int position)
{
    if (audio_player->isPlaying()) {
        audio_player->setVolume(position);
        ui->volume_per->setText("%" + QString::number(position));
    }

}

void MainWindow::on_add_playlist_button_clicked()
{
    if (selected_index != -1) {
        playlist->append(list->at(selected_index));
        VideoInfo v = list->at(selected_index);
        stringlist_playlist->append(v.getTitle());
        model_playlist->setStringList(*stringlist_playlist);
        ui->playlist->setModel(model_playlist);

    }
}

void MainWindow::on_remove_playlist_button_clicked()
{
    if (playlist->size() > 0) {
        playlist->removeAt(selected_index);
        stringlist_playlist->removeAt(selected_index);
        model_playlist->setStringList(*stringlist_playlist);
        ui->playlist->setModel(model_playlist);
    }
}

void MainWindow::on_playlist_clicked(const QModelIndex &index)
{
    selected_index = index.row();
    video_info = playlist->at(selected_index);
    cout << "Title: " << video_info.getTitle().toStdString() << endl;
    cout << video_info.getVideoId().toStdString() << endl;

}
