#ifndef INFO_H
#define INFO_H

#include <QString>
#include <QStringList>

struct videoInfo
{
    QString videoPath;//视频路径
    int begin;//hh::mm::ss->ms毫秒
    int end;//起止时间
    QStringList videoList;
};
struct subText
{
    QString text;//字幕内容
    int begin;
    int end;//起止时间
};
enum tools
{
    pick,
    clip
};
#endif // INFO_H
