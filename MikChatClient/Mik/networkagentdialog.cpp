#include <QHostAddress>
#include <QMessageBox>
#include "networkagentdialog.h"
#include "ui_networkagentdialog.h"

NetworkAgentDialog::NetworkAgentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NetworkAgentDialog),
    m_serverIp("124.207.160.106"),
    m_serverPort(9527)
{
    ui->setupUi(this);
    ui->ip_lineEdit->setText(m_serverIp);
    ui->port_lineEdit->setText(QString("%1").arg(9527));
}

NetworkAgentDialog::~NetworkAgentDialog()
{
    delete ui;
}

void NetworkAgentDialog::on_ip_lineEdit_textChanged(const QString &ip)
{
    QHostAddress ipAddress;
    if (ipAddress.setAddress(ip))
    {
        m_serverIp = ip;
    }
    else
    {
        QMessageBox::information(this, "IP设置错误", "IP地址格式错误");
        ui->ip_lineEdit->setText(m_serverIp);
    }
}

void NetworkAgentDialog::on_port_lineEdit_textChanged(const QString &port)
{
    uint p = port.toUInt();
    if (p > 1000 && p < 60000)
    {
        m_serverPort = p;
    }
    else
    {
        QMessageBox::information(this, tr("Port设置错误"), tr("Port必须位于1000~60000之间"));
        ui->port_lineEdit->setText(QString("%1").arg(9527));
    }
}

void NetworkAgentDialog::on_ok_pushButton_clicked()
{
    this->accept();
}

void NetworkAgentDialog::on_cancel_pushButton_clicked()
{
    this->reject();
}
