#include "videoinfo.h"

VideoInfo::VideoInfo() {

}

VideoInfo::VideoInfo(QString videoId, QString title, QString thumbnail_url, long duration_ms) {
    this->videoId = videoId;
    this->title = title;
    this->thumbnail_url = thumbnail_url;
    this->duration_ms = duration_ms;
}

VideoInfo::~VideoInfo() {

}

QString VideoInfo::getVideoId() {
    return videoId;
}

QString VideoInfo::getTitle() {
    return title;
}

QString VideoInfo::getThumbnailURL() {
    return thumbnail_url;
}

long VideoInfo::getDurationMs() {
    return duration_ms;
}
