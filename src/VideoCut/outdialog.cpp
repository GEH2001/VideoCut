#include "outdialog.h"
#include "ui_outdialog.h"

outDialog::outDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::outDialog)
{
    ui->setupUi(this);
}

outDialog::~outDialog()
{
    delete ui;
}
QString outDialog::getRate()
{
    return ui->rateBox->currentText();
}
QString outDialog::getScale()
{
    return ui->scaleBox->currentText();
}
QString outDialog::getFormat()
{
    return ui->formatBox->currentText();
}
void outDialog::on_enterBtn_clicked()
{
    accept();
    this->close();
}

void outDialog::on_quitBox_clicked()
{
    this->close();
}
