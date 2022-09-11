#include "getduration.h"
#include <QDebug>
GetDuration::GetDuration()
{

}
int GetDuration::getDura(const QString &strPath)
{
    //ffmpeg测试
    //unsigned version = avcodec_version();
    //qDebug()<<"version:"<<version;

    //先获取名字，并且保存
    //QString strPath = "C:\\Users\\geh20\\Videos\\out.mp4";
    QByteArray ba = strPath.toLatin1();
    char *cinputFilePath;
    cinputFilePath = ba.data();
    AVFormatContext* fmt_ctx = avformat_alloc_context();
    int avformat_open_result = avformat_open_input(&fmt_ctx,cinputFilePath,NULL,NULL);
    if (avformat_open_result != 0)
    {
        //获取异常信息
        char* error_info = new char[32];
        av_strerror(avformat_open_result, error_info, 1024);
        qDebug()<<QString("异常信息 %1").arg(avformat_open_result);
    };

    //此函数自动打印输入或输出的详细信息
    //av_dump_format(fmt_ctx, 0, cinputFilePath, 0);
    int times = fmt_ctx->duration/1000;
    //qDebug()<<"times:"<<times;
    //需要使用fflush(stderr)把缓冲区的数据打印出来
    //fflush(stderr);

    //有打开就要有关闭
    avformat_close_input(&fmt_ctx);

    return times;
}
