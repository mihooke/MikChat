#ifndef CHATTABLESUBWIDGET_H
#define CHATTABLESUBWIDGET_H

#include <QWidget>

namespace Ui {
class ChatTableSubWidget;
}

class ChatTableSubWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChatTableSubWidget(QWidget *parent = 0);
    ~ChatTableSubWidget();

    void setData(const QString &name, const QPixmap &pixmap);

private:
    Ui::ChatTableSubWidget *ui;
};

#endif // CHATTABLESUBWIDGET_H
