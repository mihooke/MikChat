#include <QFile>
#include <QMessageBox>
#include <QPixmap>
#include <QDateTime>
#include <QtConcurrent/QtConcurrent>
#include "logindialog.h"
#include "ui_logindialog.h"
#include "networkagentdialog.h"
#include "registeraccountdialog.h"
#include "utils/util.h"
#include "utils/defines.h"
#include <QDebug>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog),
    m_serverIp("192.168.1.111"),
    m_serverPort(9527)
{
    setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    ui->setupUi(this);

    readNativeAccountInfo();

    if (m_nativeAccount[ui->userLoginComboBox->currentText()].isSavePassword)
    {
        ui->userPasswordLineEdit->setText(m_nativeAccount[ui->userLoginComboBox->currentText()].accountPassword);
        ui->rememberPwdCheckBox->setChecked(true);
    }
    else
    {
        ui->userPasswordLineEdit->setText("");
    }

    ui->headImageLabel->setPixmap(QPixmap(m_nativeAccount[ui->userLoginComboBox->currentText()].accountHeadImage));
    ui->nicknameShowLabel->setText(m_nativeAccount[ui->userLoginComboBox->currentText()].accountNickname);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_LoginPushButton_clicked()
{
    /// 检查账户名称是否已注册
    const QString accountNumInUI = ui->userLoginComboBox->currentText();
    if (accountNumInUI.isEmpty())
    {
        QMessageBox::information(this, QStringLiteral("Error"), QStringLiteral("Account num must not be null!"));
        return;
    }

    /// 检查密码
    const QString accountPasswordInUI = ui->userPasswordLineEdit->text();
    if (accountPasswordInUI.isEmpty())
    {
        QMessageBox::information(this, QStringLiteral("Error"), QStringLiteral("Please input the passsword!"));
        return;
    }

    /// 如果是新的账户，把登陆过的账号保存到本地json文件
    if (m_nativeAccount.find(accountNumInUI) == m_nativeAccount.end())
    {
        const bool isCheckedInUI = ui->rememberPwdCheckBox->isChecked();
        ACCOUNTSAVEINFO accountInfo;
        accountInfo.accountNum = accountNumInUI;
        accountInfo.accountNickname = "";
        accountInfo.accountPassword = accountPasswordInUI;
        accountInfo.accountHeadImage = "";
        accountInfo.isSavePassword = isCheckedInUI;
        m_nativeAccount[accountNumInUI] = accountInfo;
        saveAccountToNativeJson();
    }

    /// 连接到服务器
    /// 1.向服务器发送登陆请求，附带信息是登陆账号和密码
    QJsonObject jo;
    jo[key_account_num] = accountNumInUI;
    jo[key_account_password] = accountPasswordInUI;
    QByteArray msgSend = toStream(ACCOUNTLOGIN, jo);
    emit loginToServer(msgSend);
}

void LoginDialog::on_registerPushButton_clicked()
{
    RegisterAccountDialog dialog;
    if (dialog.exec() == QDialog::Accepted)
    {
        /// 向服务器发送注册账号请求
        QJsonObject jo;
        jo[key_account_num] = dialog.getAccountNum();
        jo[key_account_password] = dialog.getAccountPassword();
        jo[key_account_nickname] = dialog.getAccountNickname();
        jo[key_register_time] = QDateTime::currentDateTime().toString(Qt::ISODate);
        QByteArray msgSend = toStream(ACCOUNTREGISTER, jo);
        emit registerToServer(msgSend);
    }
}

void LoginDialog::on_userLoginComboBox_activated(const QString &accountNum)
{
    if (m_nativeAccount[accountNum].isSavePassword)
    {
        ui->userPasswordLineEdit->setText(m_nativeAccount[accountNum].accountPassword);
    }
    else
    {
        ui->userPasswordLineEdit->setText("");
    }
    ui->rememberPwdCheckBox->setChecked(m_nativeAccount[accountNum].isSavePassword);
    ui->headImageLabel->setPixmap(QPixmap(m_nativeAccount[accountNum].accountHeadImage));
    ui->nicknameShowLabel->setText(m_nativeAccount[accountNum].accountNickname);
}

void LoginDialog::on_rememberPwdCheckBox_toggled(bool checked)
{
    const QString userNum = ui->userLoginComboBox->currentText();
    const QString pwd = ui->userPasswordLineEdit->text();
    if (!pwd.isEmpty() && pwd != m_nativeAccount[userNum].accountPassword)
    {
        m_nativeAccount[userNum].accountPassword = pwd;
    }
    m_nativeAccount[userNum].isSavePassword = checked;
    saveAccountToNativeJson();
}

void LoginDialog::on_agent_pushButton_clicked()
{
    NetworkAgentDialog dialog;
    if (dialog.exec() == QDialog::Accepted)
    {
        m_serverIp = dialog.getIp();
        m_serverPort = dialog.getPort();
    }
}

void LoginDialog::readNativeAccountInfo()
{
    QFile file(remPwdFile);
    file.open(QIODevice::ReadOnly);
    QByteArray jsonBytes = file.readAll();
    QJsonArray ja = QJsonDocument::fromJson(jsonBytes).array();
    for (int i = 0; i < ja.size(); ++i)
    {
        QJsonObject jo = ja.at(i).toObject();
        ACCOUNTSAVEINFO accountInfo;
        accountInfo.accountNum = jo[key_account_num].toString();
        accountInfo.accountNickname = jo[key_account_nickname].toString();
        accountInfo.accountPassword = jo[key_account_password].toString();
        accountInfo.accountHeadImage = jo[key_account_headimage].toString();
        accountInfo.isSavePassword = jo[key_issave_password].toBool();
        m_nativeAccount[accountInfo.accountNum] = accountInfo;
        ui->userLoginComboBox->insertItem(i, accountInfo.accountNum);
        ui->userLoginComboBox->setItemIcon(i, QIcon(accountInfo.accountHeadImage));
    }
}

bool LoginDialog::saveAccountToNativeJson()
{
    QJsonArray ja;
    for (auto &item : m_nativeAccount.values())
    {
        QJsonObject jo;
        jo[key_account_num] = item.accountNum;
        jo[key_account_nickname] = item.accountNickname;
        jo[key_account_password] = item.accountPassword;
        jo[key_account_headimage] = item.accountHeadImage;
        jo[key_issave_password] = item.isSavePassword;
        ja.push_back(jo);
    }
    QByteArray byteJa = QJsonDocument(ja).toJson();
    QFile file(remPwdFile);
    file.open(QIODevice::WriteOnly);
    file.write(byteJa, byteJa.length());
    file.close();
    return true;
}

/// TODO，鼠标悬浮事件
void LoginDialog::mouseMoveEvent(QMouseEvent *event)
{
    //ui->userLoginComboBox->childAt(event->x(), event->y())->setMinimumHeight(25);
}

void LoginDialog::on_userLoginComboBox_currentTextChanged(const QString &accountNum)
{
    if (accountNum.isEmpty())
    {
        ui->headImageLabel->setPixmap(QPixmap());
        ui->nicknameShowLabel->setText("");
        ui->userPasswordLineEdit->setText("");
    }
}

void LoginDialog::on_recvLoginVerify(QJsonObject &jo)
{
    if (jo["status"].toString() == "loginok")
    {
        qDebug() << jo["status"].toString();
        qDebug() << jo["user_infos"];
        QMessageBox::information(this, QStringLiteral("ok"), QStringLiteral("登陆账户成功！"));
        emit loginSuccessfully(jo["user_infos"].toObject());
    }
    else if (jo["status"].toString() == "passworderror")
    {
        qDebug() << jo["status"].toString();
    }
    else if (jo["status"].toString() == "accountnotexist")
    {
        qDebug() << jo["status"].toString();
    }
    else if (jo["status"].toString() == "loginserverissue")
    {
        qDebug() << jo["status"].toString();
    }
    else if (jo["status"].toString() == "loginnotdefineerror")
    {
        qDebug() << jo["status"].toString();
    }
}

void LoginDialog::on_recvRegister(QJsonObject &jo)
{
    if (jo["status"].toString() == "registerok")
    {
        qDebug() << jo["status"].toString();
        QMessageBox::information(this, QStringLiteral("ok"), QStringLiteral("注册账户成功！"));
    }
    else if (jo["status"].toString() == "accountalreadyexist")
    {
        qDebug() << jo["status"].toString();
    }
    else if (jo["status"].toString() == "registerserverissue")
    {
        qDebug() << jo["status"].toString();
    }
    else if (jo["status"].toString() == "registernotdefineerror")
    {
        qDebug() << jo["status"].toString();
    }
}
