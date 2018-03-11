#ifndef MIK_NETWORKAGENTDIALOG_H
#define MIK_NETWORKAGENTDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class NetworkAgentDialog;
}

class NetworkAgentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NetworkAgentDialog(QWidget *parent = 0);
    ~NetworkAgentDialog();

    const QString getIp() const {return m_serverIp;}
    const uint getPort() const {return m_serverPort;}

private slots:
    void on_ip_lineEdit_textChanged(const QString &ip);

    void on_port_lineEdit_textChanged(const QString &port);

    void on_ok_pushButton_clicked();

    void on_cancel_pushButton_clicked();

private:
    Ui::NetworkAgentDialog *ui;
    QString m_serverIp;
    uint m_serverPort;
};

#endif // MIK_NETWORKAGENTDIALOG_H
