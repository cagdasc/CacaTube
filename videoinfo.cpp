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

#include "videoinfo.h"

VideoInfo::VideoInfo() {

}

VideoInfo::VideoInfo(QString videoId, QString title, QString thumbnail_url, int duration) {
    this->videoId = videoId;
    this->title = title;
    this->thumbnail_url = thumbnail_url;
    this->duration = duration;
}

VideoInfo::~VideoInfo() {

}

QString VideoInfo::getVideoId() {
    return videoId;
}

QString VideoInfo::getEmbeddedUrl() {
    return embedded_url;
}

QString VideoInfo::getTitle() {
    return title;
}

QString VideoInfo::getThumbnailURL() {
    return thumbnail_url;
}

int VideoInfo::getDuration() {
    return duration;
}

void VideoInfo::setEmbeddedUrl(QString embedded_url) {
    this->embedded_url = embedded_url;
}
