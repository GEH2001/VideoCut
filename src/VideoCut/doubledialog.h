#ifndef DOUBLEDIALOG_H
#define DOUBLEDIALOG_H

#include <QDialog>

namespace Ui {
class doubleDialog;
}

class doubleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit doubleDialog(QWidget *parent = nullptr);
    ~doubleDialog();
    QString getText();
    void setxy(int x,int y);
    int getTime();
    int getx();
    int gety();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::doubleDialog *ui;
};

#endif // DOUBLEDIALOG_H
