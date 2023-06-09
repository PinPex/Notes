#include "reminderedit.h"
#include "ui_reminderedit.h"
#include <QDebug>
#include <QDateTime>
#include <QDir>

ReminderEdit::ReminderEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReminderEdit)
{

    ui->setupUi(this);
    this->setWindowTitle("Напоминание");
    QDir dir;
    QString path = dir.currentPath();
    QPixmap pixmap(path + "/debug/notes.png");
    QIcon icon(pixmap);
    this->setStyleSheet("#pushButton{background-color: #a3d9f7; font-size: 16px;}"
                        "#pushButton_2{background-color: #a3d9f7; font-size: 16px;}");
    this->setWindowIcon(icon);
}

ReminderEdit::~ReminderEdit()
{
    delete ui;
}

void ReminderEdit::on_pushButton_clicked()
{
    QDateTime time;
    time.setDate(ui->calendarWidget->selectedDate());
    time.setTime(ui->timeEdit->time());
    emit sendData(time);
    this->close();
}

void ReminderEdit::on_pushButton_2_clicked()
{
    this->close();
}
