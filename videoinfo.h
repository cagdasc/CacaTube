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
