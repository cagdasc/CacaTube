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

#ifndef UTILS_H
#define UTILS_H

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QString>
#include <QList>
#include <QMap>

#include "youtubeprocess.h"
#include <iostream>
#include <fstream>

using namespace std;

#include "videoinfo.h"

class Utils
{
public:
    Utils();
    ~Utils();

    static QString createJsonString(QList<VideoInfo> playlist);
    static void parseSearchJson(QList<VideoInfo> *playlist, QString json);
    static void parsePlaylistJson(QList<VideoInfo> *playlist, QString json);
    static QMap<QString, QString> parseAPIReqJson(QString json, int *duration);

    static bool savePlaylist(QString json, QString file_name);
    static bool loadPlaylist(QString file_name, QString *json);
};

#endif // UTILS_H
