#include "infodialog.h"
#include "ui_infodialog.h"

InfoDialog::InfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InfoDialog)
{
    ui->setupUi(this);
}

InfoDialog::~InfoDialog()
{
    delete ui;
}

void InfoDialog::on_forwardBtn_clicked()
{
    int t = 1;//表示要移动的距离
    QString s = ui->forLineEdit->text();
    if(!s.isEmpty())
    {
        t = s.toInt();
    }
    ui->forLineEdit->clear();
    emit moveSignal(0,t);
}


void InfoDialog::on_backwardBtn_clicked()
{
    int t = 1;//表示要移动的距离
    QString s = ui->backLineEdit->text();
    if(!s.isEmpty())
    {
        t = s.toInt();
    }
    qDebug() << "t:"<<t;
    ui->backLineEdit->clear();
    emit moveSignal(1,t);
}

void InfoDialog::setInfo(const QString &videoPath,int begin,int end)
{
    ui->pathLineEdit->setText(videoPath);
    ui->beginLineEdit->setText(QString::number(begin));
    ui->endLineEdit->setText(QString::number(end));
}


void InfoDialog::on_cutBtn_clicked()
{
    QString text1 = ui->lineEdit_1->text();
    QString text2 = ui->lineEdit_2->text();
    if(text1 != "" && text2 != "")
    {
        t1 = text1.toInt();
        t2 = text2.toInt();
        if(ui->beginLineEdit->text()!="" && ui->endLineEdit->text()!="")
        {
            int head = ui->beginLineEdit->text().toInt();
            int tail = ui->endLineEdit->text().toInt();
            if(t1>=head && t2<=tail)
            {
                if(ui->namelineEdit->text()!="")
                {
                    name = ui->namelineEdit->text();
                    ui->namelineEdit->clear();
                    accept();
                    this->close();
                }
            }
        }
    }
}

