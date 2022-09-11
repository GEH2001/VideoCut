#ifndef INFODIALOG_H
#define INFODIALOG_H

#include <QDialog>

namespace Ui {
class InfoDialog;
}

class InfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InfoDialog(QWidget *parent = nullptr);
    ~InfoDialog();

    void setInfo(const QString &videoPath,int begin,int end);
    int t1=0;
    int t2=0;
    QString name = "";
private:
    Ui::InfoDialog *ui;

private slots:
    void on_forwardBtn_clicked();
    void on_backwardBtn_clicked();
    void on_cutBtn_clicked();

signals:
    void moveSignal(bool direct,int dis);//direct=0表示向左 dis为移动的距离

};

#endif // INFODIALOG_H
