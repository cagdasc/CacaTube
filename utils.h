#ifndef UTILS_H
#define UTILS_H

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QString>
#include <QList>

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

    static bool savePlaylist(QString json, QString file_name);
    static bool loadPlaylist(QString file_name, QString *json);
};

#endif // UTILS_H
