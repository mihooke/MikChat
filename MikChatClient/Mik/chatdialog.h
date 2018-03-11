#ifndef MIK_CHATDIALOG_H
#define MIK_CHATDIALOG_H

#include <QDialog>

namespace Ui {
class ChatDialog;
}

class ChatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChatDialog(QString userName, QWidget *parent = 0);
    ~ChatDialog();

    const QString getUserName() const { return m_userName; }
signals:
    void sendChatMessage(const QString &userName, const QString &chatMsg);
public slots:
    void on_recvTransmitChatMsgOK(const QString &msg);
private slots:
    void on_sendPushButton_clicked();

private:
    Ui::ChatDialog *ui;
    QString m_userName;
};

#endif // MIK_CHATDIALOG_H
