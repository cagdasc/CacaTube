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
        object["duration"] = v.getDuration();
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
        QJsonObject in_array_obj = json_array[i].toObject();

        QJsonObject temp_obj = in_array_obj["id"].toObject();
        QString videoId = temp_obj["videoId"].toString();

        //std::cout <<videoId.toStdString()<< std::endl;

        temp_obj = in_array_obj["snippet"].toObject();
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
        QJsonObject in_array_obj = json_array[i].toObject();

        QString title = in_array_obj["title"].toString();
        QString url = in_array_obj["url"].toString();
        int duration = in_array_obj["duration"].toInt();

        VideoInfo v(url, title, 0, duration);
        playlist->push_back(v);

    }
}

VideoInfo Utils::parseAPIReqJson(QString json) {
    QMap<QString, QString> format_url_map;

    QJsonDocument json_string = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject json_object = json_string.object();

    QString title = json_object["title"].toString();
    QString embedded_url = json_object["url"].toString();
    QString thumbnail_url = json_object["thumbnail"].toString();
    int duration = json_object["duration"].toInt();

    VideoInfo video_info(0, title, thumbnail_url, duration);
    video_info.setEmbeddedUrl(embedded_url);

    return video_info;
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
