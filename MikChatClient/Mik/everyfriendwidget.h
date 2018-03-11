#ifndef MIK_EVERYFRIENDWIDGET_H
#define MIK_EVERYFRIENDWIDGET_H

#include <QWidget>
#include <QMouseEvent>

namespace Ui {
class EveryFriendWidget;
}

class EveryFriendWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EveryFriendWidget(QWidget *parent = 0);
    ~EveryFriendWidget();
    void setData(const QString userName, const QString userRemark, const QString userNickName);

    const QString getUserID() const;
    const QString getUserName() const;
    const QString getUserRemark() const;
    const QString getUserNickName() const;
protected:
    void mouseMoveEvent(QMouseEvent *event) override;
private:
    Ui::EveryFriendWidget *ui;
    QString m_userID;
    QString m_userName;
    QString m_userRemark;
    QString m_userNickName;
};

#endif // MIK_EVERYFRIENDWIDGET_H
