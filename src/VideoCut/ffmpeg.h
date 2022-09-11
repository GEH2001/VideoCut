#ifndef FFMPEG_H
#define FFMPEG_H


#include <QString>

class FFmpeg
{
    //Q_OBJECT
public:
    FFmpeg();

    //对某个视频进行裁剪
    bool cut(QString &filepath,QString &begin,QString &end,QString &outpath);
    bool concat(QString &cattxtpath,QString &outpath);
    bool addSubTitle(QString &videoPath,QString &subTitlePath,QString &outPath);
    bool toPicture(QString &strPath,QString &outPath);
    bool connect(QString &dirPath,QString &outPath,QString scale,QString rate);
private:
    //ffmpeg.exe
    //QString program = "C:\\Codefield\\QtCode\\VideoCut\\ffmpeg\\bin\\ffmpeg.exe";
    QString program;
};

#endif // FFMPEG_H
