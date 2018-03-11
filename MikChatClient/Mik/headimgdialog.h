#ifndef MIK_HEADIMGDIALOG_H
#define MIK_HEADIMGDIALOG_H

#include <QDialog>

namespace Ui {
class HeadImgDialog;
}

class HeadImgDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HeadImgDialog(QWidget *parent = 0);
    ~HeadImgDialog();

private slots:
    void on_ok_pushButton_clicked();

    void on_cancel_pushButton_clicked();

private:
    Ui::HeadImgDialog *ui;
};

#endif // MIK_HEADIMGDIALOG_H
