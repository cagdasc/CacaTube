#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <youtubeprocess.h>
#include <QEventLoop>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    prev_index = -1;

    audio_player = new AudioPlayer();
    model = new QStringListModel();

    thread = new QThread();
    thread->setObjectName("Play thread");
    audio_player->moveToThread(thread);
    connect(thread, SIGNAL(started()), audio_player, SLOT(play()));
    connect(audio_player, SIGNAL(is_pplaying()), this, SLOT(update_time_line()));
    connect(this, SIGNAL(stop_button_clicked()), audio_player, SLOT(stop()));
    connect(this, SIGNAL(pause_button_clicked()), this, SLOT(update_time_line()));

    connect(audio_player, SIGNAL(is_pstop()), thread, SLOT(quit()));
    connect(this, SIGNAL(destroyed()), thread, SLOT(deleteLater()));

}

MainWindow::~MainWindow()
{
    delete ui;
    delete thread;
}

void MainWindow::on_search_bar_returnPressed()
{

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

}

void MainWindow::on_play_button_clicked()
{
    audio_player->setRawURL(video_info.getVideoId());

    //audio_player->setRawURL("https://www.youtube.com/watch?v=zmr2I8caF0c");

    if (prev_index != -1 && prev_index != selected_index) {
        audio_player->setIsPlaying(false);
        audio_player->setIsPaused(true);
        audio_player->release();
    }

    prev_index = selected_index;

    if (thread->isFinished()) {
        cout << "Finished" << endl;
    }

    if (!audio_player->isPlaying()) {
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

    if (audio_player->isPlaying()) {
        emit pause_button_clicked();
    }

}

void MainWindow::on_title_list_clicked(const QModelIndex &index)
{
    selected_index = index.row();
    video_info = list->at(selected_index);
    cout << video_info.getVideoId().toStdString() << endl;

}
void MainWindow::update_time_line() {



    cout << "Slider max: " << audio_player->getDuration() << endl;
    double duration = audio_player->getDuration();
    long msec = 0;
    ui->time_line->setMaximum(duration);

    while (audio_player->isPlaying() && msec <= duration) {
        //ui->time_line->setValue((int)(audio_player->getCurrentPosition() * duration));
        ui->time_line->setValue(msec);
        //ui->centralWidget->update();
        //cout <<audio_player->getCurrentPosition() <<endl;

        msec += 10;
        QThread::msleep(10);
        QApplication::processEvents();

    }

    ui->play_button->setEnabled(true);
    ui->time_line->setValue(0);
    cout << "Finished" << endl;

}


