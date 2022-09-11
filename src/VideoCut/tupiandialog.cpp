#include "tupiandialog.h"
#include "ui_tupiandialog.h"

tupianDialog::tupianDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::tupianDialog)
{
    ui->setupUi(this);
}

tupianDialog::~tupianDialog()
{
    delete ui;
}

void tupianDialog::on_pushButton_clicked()
{
    accept();
    this->close();
}


void tupianDialog::on_pushButton_2_clicked()
{
    this->close();
}
int tupianDialog::getx()
{
    int x=0;
    if(ui->xlineEdit->text()!="")
    {
        x = ui->xlineEdit->text().toInt();
    }
    return x;
}
int tupianDialog::gety()
{
    int y=0;
    if(ui->ylineEdit->text()!="")
    {
        y = ui->ylineEdit->text().toInt();
    }
    return y;
}
int tupianDialog::gett()
{
    int t=1;
    if(ui->tlineEdit->text()!="")
    {
        t = ui->tlineEdit->text().toInt();
    }
    return t;
}
QString tupianDialog::getEffect()
{
    return ui->comboBox->currentText();
}

