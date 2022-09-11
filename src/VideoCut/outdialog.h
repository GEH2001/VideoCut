#ifndef OUTDIALOG_H
#define OUTDIALOG_H

#include <QDialog>

namespace Ui {
class outDialog;
}

class outDialog : public QDialog
{
    Q_OBJECT

public:
    explicit outDialog(QWidget *parent = nullptr);
    ~outDialog();
    QString getRate();
    QString getScale();
    QString getFormat();
private slots:
    void on_enterBtn_clicked();

    void on_quitBox_clicked();

private:
    Ui::outDialog *ui;
};

#endif // OUTDIALOG_H
