#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QObject>
#include <QString>
#include <QByteArray>
#include <quvi/quvi.h>
#include <quvi/llst.h>
#include <quvi/net.h>
#include <vlc-qt/Common.h>
#include <vlc-qt/Instance.h>
#include <vlc-qt/Media.h>
#include <vlc-qt/MediaPlayer.h>
#include "vlc/libvlc.h"
#include "string.h"

class AudioPlayer : public QObject
{
    Q_OBJECT
public:
    explicit AudioPlayer(QObject *parent = 0);
    AudioPlayer(QString raw_url);
    ~AudioPlayer();

    void play();
    void pause();
    void release();

    void setRawURL(QString raw_url);
    void setIsPlaying(bool is_playing);
    void setIsPaused(bool is_paused);

private:
    bool is_playing;
    bool is_paused;

    QString raw_url;
    VlcInstance *_instance;
    VlcMediaPlayer *_player;
    VlcMedia *_media;

    QString getEmbeddedMediaURL();

signals:

public slots:
};

#endif // AUDIOPLAYER_H
