#include "youtubeprocess.h"

int YoutubeProcess::max_result = 10;
QString YoutubeProcess::youtube_link = "https://www.youtube.com/watch?v=";
QString YoutubeProcess::youtube_api_url  = "https://www.googleapis.com/youtube/v3";

YoutubeProcess::YoutubeProcess(QObject *parent) : QObject(parent)
{

}

YoutubeProcess::YoutubeProcess(QObject *parent, QNetworkAccessManager *access_manager) : QObject(parent)
{
    this->access_manager = access_manager;
}

YoutubeProcess::~YoutubeProcess()
{

}

void YoutubeProcess::setQ(QString query) {
    this->query = query;
}

void YoutubeProcess::setVideo_Id(QString video_id) {
    this->video_id = video_id;
}

void YoutubeProcess::setNextPageToken(QString next_page_token) {
    this->next_page_token = next_page_token;
}

QNetworkReply *YoutubeProcess::executeSearch() {

    QUrlQuery query;
    QString max;
    max.setNum(max_result);
    query.addQueryItem("part", "id,snippet");
    query.addQueryItem("q", this->query);
    query.addQueryItem("maxResults", max);
    query.addQueryItem("key", api_key);
    query.addQueryItem("pageToken", next_page_token);

    QUrl url(this->youtube_api_url + "/search");
    url.setQuery(query.query());
    QNetworkRequest request(url);

    return access_manager->get(request);

}

QNetworkAccessManager *YoutubeProcess::getAccessManager() {
    return access_manager;
}

QNetworkReply *YoutubeProcess::executeVideos() {

    QUrlQuery query;
    query.addQueryItem("part", "contentDetails");
    query.addQueryItem("id", this->video_id);
    query.addQueryItem("key", api_key);

    QUrl url(this->youtube_api_url + "/videos");
    url.setQuery(query.query());
    QNetworkRequest request(url);

    return access_manager->get(request);
}

void YoutubeProcess::parse_search_json(QList<VideoInfo> *list, QString json) {

    QJsonDocument json_string = QJsonDocument::fromJson(json.toUtf8());

    QJsonObject json_object = json_string.object();
    QJsonArray json_array = json_object["items"].toArray();


    for (int i = 0; i < json_array.size(); ++i) {
            QJsonObject npcObject = json_array[i].toObject();

            QJsonObject temp_obj = npcObject["id"].toObject();
            QString videoId = temp_obj["videoId"].toString();

            std::cout <<videoId.toStdString()<< std::endl;

            temp_obj = npcObject["snippet"].toObject();
            QString title = temp_obj["title"].toString();

            temp_obj = temp_obj["thumbnails"].toObject();
            QString thumbnail_url = temp_obj["url"].toString();

            VideoInfo video_info(youtube_link + videoId, title, thumbnail_url, 0);
            list->push_back(video_info);

            std::cout << title.toStdString() << std::endl;

    }
}

