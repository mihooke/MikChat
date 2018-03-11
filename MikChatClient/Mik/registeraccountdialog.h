#ifndef MIK_REGISTERACCOUNTDIALOG_H
#define MIK_REGISTERACCOUNTDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class RegisterAccountDialog;
}

class RegisterAccountDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterAccountDialog(QWidget *parent = 0);
    ~RegisterAccountDialog();

    const QString getAccountNum() const {return m_accountNum;}
    const QString getAccountNickname() const {return m_accountNickname;}
    const QString getAccountPassword() const {return m_accountPassword;}

private slots:
    void on_registerPushButton_clicked();

    void on_cancelPushButton_clicked();

private:
    Ui::RegisterAccountDialog *ui;

    QString m_accountNum;
    QString m_accountNickname;
    QString m_accountPassword;
};

#endif // MIK_REGISTERACCOUNTDIALOG_H
