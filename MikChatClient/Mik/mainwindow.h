#ifndef MIK_MAINWINDOW_H
#define MIK_MAINWINDOW_H

#include <QDialog>
#include <QString>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDataStream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMainWindow>
#include <QListWidgetItem>
#include <QSharedPointer>

#include "logindialog.h"
#include "common.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void showWindow(QJsonObject &jo);
    bool checkConnectToServer(const QString &ip, const uint port);

    void recvTransmitChatMsg(QJsonObject &jo);

private slots:
    void on_headImgPushButton_clicked();
    void on_settingsPushButton_clicked();
    void on_loginToMikServer(const QByteArray &msgSend);
    void on_registerToMikServer(const QByteArray &msgSend);
    void on_receiveMsgFromServer();
    void on_modifyNetworkAgent(const QString &agentIP, const uint agentPort);

    void on_friendListWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_sendChatMessage(const QString &userName, const QString &chatMsg);
    void updatePersonalInfosDisplay();

    void on_socketError(QAbstractSocket::SocketError socketError);
    void on_reconnect();

signals:
    void recvLoginVerify(QJsonObject &jo);
    void recvRegister(QJsonObject &jo);
    void recvTransmitChatMsgOK(const QString &msg);
private:
    Ui::MainWindow *ui;

    QScopedPointer<LoginDialog> m_loginDialog;
    QTcpSocket *m_socket = nullptr;
    /// 这个状态变量用好友id保存好友列表，需要一个数据结构来保存所有好友的个人信息（包括账号，昵称，头像，生日，邮箱，上次登陆时间）
    /// 在软件初次加载账号的时候，这些信息从服务器加载，序列化到本地
    UserInformations m_userInfos;
};

#endif // MIK_MAINWINDOW_H
