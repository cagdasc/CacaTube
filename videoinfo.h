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

#ifndef VIDEOINFO_H
#define VIDEOINFO_H

#include <QString>

class VideoInfo
{
public:
    VideoInfo();
    VideoInfo(QString videoId, QString title, QString thumbnail_url, int duration_ms);
    ~VideoInfo();



    QString getVideoId();
    QString getTitle();
    QString getThumbnailURL();
    int getDurationMs();

private:

    QString videoId;
    QString title;
    QString thumbnail_url;
    int duration_ms;

};

#endif // VIDEOINFO_H
