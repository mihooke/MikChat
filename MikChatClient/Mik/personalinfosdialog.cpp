#include <QDate>
#include <QDebug>
#include "personalinfosdialog.h"
#include "ui_personalinfosdialog.h"
#include "headimgdialog.h"

PersonalInfosDialog::PersonalInfosDialog(UserInformations *infos, QWidget *parent) :
    QDialog(parent), m_infos(infos),
    ui(new Ui::PersonalInfosDialog)
{
    ui->setupUi(this);
    ui->accountLineEdit->setText(infos->u_user_name);
    ui->nickNameLineEdit->setText(infos->u_user_nickname);
    ui->remarkLineEdit->setText(infos->u_user_remark);
    ui->mailLineEdit->setText(infos->u_user_email);
    ui->lastLoginTimeLineEdit->setText(infos->u_user_last_login_time);
    ui->birthdayDateEdit->setDate(QDate::fromString(infos->u_user_birthday, "yyyy-MM-dd"));
    ui->sexManRadioButton->setChecked(infos->u_user_sex == 1 ? true : false);
    ui->sexWomanRadioButton->setChecked(infos->u_user_sex == 2 ? true : false);
}

PersonalInfosDialog::~PersonalInfosDialog()
{
    delete ui;
}

void PersonalInfosDialog::on_ok_pushButton_clicked()
{
    m_infos->u_user_nickname = ui->nickNameLineEdit->text().trimmed();
    m_infos->u_user_remark = ui->remarkLineEdit->text().trimmed();
    m_infos->u_user_email = ui->mailLineEdit->text().trimmed();
    m_infos->u_user_birthday = ui->birthdayDateEdit->date().toString("yyyy-MM-dd");
    m_infos->u_user_sex = ui->sexManRadioButton->isChecked() ? 1 : 2;

    this->accept();
}

void PersonalInfosDialog::on_cancel_pushButton_clicked()
{
    this->reject();
}

void PersonalInfosDialog::on_switchHeadPushButton_clicked()
{
    HeadImgDialog headImg;
    if (headImg.exec() == QDialog::Accepted)
    {
        ;
    }
}
