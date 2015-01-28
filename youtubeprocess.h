#ifndef YOUTUBEPROCESS_H
#define YOUTUBEPROCESS_H

#include <iostream>
#include <QObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QByteArray>
#include <QString>
#include <QList>

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
    void setNextPageToken(QString next_page_token);
    QNetworkAccessManager *getAccessManager();

    QNetworkReply *executeSearch();
    QNetworkReply *executeVideos();

    static int max_result;
    static QString youtube_link;
    static void parse_search_json(QList<VideoInfo> *list, QString q);


private:
    QString video_id;
    QString query;
    QString next_page_token;
    QNetworkAccessManager *access_manager;

    static QString youtube_api_url;

signals:

public slots:
};

#endif // YOUTUBEPROCESS_H