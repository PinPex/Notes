#include "reminder.h"
#include "ui_reminder.h"
#include <QDir>

Reminder::Reminder(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Reminder)
{
    ui->setupUi(this);
    this->setWindowTitle("Напоминание");
    QDir dir;
    QString path = dir.currentPath();
    QPixmap pixmap(path + "/debug/notes.png");
    QIcon icon(pixmap);
    this->setWindowIcon(icon);
}


Reminder::~Reminder()
{
    delete ui;
}
