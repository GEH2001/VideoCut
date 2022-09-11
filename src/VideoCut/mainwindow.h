#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QPainter>
#include <QTimer>
#include <QVideoFrame>
#include <QVideoSink>
#include <QLabel>
#include <QMap>
#include <QListWidgetItem>

#include "info.h"
#include "infodialog.h"
#include "ffmpeg.h"
#include "fileop.h"
#include "getduration.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QLabel *getPosLabel();//取得当前刻度针curPos对应的timeLabel
    void upPicList();//更新upPicList
protected:
    void paintEvent(QPaintEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);
private:
    Ui::MainWindow *ui;

    bool isplay = false;//播放的状态:true-正在播放
    int curPos = 0;//当前刻度针所在位置
    int curPlayerPos = 0;//当前播放器的位置-单位:ms
    tools curTool = pick;
    QImage curImage;//当前帧图片
    QString curText;//当前字幕
    QString curVideoPath;//当前视频路径
    QMediaPlayer *player;
    QVideoSink *videoSink;
    QTimer *timer;

    QList <QLabel*> list_TimeLabel;
    QMap <QLabel *,videoInfo *> map_TimeLabel;
    InfoDialog infoDlg;
    QLabel *curTimeLabel = nullptr;

    FFmpeg FF;//ffmpeg工具
    FileOP FileOP;//文件相关的操作
    GetDuration GD;//获取文件时长
    //测试用
    int count = 0;//图片文件夹的命名
    QStringList picList;
    QImage curPic;
    int index = 0;
    QStringList videoList[10];//10个videoList
    QMap<QString,QStringList> mp;//QListWidgetItem.text<->video.QStringList
    QMap<QListWidgetItem*,QStringList> map_item_list;
    QStringList textList;//字幕按帧存的List

public slots:
    void onTimerOut();
    void setCurImage(const QVideoFrame &frame);
    void moveTimeLabel(bool direct,int dis);
private slots:
    void on_horizontalSlider_sliderMoved(int position);
    void on_playBtn_clicked();
    void on_openVideoBtn_clicked();
    void on_addToTrackBtn_clicked();
    void on_actionPick_triggered();
    void on_actionClip_triggered();
    void on_horizontalSlider_sliderReleased();
    void on_horizontalSlider_valueChanged(int value);
    void on_importBtn_clicked();
    void on_deleteSucaiBtn_clicked();
    void on_addSucaiBtn_clicked();
    void on_zimuOPenBtn_clicked();
    void on_outBtn_clicked();
    void on_addTextBtn_clicked();
    void on_mergeBtn_clicked();
};
#endif // MAINWINDOW_H
