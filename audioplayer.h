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

#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QObject>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QString>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QThread>
#include <QProcess>

#include <vlc-qt/Common.h>
#include <vlc-qt/Instance.h>
#include <vlc-qt/Media.h>
#include <vlc-qt/MediaPlayer.h>
#include <vlc-qt/Audio.h>
#include "vlc/libvlc.h"
#include "string.h"
#include "cacatube_const.h"
#include "utils.h"

class AudioPlayer : public QObject
{
    Q_OBJECT
public:
    explicit AudioPlayer(QObject *parent = 0);
    AudioPlayer(QString raw_url);
    ~AudioPlayer();

    void pause();
    void release();

    void setRawURL(QString raw_url);
    void setIsPlaying(bool is_playing);
    void setIsPaused(bool is_paused);
    void setVolume(int volume);

    bool isPlaying();
    float getCurrentPosition();

    double getDuration();
    VlcMediaPlayer *getMediaPlayer();

private:
    bool is_playing;
    bool is_paused;
    int duration;

    QString raw_url;
    VlcInstance *_instance;
    VlcMediaPlayer *_player;
    VlcMedia *_media;
    VlcAudio *_volume;
    QProcess *link_process;

    QString getEmbeddedMediaURLWithAPI();
    QString getEmbeddedMediaURLWithLocal();

signals:
    void is_pplaying();
    void is_pstop();
    void is_perror();

public slots:
    void play();
    void stop();

    void started();
    void ended();

    void errorSomething();


};

#endif // AUDIOPLAYER_H
