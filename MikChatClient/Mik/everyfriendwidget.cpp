#include <QToolTip>
#include <QCursor>
#include "everyfriendwidget.h"
#include "ui_everyfriendwidget.h"

EveryFriendWidget::EveryFriendWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EveryFriendWidget)
{
    ui->setupUi(this);
    setMouseTracking(true);
}

EveryFriendWidget::~EveryFriendWidget()
{
    delete ui;
}

void EveryFriendWidget::setData(const QString userName, const QString userRemark, const QString userNickName)
{
    m_userName = userName;
    m_userRemark = userRemark;
    m_userNickName = userNickName;
    ui->nickNameLabel->setText(userName);
    ui->remarkLlabel->setText(userRemark);
}

void EveryFriendWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint point = QCursor::pos();
    QToolTip::showText(point, m_userName);
}

const QString EveryFriendWidget::getUserID() const
{
    return m_userID;
}

const QString EveryFriendWidget::getUserName() const
{
    return m_userName;
}

const QString EveryFriendWidget::getUserRemark() const
{
    return m_userRemark;
}

const QString EveryFriendWidget::getUserNickName() const
{
    return m_userNickName;
}
