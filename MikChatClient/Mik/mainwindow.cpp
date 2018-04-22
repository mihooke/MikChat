#include <QMessageBox>
#include <QTimer>
#include <QtConcurrent/QtConcurrent>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "personalinfosdialog.h"
#include "settingsdialog.h"
#include "everyfriendwidget.h"
#include "chatdialog.h"
#include "utils/util.h"
#include "utils/defines.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_socket(new QTcpSocket(this))
{
    m_loginDialog.reset(new LoginDialog());
    m_loginDialog->show();
    connect(m_loginDialog.data(), &LoginDialog::loginSuccessfully, this, &MainWindow::showWindow);
    ui->setupUi(this);

    connect(m_loginDialog.data(), &LoginDialog::loginToServer, this, &MainWindow::on_loginToMikServer);
    connect(m_loginDialog.data(), &LoginDialog::registerToServer, this, &MainWindow::on_registerToMikServer);
    connect(m_loginDialog.data(), &LoginDialog::modifyNetworkAgent, this, &MainWindow::on_modifyNetworkAgent);

    connect(m_socket, &QTcpSocket::readyRead, this, &MainWindow::on_receiveMsgFromServer);
    connect(m_socket, static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error),
            this, &MainWindow::on_socketError);

    connect(this, &MainWindow::recvLoginVerify, m_loginDialog.data(), &LoginDialog::on_recvLoginVerify);
    connect(this, &MainWindow::recvRegister, m_loginDialog.data(), &LoginDialog::on_recvRegister);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::checkConnectToServer(const QString &ip, const uint port)
{
    if (!m_socket->isValid())
    {
        m_socket->connectToHost(ip, port);
        m_socket->waitForConnected();
        if (!m_socket->isValid())
        {
            return false;
        }
    }
    return true;
}

void MainWindow::showWindow(QJsonObject &jo)
{
    m_loginDialog->close();
    if (jo.size() > 0)
    {
        m_userInfos.u_user_sex = jo[key_u_user_sex].toInt();
        m_userInfos.u_user_id = jo[key_u_user_id].toInt();
        m_userInfos.u_user_facetype = jo[key_u_user_facetype].toInt();
        m_userInfos.u_user_name = jo[key_u_user_name].toString();
        m_userInfos.u_user_nickname = jo[key_u_user_nickname].toString();
        m_userInfos.u_user_password = jo[key_u_user_password].toString();
        m_userInfos.u_user_customface = jo[key_u_user_customface].toString();
        m_userInfos.u_user_birthday = jo[key_u_user_birthday].toString();
        m_userInfos.u_user_email = jo[key_u_user_email].toString();
        m_userInfos.u_user_remark = jo[key_u_user_remark].toString();
        m_userInfos.u_user_last_login_time = jo[key_u_user_last_login_time].toString();
        QJsonArray ja = jo[key_friends].toArray();
        for (int i = 0; i < ja.size(); ++i)
        {
            m_userInfos.f_friendship.insert(ja[i].toObject()[key_friend].toString());
        }
    }

    /// 当前登陆账户信息显示到主界面上。
    QString nameShow = m_userInfos.u_user_name + QStringLiteral("(") + m_userInfos.u_user_nickname + QStringLiteral(")");
    ui->nickNameLabel->setText(nameShow);
    ui->remarkLineEdit->setText(m_userInfos.u_user_remark);
    /// 先显示自己
    EveryFriendWidget *selfWidget = new EveryFriendWidget();
    selfWidget->setData(m_userInfos.u_user_name, m_userInfos.u_user_remark, m_userInfos.u_user_nickname);
    QListWidgetItem *item = new QListWidgetItem(ui->friendListWidget);
    item->setSizeHint(QSize(0, selfWidget->height()));
    ui->friendListWidget->setItemWidget(item, selfWidget);
    for (const QString &name : m_userInfos.f_friendship)
    {
        EveryFriendWidget *friendWidget = new EveryFriendWidget();
        /// TODO 这里需要改成好友的remark和好友的昵称
        friendWidget->setData(name, m_userInfos.u_user_remark, m_userInfos.u_user_nickname);
        QListWidgetItem *item = new QListWidgetItem(ui->friendListWidget);
        item->setSizeHint(QSize(0, friendWidget->height()));
        ui->friendListWidget->setItemWidget(item, friendWidget);
        //ui->friendListWidget->setStyleSheet("#friendListWidget::item{height:50px}"
        //                                    "#friendListWidget::item:hover{background-color:rgb(100, 100, 100)}");
    }
    this->show();
}

void MainWindow::on_headImgPushButton_clicked()
{
    PersonalInfosDialog personalInfos(&m_userInfos);
    if (personalInfos.exec() == QDialog::Accepted)
    {
        updatePersonalInfosDisplay();
    }
}

void MainWindow::on_settingsPushButton_clicked()
{
    SettingsDialog settings;
    if (settings.exec() == QDialog::Accepted)
    {
        ;
    }
}

void MainWindow::on_loginToMikServer(const QByteArray &msgSend)
{
    if (!checkConnectToServer(temp_ip, temp_port))
    {
        QMessageBox::critical(this, QStringLiteral("失败"), QStringLiteral("无法连接服务器！"));
        return;
    }
    m_socket->write(msgSend);
}

void MainWindow::on_registerToMikServer(const QByteArray &msgSend)
{
    if (!checkConnectToServer(temp_ip, temp_port))
    {
        QMessageBox::critical(this, QStringLiteral("失败"), QStringLiteral("无法连接服务器！"));
        return;
    }
    m_socket->write(msgSend);
}

void MainWindow::on_receiveMsgFromServer()
{
    QByteArray cmdByte = m_socket->read(4);
    QDataStream stream(&cmdByte, QIODevice::ReadOnly);
    QJsonObject responseOfLoginJo = QJsonDocument::fromJson(m_socket->readAll()).object();
    int cmdType;
    stream >> cmdType;
    switch (cmdType) {
    case LOGINVERIFYACCOUNT:
        emit recvLoginVerify(responseOfLoginJo);
        break;
    case ACCOUNTREGISTER:
        emit recvRegister(responseOfLoginJo);
        break;
    case TRANSMITCHATMSG:
        recvTransmitChatMsg(responseOfLoginJo);
        break;
    case NOTIFUFRIENDSONLINE:
        break;
    }
}

void MainWindow::on_modifyNetworkAgent(const QString &agentIP, const uint agentPort)
{
    if (!checkConnectToServer(agentIP, agentPort))
    {
        return;
    }
}

void MainWindow::on_friendListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    if (!item)
    {
        return;
    }
    EveryFriendWidget *widget = static_cast<EveryFriendWidget*>(ui->friendListWidget->itemWidget(item));
    ChatDialog *chat = new ChatDialog(widget->getUserName(), this);
    connect(chat, &ChatDialog::sendChatMessage, this, &MainWindow::on_sendChatMessage);
    connect(this, &MainWindow::recvTransmitChatMsgOK, chat, &ChatDialog::on_recvTransmitChatMsgOK);
    QString titleName = QStringLiteral("与") + widget->getUserName() + QStringLiteral("聊天");
    chat->setWindowTitle(titleName);
    chat->show();
}

void MainWindow::on_sendChatMessage(const QString &userName, const QString &chatMsg)
{
    QJsonObject jo;
    jo[key_send_user_name] = m_userInfos.u_user_name;
    jo[key_recv_user_name] = userName;
    jo[key_chat_message] = chatMsg;
    QByteArray msgSend = toStream(CHATMESSAGE, jo);
    m_socket->write(msgSend);
}

void MainWindow::recvTransmitChatMsg(QJsonObject &jo)
{
    QString msg = jo["chat_message"].toString();
    qDebug() << msg;
    emit recvTransmitChatMsgOK(msg);
}

void MainWindow::updatePersonalInfosDisplay()
{
    QString nameShow = m_userInfos.u_user_name + QStringLiteral("(") + m_userInfos.u_user_nickname + QStringLiteral(")");
    ui->nickNameLabel->setText(nameShow);
    ui->remarkLineEdit->setText(m_userInfos.u_user_remark);
    /// 更新服务器上的信息
    QJsonObject jo;
    jo[key_u_user_sex] = m_userInfos.u_user_sex;
    jo[key_u_user_id] = m_userInfos.u_user_id;
    jo[key_u_user_facetype] = m_userInfos.u_user_facetype;
    jo[key_u_user_name] = m_userInfos.u_user_name;
    jo[key_u_user_nickname] = m_userInfos.u_user_nickname;
    jo[key_u_user_password] = m_userInfos.u_user_password;
    jo[key_u_user_customface] = m_userInfos.u_user_customface;
    jo[key_u_user_birthday] = m_userInfos.u_user_birthday;
    jo[key_u_user_email] = m_userInfos.u_user_email;
    jo[key_u_user_remark] = m_userInfos.u_user_remark;
    jo[key_u_user_last_login_time] = m_userInfos.u_user_last_login_time;
    qDebug() << jo;
    QByteArray msgSend = toStream(UPDATEPERSONALINFOS, jo);
    m_socket->write(msgSend);
}

void MainWindow::on_socketError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);
    qDebug() << m_socket->errorString();

    /// TODO 重连机制此做法欠妥，会造成界面卡顿，需重新实现，一个思路是：把socket放到一个thread中去，在mainwindow中开启关闭此thread
    QtConcurrent::run(this, &MainWindow::startReconnectTimer);
}

void MainWindow::on_reconnect()
{
    m_socket->disconnectFromHost();
    m_socket->abort();
    m_socket->close();
    m_socket->deleteLater();

    m_socket = new QTcpSocket(this);
    connect(m_socket, &QTcpSocket::readyRead, this, &MainWindow::on_receiveMsgFromServer);
    connect(m_socket, static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error),
            this, &MainWindow::on_socketError);
    m_socket->connectToHost(temp_ip, temp_port);
    m_socket->waitForConnected();
}

void MainWindow::startReconnectTimer()
{
    QTimer::singleShot(2000, this, &MainWindow::on_reconnect);
}
