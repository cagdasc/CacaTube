#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <youtubeprocess.h>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
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
        strlist << v.title;
    }

    QStringListModel *model = new QStringListModel(strlist, this);

    ui->title_list->setModel(model);

}

void MainWindow::on_title_list_doubleClicked(const QModelIndex &index)
{
    cout << index.row() << endl;

    if (audio_player == 0) {
        audio_player = new AudioPlayer(list->at(index.row()).videoId);
    } else {
        audio_player->setRawURL(list->at(index.row()).videoId);
        audio_player->setIsPlaying(false);
        audio_player->setIsPaused(true);
        audio_player->release();

    }

    //cout << list->at(index.row()).videoId.toStdString() << endl;
}

void MainWindow::on_play_button_clicked()
{
    audio_player->play();
}

void MainWindow::on_pause_button_clicked()
{
    audio_player->pause();
}
