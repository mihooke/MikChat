#include "chattablesubwidget.h"
#include "ui_chattablesubwidget.h"

ChatTableSubWidget::ChatTableSubWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatTableSubWidget)
{
    ui->setupUi(this);
}

ChatTableSubWidget::~ChatTableSubWidget()
{
    delete ui;
}

void ChatTableSubWidget::setData(const QString &name, const QPixmap &pixmap)
{
    ui->headImgLabel->setPixmap(pixmap);
    ui->nameLabel->setText(name);
}
