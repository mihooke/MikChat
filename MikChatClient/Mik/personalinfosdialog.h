#ifndef PERSONALINFOSDIALOG_H
#define PERSONALINFOSDIALOG_H

#include <QDialog>
#include "common.h"

namespace Ui {
class PersonalInfosDialog;
}

class PersonalInfosDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PersonalInfosDialog(UserInformations *infos, QWidget *parent = 0);
    ~PersonalInfosDialog();

    UserInformations *getUserInfos() const { return m_infos; }

private slots:
    void on_ok_pushButton_clicked();

    void on_cancel_pushButton_clicked();

    void on_switchHeadPushButton_clicked();

private:
    Ui::PersonalInfosDialog *ui;
    UserInformations *m_infos;
};

#endif // PERSONALINFOSDIALOG_H
