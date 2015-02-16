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

#include "audioplayer.h"
#include <iostream>

//QString AudioPlayer::cacatube_api_url = "http://api.caglak.cc:8080/cacatubeapi/v1/eURL";

AudioPlayer::AudioPlayer(QObject *parent) : QObject(parent)
{
    _instance = new VlcInstance(VlcCommon::args(), this);
    _player = new VlcMediaPlayer(_instance);
    connect(_player, SIGNAL(playing()), this, SLOT(started()));
    connect(_player, SIGNAL(error()), this, SLOT(errorSomething()));
    connect(_player, SIGNAL(end()), this, SLOT(ended()));

    _volume = new VlcAudio(_player);

}

AudioPlayer::AudioPlayer(QString raw_url) {
    this->raw_url = raw_url;
    is_playing = false;
    is_paused = true;
}

void AudioPlayer::setRawURL(QString raw_url) {
    this->raw_url = raw_url;
}

QString AudioPlayer::getEmbeddedMediaURLWithAPI() {
    QEventLoop loop;
    QNetworkAccessManager *access = new QNetworkAccessManager();

    QUrlQuery query;
    query.addQueryItem("url", raw_url);

    QUrl url(cacatube_api_url);
    url.setQuery(query.query());
    QNetworkRequest request(url);

    QNetworkReply *reply = access->get(request);
    QObject::connect(access, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
    loop.exec();

    QString embedded;

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray bytes = reply->readAll();
        QString json(bytes);

        VideoInfo video_info = Utils::parseAPIReqJson(json);
        duration = video_info.getDuration() * 1000;

        embedded = video_info.getEmbeddedUrl();
    }

    delete reply;
    delete access;

    return embedded;
}

QString AudioPlayer::getEmbeddedMediaURLWithLocal() {
    QEventLoop loop;
    link_process = new QProcess();
    connect(link_process, SIGNAL(finished(int, QProcess::ExitStatus)), &loop, SLOT(quit()));
    link_process->start("/usr/local/bin/youtube-dl -g " + raw_url);
    loop.exec();

    return link_process->readAllStandardOutput();
}

void AudioPlayer::play() {

    if (!is_playing) {
        QString embedded_url = getEmbeddedMediaURLWithAPI();
        //QString embedded_url = getEmbeddedMediaURLWithLocal();

        _media = new VlcMedia(embedded_url, _instance);
        _player->open(_media);
    }

}

void AudioPlayer::started() {
    if (!is_playing) {
        is_playing = true;
        is_paused = false;
        emit is_pplaying();
    }

}

void AudioPlayer::ended() {
    is_playing = false;
    is_paused = true;
    std::cout << "Stopped baby" << std::endl;
    emit is_pstop();
}

void AudioPlayer::pause() {

    if (!is_paused) {
        _player->pause();
        is_paused = true;
    } else {
        _player->resume();
        is_paused = false;
    }

}

void AudioPlayer::stop() {
    if (is_playing || is_paused) {
        _player->stop();
        is_playing = false;
        emit is_pstop();
    }
}

void AudioPlayer::setIsPlaying(bool is_playing) {
    this->is_playing = is_playing;
}

void AudioPlayer::setIsPaused(bool is_paused) {
    this->is_paused = is_paused;
}

bool AudioPlayer::isPlaying() {
    return is_playing;
}

void AudioPlayer::setVolume(int volume) {
    _volume->setVolume(volume);
}

float AudioPlayer::getCurrentPosition() {
    return _player->position();
}

double AudioPlayer::getDuration() {
    return duration;
}

void AudioPlayer::errorSomething() {
    stop();
    emit is_perror();
}

void AudioPlayer::release() {
    delete _player;
    delete _media;
    delete _instance;
    delete _volume;
    //delete link_process;
}

AudioPlayer::~AudioPlayer()
{
    release();
}

