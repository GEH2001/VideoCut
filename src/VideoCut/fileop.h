#ifndef FILEOP_H
#define FILEOP_H
//文件处理相关的操作
/*
 * @DeleteFileOrFolder:删除路径对应的文件或者文件夹
 * @fromSrt:srt->txt,读取内容并返回QList<subText*>
 * @QString:从textlist获取当前播放位置timeMile对应的字幕
 * @getDuration：获取视频文件的播放时长,返回值为int类型，单位为毫秒
 * @toTextList:将QList<subText*>按照25帧率转为QStringList
*/
#include <QString>
#include "info.h"
class FileOP
{
public:
    FileOP();
    bool DeleteFileOrFolder(const QString &strPath);

    QList<subText*> fromSrt(const QString &strPath,const QString &outPath);
    QString GetTextString(QList<subText*> &textList,int timeMile);
    QStringList toTextList(QList<subText*>& list);
};

#endif // FILEOP_H
