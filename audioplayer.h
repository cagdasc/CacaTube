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
#include <QMediaPlayer>
#include <quvi/quvi.h>
#include <quvi/llst.h>
#include <quvi/net.h>
#include <vlc-qt/Common.h>
#include <vlc-qt/Instance.h>
#include <vlc-qt/Media.h>
#include <vlc-qt/MediaPlayer.h>
#include "vlc/libvlc.h"
#include "string.h"
#include "cacatube_const.h"

#define VLC 1

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

    bool isPlaying();
    float getCurrentPosition();

    double getDuration();
    VlcMediaPlayer *getMediaPlayer();

private:
    bool is_playing;
    bool is_paused;
    double duration;

    QString raw_url;
    VlcInstance *_instance;
    VlcMediaPlayer *_player;
    VlcMedia *_media;

    QMediaPlayer *player;

    QString getEmbeddedMediaURL();
    QString getEmbeddedMediaURLWithAPI();

signals:
    void is_pplaying();
    void is_pstop();

public slots:
    void play();
    void stop();

    void started();
    void ended();
};

#endif // AUDIOPLAYER_H
