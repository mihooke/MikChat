﻿#ifndef MIK_SETTINGSDIALOG_H
#define MIK_SETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

private slots:
    void on_ok_pushButton_clicked();

    void on_cancel_pushButton_clicked();

private:
    Ui::SettingsDialog *ui;
};

#endif // MIK_SETTINGSDIALOG_H
