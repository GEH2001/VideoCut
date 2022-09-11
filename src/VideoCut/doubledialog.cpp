#include "doubledialog.h"
#include "ui_doubledialog.h"

doubleDialog::doubleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::doubleDialog)
{
    ui->setupUi(this);
}

doubleDialog::~doubleDialog()
{
    delete ui;
}

void doubleDialog::on_pushButton_clicked()
{
    accept();
    this->close();
}


void doubleDialog::on_pushButton_2_clicked()
{
    this->close();
}
QString doubleDialog::getText()
{
    return ui->lineEdit->text();
}
void doubleDialog::setxy(int x,int y)
{
    ui->xlineEdit->setText(QString::number(x));
    ui->ylineEdit->setText(QString::number(y));
}
int doubleDialog::getTime()
{
    int time = 1;
    QString s = ui->tlineEdit->text();
    if(s != "")
    {
        time = s.toInt();
    }
    return time;
}
int doubleDialog::getx()
{
    int x = 0;
    if(ui->xlineEdit->text()!="")
        x=ui->xlineEdit->text().toInt();
    return x;
}

int doubleDialog::gety()
{
    int y = 0;
    if(ui->ylineEdit->text()!="")
        y=ui->ylineEdit->text().toInt();
    return y;
}
