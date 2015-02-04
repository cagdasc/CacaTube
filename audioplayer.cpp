#include "audioplayer.h"
#include <iostream>

//QString AudioPlayer::cacatube_api_url = "http://api.caglak.cc:8080/cacatubeapi/v1/eURL";

AudioPlayer::AudioPlayer(QObject *parent) : QObject(parent)
{
#ifdef VLC
    _instance = new VlcInstance(VlcCommon::args(), this);
    _player = new VlcMediaPlayer(_instance);
    connect(_player, SIGNAL(playing()), this, SLOT(started()));
    connect(_player, SIGNAL(end()), this, SLOT(ended()));
    _volume = new VlcAudio(_player);
#else
    player = new QMediaPlayer();
    player->play();


#endif

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
    std::cout << "raw link:" << embedded_url.toStdString() << std::endl;

    quvi_parse_close(&m);
    quvi_close(&q);

    return embedded_url;
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

        QJsonDocument json_string = QJsonDocument::fromJson(json.toUtf8());

        QJsonObject json_object = json_string.object();

        std::cout << json_object["title"].toString().toStdString() << std::endl;
        std::cout << json_object["duration"].toInt() << std::endl;
        duration = json_object["duration"].toInt() * 1000;

        embedded = json_object["url"].toString();
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
        //QString embedded_url = getEmbeddedMediaURL();
        QString embedded_url = getEmbeddedMediaURLWithAPI();
        //QString embedded_url = getEmbeddedMediaURLWithLocal();

#ifdef VLC
        _media = new VlcMedia(embedded_url, _instance);
        _player->open(_media);
#else
        player->setMedia(QUrl(embedded_url));
        player->setVolume(50);
        player->play();
        is_playing = true;
        is_paused = false;
        emit is_pplaying();
#endif
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
#ifdef VLC
        _player->pause();
#else
        player->pause();
#endif
        is_paused = true;
    } else {
#ifdef VLC
        _player->resume();
#else
        player->play();
#endif
        is_paused = false;
    }

}

void AudioPlayer::stop() {
    if (is_playing || is_paused) {

#ifdef VLC
        _player->stop();
#else
        player->stop();
#endif
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
#ifdef VLC
    _volume->setVolume(volume);
#endif
}

float AudioPlayer::getCurrentPosition() {
#ifdef VLC
    return _player->position();
#else
    return player->position() / duration;
#endif

}

double AudioPlayer::getDuration() {
    return duration;
}

void AudioPlayer::release() {
#ifdef VLC
    delete _player;
    delete _media;
    delete _instance;
    delete _volume;
#else
    delete player;
#endif
}

AudioPlayer::~AudioPlayer()
{
    release();
}

