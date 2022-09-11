#include "fileop.h"
#include <QDir>
#include <QProcess>
FileOP::FileOP()
{

}
bool FileOP::DeleteFileOrFolder(const QString &strPath)//要删除的文件夹或文件的路径
{
    if (strPath.isEmpty() || !QDir().exists(strPath))//是否传入了空的路径||路径是否存在
        return false;

    QFileInfo FileInfo(strPath);

    if (FileInfo.isFile())//如果是文件
        QFile::remove(strPath);
    else if (FileInfo.isDir())//如果是文件夹
    {
        QDir qDir(strPath);
        qDir.removeRecursively();
    }
    return true;
}

QList<subText*> FileOP::fromSrt(const QString &strPath,const QString &outPath)
{
    QList<subText *> list;
    if (strPath.isEmpty() || !QDir().exists(strPath))//是否传入了空的路径||路径是否存在
        return list;//返回一个空的list
    QFile::copy(strPath,outPath);//拷贝重命名
    QFile file(outPath);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        qDebug()<<"Can't open the file!";
    }
    QString temp = "";
    //while(!file.atEnd())//最后一行直接是空行，所以没有进到循环里面
    while(1)
    {
        QByteArray line = file.readLine();
        QString str(line);
        str.replace("\n","");//读取的时候会把换行符\n读进来，将其换成空“”
        qDebug()<<str;

        if(str != "")//不是空行
        {
            //@是分隔符
            //continue之后的代码不会再进行，直接跳转到while的判断条件
            temp.append(str).append("@");
            continue;
        }//这个if保证了每一块字幕的读取然后再处理
        QStringList templist = temp.split("@");
        if(templist.count()<3)
        {   //每个正常的字幕块是有三行，所以如果少于三行说明这块字幕无效
            //这个判断条件 非必要
            temp.remove(0,templist.count());
            continue;//继续下一次循环
        }

        QString strToTime = templist[1];//那个序号直接跳过不处理
        int begin_hour = strToTime.mid(0,2).toInt();
        int begin_minute = strToTime.mid(3,2).toInt();
        int begin_second = strToTime.mid(6,2).toInt();
        int begin_millisecond = strToTime.mid(9,3).toInt();
        int beginTime = (begin_hour*3600+begin_minute*60+begin_second)*1000+begin_millisecond;

        int end_hour = strToTime.mid(17,2).toInt();
        int end_minute = strToTime.mid(20,2).toInt();
        int end_second = strToTime.mid(23,2).toInt();
        int end_millisecond = strToTime.mid(26,3).toInt();
        int endTime = (end_hour*3600+end_minute*60+end_second)*1000+end_millisecond;

        QString newText = "";
        for(int i=2;i<templist.count();i++)
        {
            newText += templist.at(i);
        }

        subText *newSubText = new subText;
        newSubText->begin = beginTime;
        newSubText->end = endTime;
        newSubText->text = newText;
        list.append(newSubText);

        templist.clear();//这个templist在循环内，没必要clear
        temp.clear();
        //前面都处理好了，并且文件也结束了，那就跳出循环
        if(file.atEnd()) break;
    }
    for(int i=0;i<list.length();i++)
    {
        subText *temp = list.at(i);
        qDebug()<<"读取字幕内容:";
        qDebug()<<temp->begin<<":"<<temp->end;
        qDebug()<<temp->text;
    }
    return list;
}
QString FileOP::GetTextString(QList<subText*> &textList,int timeMile)
{
    QString text = "";
    if(!textList.empty())
    {
        for(int i=0;i<textList.count();i++)
        {
            subText *temp=textList.at(i);
            if(temp->begin<timeMile && timeMile<temp->end)
            {
                text=temp->text;
            }
        }
    }
    return text;
    //如果textList是空的，那么返回的是空字符串
}

QStringList FileOP::toTextList(QList<subText*> &list)
{
    QStringList strText;
    int cnt = 0;//表示现在strText的个数
    for(int i=0;i<list.count();i++)
    {
        int head = 25*list.at(i)->begin/1000;
        int tail = 25*list.at(i)->end/1000;
        QString text = list.at(i)->text;
        for(int j=cnt;j<head;j++)
        {
            strText.append("");
            cnt++;
        }
        for(int j=head;j<tail;j++)
        {
            strText.append(text);
            cnt++;
        }
    }
    qDebug()<<"字幕帧数:"<<strText.count();
    return strText;
}
