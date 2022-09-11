#include "fileop.h"
#include "ffmpeg.h"
#include <QProcess>
#include <QDebug>
#include <QDir>
#include <QMessageBox>
FFmpeg::FFmpeg()
{
    QString curPath = QDir::currentPath().replace("/","\\");
    program = curPath + "\\ffmpeg\\bin\\ffmpeg.exe";
}
bool FFmpeg::cut(QString &filepath,QString &begin,QString &end,QString &outpath)
{
    //ffmpeg -i input.mp4 -ss 1:05 -t 10 output.mp4
    QProcess *po = new QProcess;
    QStringList argu;
    argu.append("-i");
    argu.append(filepath);
    argu.append("-ss");
    argu.append(begin);
    //-t是区间，-to是终止时间
    //argu.append("-t");
    argu.append("-to");
    argu.append(end);
    //argu.append("outpath");
    //这个应该作为参数传进来
    //QString outfilename = "hello.mp4";//这里的name和上面的Path之间有个空格 所以失败了
    argu.append(outpath);
    //qDebug()<<program;
    //qDebug()<<argu;
    //不用加空格，因为它们会自动加
    po->start(program,argu);
    //这里加一个判断程序结束的条件，再return
    return true;
}

bool FFmpeg::concat(QString &cattxtpath,QString &outpath)
{
    QProcess *po = new QProcess;
    QStringList argu;
    argu.append("-f");
    argu.append("concat");
    argu.append("-safe");//concat unsafe filename
    argu.append("0");//这个 -safe 0写在一起还不行
    argu.append("-i");
    argu.append(cattxtpath);
    // -c copy 如果文件格式都相同则不用转码了
    argu.append("-c");
    argu.append("copy");
    argu.append(outpath);
    po->start(program,argu);
    return true;
}
bool FFmpeg::addSubTitle(QString &videoPath,QString &subTitlePath,QString &outPath)
{
    //默认采用 硬字幕
    //ffmpeg -i test_1280x720_3.mkv -vf subtitles=test_1280x720_3.srt out.mp4
    //subtitles=C\\:\\\\Users\\\\geh20\\\\Videos\\\\zimu.srt
    //这个过滤器需要很多的转义符
    QProcess *po = new QProcess;
    QStringList argu;

    argu.append("-i");
    argu.append(videoPath);
    //argu.append("-vf");
    argu.append("-filter_complex");
    //给过滤器添加双倍转义符
    //s.replace("\\","\\\\");
    //s.insert(1,"\\");
    //qDebug()<<s;
    //argu.append("subtitles="+s);//硬字幕路径有问题
    QString s = subTitlePath;
    s.replace("\\","/");
    s.insert(1,"\\");
    s="'"+s+"'";//两端加了两个单引号
    s = "subtitles="+s;
    //qDebug()<<s;
    argu.append(s);

    argu.append(outPath);
    qDebug()<<argu;
    po->start(program,argu);
    //po->waitForFinished();
    //如果已经有了这个文件，那么会无限等待下去
    //qDebug()<<QString::fromLocal8Bit(po->readAllStandardError());
    //Error initializing filter 'subtitles' with
            //args 'C:\\Users\\geh20\\Videos\\qt\\zimu.srt'
    return true;
}

bool FFmpeg::toPicture(QString &strPath,QString &outPath)
{
    QProcess *po = new QProcess;
    QStringList argu;
    argu.append("-i");
    argu.append(strPath);
    argu.append("-r");
    argu.append("25");
    argu.append("-q:v");
    argu.append("2");
    argu.append("-f");
    argu.append("image2");
    outPath += "\\%d.jpg";
    argu.append(outPath);
    po->start(program,argu);

    //qDebug()<<argu;
    po->waitForFinished();
    qDebug()<<"toPicture exitCode:"<<po->exitCode();
    //qDebug()<<QString::fromLocal8Bit(po->readAllStandardError());
    return true;
}

bool FFmpeg::connect(QString &dirPath,QString &outPath,QString scale,QString rate)
{
    QProcess *po = new QProcess;
    QStringList argu;
    argu.append("-y");
    argu.append("-r");
    argu.append("25");
    argu.append("-f");
    argu.append("image2");
    argu.append("-i");
    argu.append(dirPath);
    argu.append("-c:v");
    argu.append("libx265");
    argu.append("-b:v");//码率
    argu.append(rate);
    argu.append("-s");//分辨率
    argu.append(scale);
    argu.append(outPath);

    po->start(program,argu);
    //qDebug()<<argu;
    po->waitForFinished();
    qDebug()<<"FFmpeg::connect exitCode:"<<po->exitCode();
    //qDebug()<<QString::fromLocal8Bit(po->readAllStandardError());
    return true;
}
