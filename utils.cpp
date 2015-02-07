#include "utils.h"

Utils::Utils()
{

}

Utils::~Utils()
{

}

QString Utils::createJsonString(QList<VideoInfo> playlist) {
    QJsonObject root;
    QJsonArray items;
    for (int i = 0; i < playlist.size(); ++i) {
        VideoInfo v = playlist.at(i);
        QJsonObject object;
        object["title"] = v.getTitle();
        object["url"] = v.getVideoId();
        object["duration"] = v.getDurationMs();
        items.append(object);
    }

    root["items"] = items;

    //cout << QString(QJsonDocument(root).toJson()).toStdString() << endl;
    return QString(QJsonDocument(root).toJson());

}

void Utils::parseSearchJson(QList<VideoInfo> *list, QString json) {

    QJsonDocument json_string = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject json_object = json_string.object();

    YoutubeProcess::total_results = json_object["pageInfo"].toObject()["totalResults"].toInt();
    //max_result = json_object["pageInfo"].toObject()["resultsPerPage"].toInt();
    YoutubeProcess::next_page_token = json_object["nextPageToken"].toString();
    YoutubeProcess::prev_page_token = json_object["prevPageToken"].toString();

    std::cout << "Total Results: " << YoutubeProcess::total_results << std::endl;
    std::cout << "Result Per Page: " << YoutubeProcess::max_result << std::endl;
    std::cout << "Next Page Token: " << YoutubeProcess::next_page_token.toStdString() << std::endl;
    std::cout << "Prev Page Token: " << YoutubeProcess::prev_page_token.toStdString() << std::endl;

    QJsonArray json_array = json_object["items"].toArray();


    for (int i = 0; i < json_array.size(); ++i) {
        QJsonObject npcObject = json_array[i].toObject();

        QJsonObject temp_obj = npcObject["id"].toObject();
        QString videoId = temp_obj["videoId"].toString();

        //std::cout <<videoId.toStdString()<< std::endl;

        temp_obj = npcObject["snippet"].toObject();
        QString title = temp_obj["title"].toString();

        temp_obj = temp_obj["thumbnails"].toObject();
        QString thumbnail_url = temp_obj["url"].toString();

        VideoInfo video_info(YoutubeProcess::youtube_link + videoId, title, thumbnail_url, 0);
        list->push_back(video_info);

        //std::cout << title.toStdString() << std::endl;

    }
}

void Utils::parsePlaylistJson(QList<VideoInfo> *playlist, QString json) {
    QJsonDocument json_string = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject json_object = json_string.object();

    QJsonArray json_array = json_object["items"].toArray();


    for (int i = 0; i < json_array.size(); ++i) {
        QJsonObject npcObject = json_array[i].toObject();

        QString title = npcObject["title"].toString();
        QString url = npcObject["url"].toString();
        int duration = npcObject["duration"].toInt();

        VideoInfo v(url, title, 0, duration);
        playlist->push_back(v);

    }
}

bool Utils::savePlaylist(QString json, QString file_name) {
    ofstream out_file;
    out_file.open(file_name.toStdString(), ios::out);

    if (!out_file.is_open()) {
        return false;
    }

    out_file << json.toStdString();
    out_file.close();
    return true;
}

bool Utils::loadPlaylist(QString file_name, QString *json) {
    ifstream in_file;
    in_file.open(file_name.toStdString(), ios::in);

    if (!in_file.is_open()) {
        return false;
    }

    in_file.seekg(0, ios::end);
    int size = in_file.tellg();
    in_file.seekg(0, ios::beg);

    char *data = new char[size];
    in_file.read(data, size);
    QByteArray array;
    for (int i = 0; i < size; ++i) {
        array.append(data[i]);
    }
    *json = QString(array);
    delete [] data;
    in_file.close();

    return true;
}
