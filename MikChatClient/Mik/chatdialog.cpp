#include <QTableWidgetItem>
#include <QHeaderView>
#include "chatdialog.h"
#include "ui_chatdialog.h"
#include "widgets/chattablesubwidget.h"

ChatDialog::ChatDialog(QString userName, QWidget *parent) :
    QDialog(parent), m_userName(userName),
    ui(new Ui::ChatDialog)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setColumnWidth(0, 30);
    ui->tableWidget->setColumnWidth(1, 300);
    ui->tableWidget->setColumnWidth(2, 30);
//    ui->tableWidget->resizeColumnsToContents();
//    ui->tableWidget->resizeRowsToContents();
    ui->tableWidget->horizontalHeader()->setVisible(false);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem(QIcon("./picture/bitoff.png"), "bitoff"));
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem(QStringLiteral("一段内容把阿萨达萨达打撒萨达萨达萨达啊是大大大大一段内容把阿萨达萨达打撒萨达萨达萨达啊是大大大大")));
    ChatTableSubWidget *widget = new ChatTableSubWidget(this);
    widget->setData("zdd", QPixmap("./picture/biton.png"));
    ui->tableWidget->setCellWidget(0, 2, widget);
}

ChatDialog::~ChatDialog()
{
    delete ui;
}

void ChatDialog::on_sendPushButton_clicked()
{
    emit sendChatMessage(m_userName, ui->contextTextEdit->toPlainText());
}

void ChatDialog::on_recvTransmitChatMsgOK(const QString &msg)
{
    Q_UNUSED(msg)
    ui->contextTextEdit->setText("");
}
