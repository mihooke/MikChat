#ifndef MIK_LOGINDIALOG_H
#define MIK_LOGINDIALOG_H

#include <QDialog>
#include <QString>
#include <QHash>
#include <QMouseEvent>
#include <QDataStream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

typedef struct _tag_account_save_info
{
    QString accountNum;
    QString accountNickname;
    QString accountPassword;
    QString accountHeadImage;
    bool isSavePassword;/// 是否保存密码
} ACCOUNTSAVEINFO;

enum CmdType
{
    ACCOUNTREGISTER = 1,
    ACCOUNTLOGIN    = 2,
    LOGINVERIFYACCOUNT = 3,
    CHATMESSAGE     = 4,
    TRANSMITCHATMSG = 5,
    NOTIFUFRIENDSONLINE = 6,
    UPDATEPERSONALINFOS = 7,
};

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

    //QByteArray toStream(int cmdType, QJsonObject &jo);

private slots:
    void on_LoginPushButton_clicked();
    void on_registerPushButton_clicked();
    void on_userLoginComboBox_activated(const QString &accountNum);
    void on_rememberPwdCheckBox_toggled(bool checked);
    void on_agent_pushButton_clicked();

    void on_userLoginComboBox_currentTextChanged(const QString &accountNum);
public slots:
    void on_recvLoginVerify(QJsonObject &jo);
    void on_recvRegister(QJsonObject &jo);
signals:
    void loginSuccessfully(QJsonObject &jo);
    void loginToServer(QByteArray msgSend);
    void registerToServer(QByteArray msgSend);
    void modifyNetworkAgent(const QString agentIP, const uint agentPort);
protected:
    void mouseMoveEvent(QMouseEvent *event);
private:
    /// 从本地配置文件中读取所有保存的账户信息到内存
    void readNativeAccountInfo();
    /// 保存账户信息到本地json文件
    bool saveAccountToNativeJson();
private:
    Ui::LoginDialog *ui;

    QString m_serverIp;
    uint m_serverPort;

    /// 本地所有账户
    QHash<QString ,ACCOUNTSAVEINFO> m_nativeAccount;
};

#endif // MIK_LOGINDIALOG_H
