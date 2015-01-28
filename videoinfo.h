#ifndef VIDEOINFO_H
#define VIDEOINFO_H

#include <QString>

class VideoInfo
{
public:
    VideoInfo(QString videoId, QString title, QString thumbnail_url, long duration_ms);
    ~VideoInfo();

    QString videoId;
    QString title;
    QString thumbnail_url;
    long duration_ms;

private:
    QString getVideoId();
    QString getTitle();
    QString getThumbnailURL();
    long getDurationMs();
};

#endif // VIDEOINFO_H
