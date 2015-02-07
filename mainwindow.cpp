/*
 * Copyright 2015 Cagdas Caglak
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "youtubeprocess.h"

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
    connect(audio_player, SIGNAL(is_perror()), this, SLOT(error()));
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
    playlist_size = 0;
    playlist_change = false;

    sure_box = new QMessageBox("Warning!!","You have a playlist. \n Do you want to remove current playlist?",
                               QMessageBox::Warning, QMessageBox::Yes, QMessageBox::No | QMessageBox::Default, QMessageBox::Cancel | QMessageBox::Escape,
                               this);
    connect(sure_box, SIGNAL(finished(int)), this, SLOT(openFileDialog(int)));

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
    delete sure_box;
}

void MainWindow::on_search_bar_returnPressed()
{
    ui->app_status->setText("Searching...");
    pageNumber = 0;

    QEventLoop loop;
    QNetworkAccessManager *access = new QNetworkAccessManager();

    YoutubeProcess *process = new YoutubeProcess(this, access);
    process->setQ(ui->search_bar->text());
    process->max_result = 50;

    QNetworkReply *reply = process->executeSearch();
    QObject::connect(access, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
    loop.exec();

    QByteArray data_bytes;
    QString json;
    if (reply->error() == QNetworkReply::NoError) {
        data_bytes = reply->readAll();
        json = QString::fromUtf8(data_bytes);

        delete process;
        delete access;

        list = new QList<VideoInfo>();

        //YoutubeProcess::parse_search_json(list, string);
        Utils::parseSearchJson(list, json);

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

        ui->app_status->setText("Success");
    } else {
        ui->app_status->setText("Network Error!!");
    }

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

    //YoutubeProcess::parse_search_json(list, string);
    Utils::parseSearchJson(list, string);

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
}

void MainWindow::on_play_button_clicked()
{
    audio_player->setRawURL(video_info.getVideoId());
    //audio_player->setRawURL("https://www.youtube.com/watch?v=URLRdcnU6Hk");

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
    ui->time_line_text->setText("...:...:...");
    ui->play_button->setEnabled(true);
    emit stop_button_clicked();
}

void MainWindow::on_pause_button_clicked()
{
    audio_player->pause();
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

    int total_time;
    while (audio_player->isPlaying()) {
        ui->loading_status->setText("Playing...");
        total_time = audio_player->getCurrentPosition() * duration;
        ui->time_line->setValue(total_time);
        total_time /= 1000;
        int sec = total_time % 60;
        QString sec_t = QString("%1").arg(sec, 2, 10, QChar('0'));
        total_time /= 60;
        int min = total_time % 60;
        QString min_t = QString("%1").arg(min, 2, 10, QChar('0'));
        total_time /= 60;
        if (total_time != 0) {
            int hr = total_time % 60;
            QString hr_t = QString("%1").arg(hr, 2, 10, QChar('0'));
            ui->time_line_text->setText(hr_t + ":" + min_t + ":" + sec_t);
        } else {
            ui->time_line_text->setText(min_t + ":" + sec_t);
        }
        QApplication::processEvents();
    }


    ui->play_button->setEnabled(true);
    emit stop_button_clicked();

    ui->time_line_text->setText("...:...:...");
    ui->loading_status->setText("Stopped");
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

void MainWindow::on_volume_slider_sliderPressed()
{
    if (audio_player->isPlaying()) {
        int position = ui->volume_slider->value();
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

        playlist_change = true;

    }
}

void MainWindow::on_remove_playlist_button_clicked()
{
    if (playlist->size() > 0) {
        playlist->removeAt(selected_index);
        stringlist_playlist->removeAt(selected_index);
        model_playlist->setStringList(*stringlist_playlist);
        ui->playlist->setModel(model_playlist);

        playlist_change = true;
    }
}

void MainWindow::on_playlist_clicked(const QModelIndex &index)
{
    selected_index = index.row();
    video_info = playlist->at(selected_index);
    cout << "Title: " << video_info.getTitle().toStdString() << endl;
    cout << video_info.getVideoId().toStdString() << endl;

}

void MainWindow::on_tool_open_triggered()
{
    if (playlist->size() > 0) {

        sure_box->open();

    } else {
        openFileDialog(QMessageBox::Yes);
    }



}

void MainWindow::on_tool_save_triggered()
{
    if (playlist->size() > 0 && playlist_change) {
        if (file_name != 0) {
            QString json = Utils::createJsonString(*playlist);
            if (Utils::savePlaylist(json, file_name)) {
                cout << "Save Saved" << endl;
            } else {
                cout << "Save Not Saved" << endl;
            }
        }
    }
}

void MainWindow::on_tool_saveas_triggered()
{
    if (playlist->size() > 0) {
        file_name = QFileDialog::getSaveFileName(this, tr("Save File"), QDir::homePath(), tr("CacaTube (*.ct)"));

        if (file_name != 0 && playlist->size() != 0) {
            QString json = Utils::createJsonString(*playlist);
            if (Utils::savePlaylist(json, file_name)) {
                cout << "Save As Saved" << endl;
            } else {
                cout << "Save As Not Saved" << endl;
            }
        }
    }
}

void MainWindow::openFileDialog(int result) {

    bool open_playlist = true;

    if (result == QMessageBox::No) {
        open_playlist = false;
    } else if (result == QMessageBox::Cancel) {
        open_playlist = false;
    }

    if (open_playlist) {

        file_name = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(), tr("CacaTube (*.ct)"));
        QString *json = new QString();

        if (Utils::loadPlaylist(file_name, json)) {
            playlist->clear();
            Utils::parsePlaylistJson(playlist, *json);

            for (int i = 0; i < playlist->size(); ++i) {
                VideoInfo v = playlist->at(i);
                stringlist_playlist->append(v.getTitle());
            }
            model_playlist->setStringList(*stringlist_playlist);
            ui->playlist->setModel(model_playlist);
            playlist_size = playlist->size();
        }

        delete json;
    }

}

void MainWindow::error() {
    ui->app_status->setText("Error while playing!!");
}
