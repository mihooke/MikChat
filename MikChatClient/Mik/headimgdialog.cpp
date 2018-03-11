#include "headimgdialog.h"
#include "ui_headimgdialog.h"

HeadImgDialog::HeadImgDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HeadImgDialog)
{
    ui->setupUi(this);
}

HeadImgDialog::~HeadImgDialog()
{
    delete ui;
}

void HeadImgDialog::on_ok_pushButton_clicked()
{
    this->accept();
}

void HeadImgDialog::on_cancel_pushButton_clicked()
{
    this->reject();
}
