#include <QMessageBox>
#include "registeraccountdialog.h"
#include "ui_registeraccountdialog.h"

RegisterAccountDialog::RegisterAccountDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterAccountDialog)
{
    ui->setupUi(this);
}

RegisterAccountDialog::~RegisterAccountDialog()
{
    delete ui;
}

void RegisterAccountDialog::on_registerPushButton_clicked()
{
    /// 检查账户名称
    const QString accountNum = ui->accountNumLineEdit->text();
    if (accountNum.isEmpty())
    {
        QMessageBox::critical(this, tr("Error"), tr("Account num cannot be null!"));
        return;
    }
    m_accountNum = ui->accountNumLineEdit->text();

    /// 检查昵称
    if (ui->accountNicknameLineEdit->text().isEmpty())
    {
        QMessageBox::critical(this, tr("Error"), tr("Nickname cannot be null!"));
        return;
    }
    m_accountNickname = ui->accountNicknameLineEdit->text();

    /// 检查密码
    const QString accountPassword = ui->accountPasswordLineEdit->text();
    if (accountPassword.isEmpty())
    {
        QMessageBox::critical(this, tr("Error"), tr("Password cannot be null!"));
        return;
    }
    if (accountPassword.length() < 8)
    {
        QMessageBox::critical(this, tr("Error"), tr("Password must greater than 8 chars!"));
        return;
    }

    /// 检查确认密码
    if (ui->accountPasswordConfirmLineEdit->text().isEmpty())
    {
        QMessageBox::critical(this, tr("Error"), tr("Confirm password cannot be null!"));
        return;
    }
    if (accountPassword != ui->accountPasswordConfirmLineEdit->text())
    {
        QMessageBox::critical(this, tr("Error"), tr("Password not same as confirm password!"));
        return;
    }
    m_accountPassword = ui->accountPasswordConfirmLineEdit->text();
    this->accept();
}

void RegisterAccountDialog::on_cancelPushButton_clicked()
{
    this->reject();
}
