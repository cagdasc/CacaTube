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

#ifndef YOUTUBEPROCESS_H
#define YOUTUBEPROCESS_H

#include <iostream>
#include <QObject>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QByteArray>
#include <QString>
#include <QLinkedList>
#include "videoinfo.h"
#include "cacatube_const.h"

class YoutubeProcess : public QObject
{
    Q_OBJECT
public:
    explicit YoutubeProcess(QObject *parent = 0);
    YoutubeProcess(QObject *, QNetworkAccessManager *);
    ~YoutubeProcess();

    void setQ(QString query);
    void setVideo_Id(QString video_id);
    QNetworkAccessManager *getAccessManager();

    QNetworkReply *executeSearch();
    QNetworkReply *executeSearch(QString pageToken);
    QNetworkReply *executeVideos();

    static int max_result;
    static QString youtube_link;
    //static void parse_search_json(QList<VideoInfo> *list, QString q);

    static int total_results;
    static QString next_page_token;
    static QString prev_page_token;


private:
    QString video_id;
    QString query;
    QNetworkAccessManager *access_manager;

    static QString youtube_api_url;

signals:

public slots:
};

#endif // YOUTUBEPROCESS_H
