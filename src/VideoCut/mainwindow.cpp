#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDialog>
#include <QMouseEvent>
#include <QMessageBox>
#include "outdialog.h"
#include "doubledialog.h"
#include "tupiandialog.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Qtimer
    //connect:只要发出时间信号，就会在槽函数onTimerOut更新一些值
    timer = new QTimer(this);
    timer -> setInterval(40);//周期1000ms
    //timer->start();
    connect(timer,SIGNAL(timeout()),this,SLOT(onTimerOut()));

    //player->videosink->videoImage
    //connect:只要帧变化，就会修改curImage
    player = new QMediaPlayer(this);
    player->setVideoOutput(ui->videoWidget);
    videoSink = new QVideoSink(this);
    videoSink = ui->videoWidget->videoSink();//不设置这个初始值的话，视频抽帧不进行
    connect(videoSink,&QVideoSink::videoFrameChanged,this,&MainWindow::setCurImage);

    //infoDlg:forWard backWard
    connect(&infoDlg,SIGNAL(moveSignal(bool,int)),this,SLOT(moveTimeLabel(bool,int)));

    //建立工作文件夹 resources
    FileOP.DeleteFileOrFolder(".\\sources");//先把它删了再重建
    QDir dir(".\\");
    if(!dir.exists("ms"))
    {
        dir.mkdir(QString::fromLocal8Bit("sources"));
    }

    //frame双击添加文字
    ui->frame->installEventFilter(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    //视频轨道和字幕轨道
    //border-color:red green blue pink
    //dotted dashed solid
    ui->scaleWidget->setStyleSheet("border:1px dotted red;");
    ui->videoTrackWidget->setStyleSheet("border:1px dotted green");
    ui->subTextWidget->setStyleSheet("border:1px dashed blue");
    ui->tupianWidget->setStyleSheet("border:1px dashed blue");
    ui->frame->setStyleSheet("border:1px solid pink;");
    //时间刻度
    int startPos = ui->scaleWidget->pos().x();
    int height = ui->scaleWidget->pos().y()+ui->scaleWidget->height()+10;//我这里和他不同是因为我的两个Widget之间有缝隙
    int width = ui->scaleWidget->width();
    QPen hPen(Qt::black,2);
    painter.setPen(hPen);
    painter.drawLine(startPos,height,startPos+width,height);
    QPen vPen(Qt::black,1);
    painter.setPen(vPen);
    QFont font("宋体",8);
    painter.setFont(font);
    int unitScale = 10;//单位刻度
    int maxScale = ui->scaleWidget->width()/unitScale;
    for(int i=0;i<=maxScale;i++)
    {
        int x = i*unitScale;
        painter.drawLine(x+startPos,height-5,x+startPos,height);
        if(i%2 == 0)
        {
            painter.drawText(x+startPos-3,height-8,QString::number(i));
        }
    }
    //刻度针
    QPen vpen(Qt::red,1);
    painter.setPen(vpen);
    painter.drawLine(curPos+startPos,height,curPos+startPos,ui->subTextWidget->y()+2*ui->subTextWidget->height());
    //帧图片
    QRect videoRect(750,42,480,270);
    //painter.drawImage(videoRect,curImage);

    painter.drawImage(videoRect,curPic);
}
//Qtimer
void MainWindow::onTimerOut()
{
    //字幕更新
    curText = "";
    ui->textLabel->hide();
    if(index<textList.count())
    {
        curText = textList.at(index);
    }
    ui->textLabel->setText(curText);
    if(curText!="")
    {
        //ui->textLabel->show();
    }


    if(index<picList.count())
    {
        curPic.load(picList[index]);
        index++;
    }
    else//播放结束就循环
    {
        index = 0;
    }
    //slider和刻度针的位置
    if(picList.count()!=0)
    {
        ui->horizontalSlider->setValue(1000*index/picList.count());
        curPos = 10*index*40/1000;//curPos单位是100ms
    }
    this->update();
}
//弃用
void MainWindow::setCurImage(const QVideoFrame &frame)
{
    curImage = frame.toImage();
}
void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    //timer->stop();
    int totaltime = 10*picList.count()/25;//单位100ms
    curPos = position*totaltime/1000;//1000是slider的maxValue positon是当前value
    index = picList.count()*position/1000;
    this->update();
}
void MainWindow::on_horizontalSlider_sliderReleased()
{
    //timer->start();
}
void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    //qDebug()<<value;
}
//播放暂停按钮
void MainWindow::on_playBtn_clicked()
{
    if(isplay)//如果正在播放
    {
        //player->pause();
        timer->stop();
        ui->playBtn->setIcon(QIcon(":/icons/icons/play.ico"));
    }
    else
    {
        //player->play();
        upPicList();

        timer->start();
        ui->playBtn->setIcon(QIcon(":/icons/icons/pause.ico"));
    }
    isplay = !isplay;
}
//打开视频按钮
void MainWindow::on_openVideoBtn_clicked()
{
    QFileDialog *f = new QFileDialog(this);
    f->setNameFilter("*.mp4 *.mkv *.mpeg *.avi *.wmv");
    QString filePath;
    if(f->exec() == QDialog::Accepted)
    {
        filePath = f->selectedFiles()[0];
        ui->videoListWidget->addItem(filePath);
        //将视频转为25帧的图片 存在工作目录bulid-sources
        QDir dir(".\\sources");
        if(!dir.exists("ms"))
        {
            //dir.mkdir(QString::fromLocal8Bit("dasd"));
            dir.mkdir(QString::number(count));
            QString strPath = filePath.replace("/","\\");
            QString currentPath = QDir::currentPath();
            currentPath = currentPath.replace("/","\\");
            QString outPath = currentPath +"\\sources"+"\\"+QString::number(count);
            FF.toPicture(filePath,outPath);
            int times = GD.getDura(strPath);
            for(int i=0;i<times*25/1000;i++)
            {
                QString path = currentPath+"\\sources\\"+QString::number(count)+"\\"+QString::number(i)+".jpg";
                videoList[count].append(path);
            }
            mp[filePath]=videoList[count];
            count++;
        }
    }
}
//添加到轨道-按钮
void MainWindow::on_addToTrackBtn_clicked()
{
    //没有判空条件会crashed
    if(ui->videoListWidget->currentItem()!=nullptr)
    {
        QString file = ui->videoListWidget->currentItem()->text();
        //qDebug()<<file;
        QStringList namelist= file.split("/");
        QString name = "";//准备写在label上的
        if(!namelist.isEmpty())
        {
            name = namelist.last();
        }
        int totalTime = GD.getDura(file);//单位:ms
        //qDebug()<<totalTime;
        //timeLabel的基础设置
        QLabel *timeLabel = new QLabel(this);
        int x = ui->videoTrackWidget->pos().x();
        int y = ui->videoTrackWidget->pos().y()+21;
        if(!list_TimeLabel.empty())
        {
            int len = list_TimeLabel.count();
            QLabel *temp = list_TimeLabel.at(len-1);
            x = temp->pos().x()+temp->width();
        }
        int width = totalTime/100;//1s是10个像素点
        int height = ui->videoTrackWidget->height();
        timeLabel->move(x,y);
        timeLabel->resize(width,height);
        timeLabel->setText(name);
        timeLabel->setFrameShape(QFrame::Box);
        timeLabel->setStyleSheet("border-width: 1px;border-style: solid;border-color: rgb(255, 170, 0);");
        timeLabel->show();
        list_TimeLabel.append(timeLabel);
        //map:QLabel-videoInfo
        videoInfo *videoInfo = new struct videoInfo;
        videoInfo->videoPath=file;
        videoInfo->begin=0;
        videoInfo->end=totalTime;
        QString sfile = file.replace("/","\\");
        videoInfo->videoList=mp.value(sfile);
        map_TimeLabel[timeLabel] = videoInfo;
        //一定安装事件过滤器，为之后的鼠标事件做准备
        timeLabel->installEventFilter(this);

        upPicList();

    }
}

void MainWindow::on_actionPick_triggered()
{
    if(curTool == pick)
    {
        return;
    }
    curTool = pick;
    this->setCursor(Qt::ArrowCursor);
}


void MainWindow::on_actionClip_triggered()
{
    if(curTool == clip)
    {
        return;
    }
    curTool = clip;
    this->setCursor(Qt::IBeamCursor);
}

bool MainWindow::eventFilter(QObject* obj,QEvent *event)
{

    if(obj != nullptr && list_TimeLabel.contains(obj))
    {
        QLabel *object = dynamic_cast<QLabel *>(obj);
        //将抽象的QObject转换为 实例QLabel
        if(event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *mousePress = static_cast<QMouseEvent *>(event);
            if(mousePress->button() == Qt::LeftButton)
            {
                if(curTool == clip)
                {
                    QLabel *newTimeLabel = new QLabel(this);
                    newTimeLabel->installEventFilter(this);
                    newTimeLabel->setText(object->text());
                    //int x = timeLabel->pos().x()+timeLabel->width();
                    int prex=object->pos().x();
                    int x = mousePress->pos().x()+object->pos().x();
                    int y = object->pos().y();
                    int nextWidth  = x - object->pos().x();
                    int preWidth = object->pos().x()+object->width()-x;
                    int height = object->height();
                    object->resize(preWidth,height);
                    object->move(x,y);

                    //更新info
                    videoInfo *preInfo = map_TimeLabel.value(object);
                    videoInfo *newInfo = new videoInfo;
                    newInfo->videoPath = preInfo->videoPath;
                    newInfo->begin=preInfo->begin;
                    newInfo->end=newInfo->begin+nextWidth*100;//一个像素点100ms
                    newInfo->videoList=preInfo->videoList;
                    preInfo->begin=preInfo->end-preWidth*100;
                    map_TimeLabel[newTimeLabel] = newInfo;
                    //end 更新info

                    newTimeLabel->move(prex,y);
                    newTimeLabel->setFrameShape(QFrame::Box);
                    newTimeLabel->setStyleSheet("border-width: 1px;border-style: solid;border-color: rgb(255, 170, 0);");
                    newTimeLabel->resize(nextWidth,height);
                    newTimeLabel->show();
                    int index = list_TimeLabel.indexOf(object);
                    list_TimeLabel.insert(index,newTimeLabel);
                    //list_TimeLabel.append(newTimeLabel);
                    return true;
                }
                else if(curTool == pick)
                {
                    curTimeLabel = object;
                    if(object == nullptr) qDebug()<<"NULL";
                    videoInfo *tempInfo = map_TimeLabel.value(object);
                    if(tempInfo==nullptr) qDebug()<<"nullptr";
                    if(tempInfo!=nullptr)
                    {
                        infoDlg.setInfo(tempInfo->videoPath,tempInfo->begin,tempInfo->end);
                    }
                    infoDlg.show();
                    //qDebug()<<list_TimeLabel.indexOf(curTimeLabel);
                    //这里的curTimeLabel还没清空
                    //cut
                    if(infoDlg.exec() == QDialog::Accepted)
                    {
                        QStringList prelist = tempInfo->videoList;
                        QStringList newlist;
                        int head = 25*infoDlg.t1/1000;
                        int tail = 25*infoDlg.t2/1000;
                        QString name = infoDlg.name;
                        for(int i=head;i<tail;i++)
                        {
                            newlist.append(prelist[i]);
                        }
                        mp[name]=newlist;
                        ui->videoListWidget->addItem(name);
                    }
                }

            }
            else if(mousePress->button() == Qt::RightButton)
            {
                //得调整它之后的所有位置
                int index = list_TimeLabel.indexOf(object);
                int width = object->width();
                //qDebug()<<index;
                list_TimeLabel.removeAt(index);
                map_TimeLabel.remove(object);
                delete object;
                object = nullptr;
                for(int i=index;i<list_TimeLabel.length();i++)
                {
                    QLabel *temp = list_TimeLabel.at(i);
                    int x = temp->pos().x() - width;
                    int y = temp->pos().y();
                    temp->move(x,y);
                }
            }
            else return false;
        }
    }
    if(obj!=nullptr && obj == ui->frame)//双击添加文字
    {
        if(event->type() == QEvent::MouseButtonDblClick)
        {
            QMouseEvent *mouseDouble = static_cast<QMouseEvent *>(event);
            if(mouseDouble->button() == Qt::LeftButton)
            {
                upPicList();
                int x = mouseDouble->pos().x();
                int y = mouseDouble->pos().y();
                qDebug()<<"doubleClick";
                qDebug()<<"x:"<<x;
                qDebug()<<"y:"<<y;
                doubleDialog ldg;
                ldg.setxy(x,y);
                ldg.show();
                if(ldg.exec() == QDialog::Accepted)
                {
                    QString text = ldg.getText();
                    int t = ldg.getTime();
                    if(text != "")
                    {
                        for(int i=index;i<index+25*t;i++)
                        {
                            if(i>=picList.count())
                                break;
                            QImage image(1920,1080,QImage::Format_ARGB32);
                            QPainter p(&image);
                            QPen pen(Qt::green,3);
                            QFont font("宋体",50,QFont::Bold);
                            p.setPen(pen);
                            p.setFont(font);
                            p.drawImage(0,0,QImage(picList[i]).scaled(1920,1080));
                            p.drawText(x*4,y*4,text);
                            image.save(picList[i]);
                        }
                        //添加label
                        QLabel *label = new QLabel(this);
                        int posx = 10*index/25+ui->subTextWidget->x();
                        int posy = ui->subTextWidget->y()+22;
                        int width = t*10;
                        int height = ui->subTextWidget->height();
                        label->move(posx,posy);
                        label->resize(width,height);
                        label->setText(text);
                        label->setFrameShape(QFrame::Box);
                        label->setStyleSheet("border-width: 1px;border-style: solid;border-color: rgb(255, 100, 255);");
                        label->show();
                    }
                }
                return true;
            }
        }
    }
    return QWidget::eventFilter(obj,event);
}

void MainWindow::moveTimeLabel(bool direct,int dis)
{
    if(curTimeLabel == nullptr)
    {
        return;
    }
    qDebug()<<curTimeLabel;
    int index = list_TimeLabel.indexOf(curTimeLabel);
    qDebug()<<"index:"<<index;
    if(index+1 == list_TimeLabel.length() && direct == true)
    {
        return;
    }
    if(index == 0 && direct == false)
    {
        return;
    }
    int width = curTimeLabel->width();
    //int x = curTimeLabel->pos().x();

    list_TimeLabel.removeAt(index);//先把它弄出去

    if(direct)//向右
    {
        curTimeLabel->hide();//先隐藏起来，影响视觉
        int right = list_TimeLabel.length()-index;//他右边有多少个
        qDebug()<<"right:"<<right;
        //right=0即他就在最右边
        dis = right<dis ? right:dis;//超出了那就算最后一个
        for(int i=index;i<index+dis;i++)
        {
            QLabel *temp = list_TimeLabel.at(i);
            int x = temp->pos().x() - width;
            int y = temp->pos().y();
            temp->move(x,y);
        }
        QLabel *temp = list_TimeLabel.at(index+dis-1);//最右边的那个
        int x = temp->pos().x()+temp->width();
        int y = temp->pos().y();
        curTimeLabel->move(x,y);
        list_TimeLabel.insert(index+dis,curTimeLabel);//insert表示将要插入的位置，不是在某个位置后面
        curTimeLabel->show();//再恢复
    }
    else//向左
    {
        curTimeLabel->hide();
        if(dis>index)
        {
            dis = index;
        }
        for(int i=index-1;i>=index-dis;i--)//从后往前依次开始挪
        {
            QLabel *temp = list_TimeLabel.at(i);
            int x = temp->pos().x()+width;
            int y = temp->pos().y();
            temp->move(x,y);
        }
        QLabel *temp = list_TimeLabel.at(index-dis);
        int x = temp->pos().x() - width;
        int y = temp->pos().y();
        curTimeLabel->move(x,y);
        list_TimeLabel.insert(index-dis,curTimeLabel);
        curTimeLabel->show();
    }
}

void MainWindow::upPicList()
{
    picList.clear();
    qDebug()<<list_TimeLabel.count();
    for(int i=0;i<list_TimeLabel.count();i++)
    {
        QLabel *temp = list_TimeLabel.at(i);
        videoInfo *info = map_TimeLabel.value(temp);
        int head = info->begin*25/1000;
        int tail = info->end*25/1000;
        qDebug()<<info->videoList.count();
        qDebug()<<"head:"<<head;
        qDebug()<<"tail:"<<tail;
        for(int j = head;j < tail;j++)
        {
            QString s = info->videoList.at(j);
            picList.append(s);
        }
    }
    qDebug()<<picList.count();
}



void MainWindow::on_importBtn_clicked()
{

    upPicList();
    QListWidgetItem *item = ui->materialListWidget->currentItem();
    if(item == nullptr) return;
    if(picList.count() < 100) return;
    QString file = item->text();//图片地址
    file.replace("/","\\");
    tupianDialog dlg;
    dlg.show();
    if(dlg.exec()==QDialog::Accepted)
    {
        int x = dlg.getx();
        int y = dlg.gety();
        int t = dlg.gett();
        QString effect = dlg.getEffect();
        QImage image(1920,1080,QImage::Format_ARGB32);
        QPainter p(&image);
        if(effect == "fade")
        {
            double tt = 0;
            bool isadd = true;
            for(int it = index;it<index+t*25;it++)//持续t秒
            {
                if(isadd) tt += 0.02;
                else tt -=0.02;
                if(tt>=1)
                {
                    tt=1;
                    isadd = !isadd;
                }
                if(tt<=0)
                {
                    tt=0;
                    isadd = !isadd;
                }
                if(index >= picList.count()) break;
                p.setOpacity(1.0);
                p.drawImage(0,0,QImage(picList[it]).scaled(1920,1080));
                p.setOpacity(tt);
                p.drawImage(x,y,QImage(file));
                image.save(picList[it]);
            }
        }
        if(effect == "big")
        {
            for(int it = index;it<index+25*t;it++)
            {
                if(index >= picList.count()) break;
                p.drawImage(0,0,QImage(picList[it]).scaled(1920,1080));
                int sx = 128+(it-index)*5;
                int sy = 128+(it-index)*5;
                p.drawImage(x,y,QImage(file).scaled(sx,sy));
                image.save(picList[it]);
            }
        }
        if(effect == "move")
        {
            for(int it = index;it<index+25*t;it++)
            {
                if(index >= picList.count()) break;
                p.drawImage(0,0,QImage(picList[it]).scaled(1920,1080));
                int sx = x+(it-index)*5;
                int sy = y+(it-index)*5;
                p.drawImage(sx,sy,QImage(file));
                image.save(picList[it]);
            }
        }
        if(effect == "no")
        {
            for(int it = index;it<index+25*t;it++)
            {
                if(index >= picList.count()) break;
                p.drawImage(0,0,QImage(picList[it]).scaled(1920,1080));
                p.drawImage(x,y,QImage(file));
                image.save(picList[it]);
            }
        }
        //添加QLabel
        QLabel *label = new QLabel(this);
        int posx = 10*index/25+ui->tupianWidget->x();
        int posy = ui->tupianWidget->y()+22;
        int width = t*10;
        int height = ui->tupianWidget->height();
        label->move(posx,posy);
        label->resize(width,height);
        label->setText(effect);
        label->setFrameShape(QFrame::Box);
        label->setStyleSheet("border-width: 1px;border-style: solid;border-color: rgb(255, 100, 255);");
        label->show();
    }
}


void MainWindow::on_deleteSucaiBtn_clicked()
{
    QListWidgetItem *item = ui->materialListWidget->currentItem();
    if(item == nullptr) return;
    ui->materialListWidget->removeItemWidget(item);
    delete item;
    item = nullptr;
}


void MainWindow::on_addSucaiBtn_clicked()
{
    QFileDialog *f = new QFileDialog(this);
    f->setNameFilter("*.jpg *.png");
    QString filePath;
    if(f->exec() == QDialog::Accepted)
    {
        filePath = f->selectedFiles()[0];
        ui->materialListWidget->addItem(filePath);
    }
}

void MainWindow::on_addTextBtn_clicked()
{
    upPicList();
    if(picList.count()<100) return;
    doubleDialog dlg;
    dlg.show();
    if(dlg.exec() == QDialog::Accepted)
    {
        QImage image(1920,1080,QImage::Format_ARGB32);
        QPainter p(&image);
        int x = dlg.getx();
        int y = dlg.gety();
        int t = dlg.getTime();
        QString text = dlg.getText();
        for(int it = index;it<index+25*t;it++)
        {
            if(index >= picList.count()) break;
            p.drawImage(0,0,QImage(picList[it]).scaled(1920,1080));
            QPen pen(Qt::green,3);
            QFont font("宋体",50,QFont::Bold);
            p.setPen(pen);
            p.setFont(font);
            p.drawText(x,y,text);
            image.save(picList[it]);
        }

        QLabel *label = new QLabel(this);
        int posx = 10*index/25+ui->subTextWidget->x();
        int posy = ui->subTextWidget->y()+22;
        int width = t*10;
        int height = ui->subTextWidget->height();
        label->move(posx,posy);
        label->resize(width,height);
        label->setText(text);
        label->setFrameShape(QFrame::Box);
        label->setStyleSheet("border-width: 1px;border-style: solid;border-color: rgb(255, 100, 255);");
        label->show();
    }
}
void MainWindow::on_zimuOPenBtn_clicked()
{
    upPicList();
    if(picList.count()<100)//视频太短直接返回
    {
        return;
    }
    QFileDialog *f = new QFileDialog(this);
    f->setNameFilter("*.srt");
    QString filePath;
    if(f->exec() == QDialog::Accepted)
    {
        filePath = f->selectedFiles()[0];

        //将视频转为25帧的图片 存在工作目录bulid-sources
        QDir dir(".\\sources");
        if(!dir.exists("ms"))
        {
            dir.mkdir("zimu");
            QString strPath = filePath.replace("/","\\");
            QString currentPath = QDir::currentPath();
            currentPath = currentPath.replace("/","\\");
            QString outPath = currentPath +"\\sources\\zimu\\zimu.txt";
            QList<subText *> subText = FileOP.fromSrt(filePath,outPath);
            textList = FileOP.toTextList(subText);
            ui->zimuLineEdit->setText(currentPath+"\\sources\\zimu");

            //将字幕直接画在图片上面
            if(picList.count()>100)//图片数量大于一百
            {
                QImage image(1920,1080,QImage::Format_ARGB32);
                QPainter p(&image);
                for(int i=0;i<textList.count();i++)
                {
                    if(i >= picList.count()) break;
                    p.drawImage(0,0,QImage(picList[i]).scaled(1920,1080));
                    QPen pen(Qt::red,3);
                    QFont font("宋体",60,QFont::Bold);
                    p.setPen(pen);
                    p.setFont(font);
                    QRect rect(100,800,1600,100);
                    p.drawText(rect,Qt::AlignCenter,textList[i]);
                    image.save(picList[i]);
                }
            }
        }
    }
}



void MainWindow::on_outBtn_clicked()
{
    upPicList();
    if(picList.count()<100) return;
    //先把工程文件夹output删了,连续导出的时候copy不会覆盖
    FileOP.DeleteFileOrFolder(".\\sources\\output");
    QDir dir(".\\sources");
    if(!dir.exists("ms"))
    {
        dir.mkdir("output");
        QString currentPath = QDir::currentPath();
        currentPath = currentPath.replace("/","\\");
        QString outPath = currentPath +"\\sources\\output";
        for(int i=0;i<picList.count();i++)
        {
            QFile::copy(picList[i],outPath + "\\" +QString::number(i)+".jpg");
        }
        for(int i=25;i<=picList.count();i++)
        {
            QString filepath = outPath+"\\"+QString::number(i)+".jpg";
            QFile file(filepath);
            if(!file.exists())
            {
                QString prepath = outPath+"\\"+QString::number(i-1)+".jpg";
                QFile::copy(prepath,filepath);
            }
        }
        outDialog dlg;
        dlg.show();
        if(dlg.exec()==QDialog::Accepted)
        {
            QString rate = dlg.getRate();
            QString scale = dlg.getScale();
            QString format = dlg.getFormat();
            QString strPath = outPath+"\\%d.jpg";
            QString out = currentPath+"\\sources\\"+"output"+format;

            FF.connect(strPath,out,scale,rate);
            QMessageBox::information(this,tr("视频导出"),tr("导出完成"),QMessageBox::Yes);
        }
    }
}





void MainWindow::on_mergeBtn_clicked()
{
    upPicList();
    if(picList.count()<50) return;
    QStringList tmplist = picList;
    int x = ui->videoTrackWidget->x();
    int y = ui->videoTrackWidget->y()+21;
    int width  = 10*tmplist.count()/25;
    int height = ui->videoTrackWidget->height();
    videoInfo *info = new videoInfo;
    info->videoList = tmplist;
    info->videoPath = "";
    info->begin = 0;
    info->end = width*100;
    QLabel *newLabel = new QLabel(this);
    newLabel->move(x,y);
    newLabel->resize(width,height);
    newLabel->setText("merge");
    newLabel->setFrameShape(QFrame::Box);
    newLabel->setStyleSheet("border-width: 1px;border-style: solid;border-color: rgb(255, 170, 0);");
    newLabel->installEventFilter(this);

    for(int i=0;i<list_TimeLabel.count();i++)
    {
        QLabel *tmp = list_TimeLabel.at(i);
        tmp->hide();
    }
    list_TimeLabel.clear();

    newLabel->show();
    list_TimeLabel.append(newLabel);
    map_TimeLabel[newLabel]=info;
    qDebug()<<"now num:"<<list_TimeLabel.count();
}

