#ifndef TUPIANDIALOG_H
#define TUPIANDIALOG_H

#include <QDialog>

namespace Ui {
class tupianDialog;
}

class tupianDialog : public QDialog
{
    Q_OBJECT

public:
    explicit tupianDialog(QWidget *parent = nullptr);
    ~tupianDialog();

    int getx();
    int gety();
    int gett();
    QString getEffect();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::tupianDialog *ui;
};

#endif // TUPIANDIALOG_H
