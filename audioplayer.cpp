#include "audioplayer.h"
#include <iostream>

AudioPlayer::AudioPlayer(QObject *parent) : QObject(parent)
{

}

AudioPlayer::AudioPlayer(QString raw_url) {
    this->raw_url = raw_url;
    is_playing = false;
    is_paused = true;
}

void AudioPlayer::setRawURL(QString raw_url) {
    this->raw_url = raw_url;
}

QString AudioPlayer::getEmbeddedMediaURL() {

    quvi_media_t m;
    QUVIcode rc;
    quvi_t q;

    QByteArray array = raw_url.toUtf8();

    std::cout << array.size()<< std::endl;
    char *URL = array.data();
    char *newUrl;
    double dur;
    rc = quvi_init(&q);
    quvi_parse(q, URL, &m);
    quvi_getprop(m, QUVIPROP_MEDIAURL, &newUrl);
    quvi_getprop(m, QUVIPROP_MEDIADURATION, &dur);

    QString embedded_url(newUrl);

    duration = dur;
    std::cout << "raw maximum:" << dur << std::endl;

    quvi_parse_close(&m);
    quvi_close(&q);

    return embedded_url;
}

void AudioPlayer::play() {

    if (!is_playing) {

        QString embedded_url = getEmbeddedMediaURL();

        _instance = new VlcInstance(VlcCommon::args(), this);
        _player = new VlcMediaPlayer(_instance);
        _media = new VlcMedia(embedded_url, _instance);
        _player->open(_media);

        is_playing = true;
        is_paused = false;

        emit is_pplaying();
    }

}

void AudioPlayer::pause() {
    if (is_playing) {
        _player->pause();
        is_playing = false;
        is_paused = true;
    } else if (!is_playing && is_paused) {
        _player->resume();
        is_playing = true;
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

float AudioPlayer::getCurrentPosition() {
    return _player->position();
}

double AudioPlayer::getDuration() {
    return duration;
}

void AudioPlayer::release() {
    delete _player;
    delete _media;
    delete _instance;
}

AudioPlayer::~AudioPlayer()
{
    release();
}

