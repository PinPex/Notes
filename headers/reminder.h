#ifndef REMINDER_H
#define REMINDER_H

#include <QWidget>

namespace Ui {
class Reminder;
}

class Reminder : public QWidget
{
    Q_OBJECT

public:
    explicit Reminder(QWidget *parent = 0);
    ~Reminder();



private:
    Ui::Reminder *ui;
};

#endif // REMINDER_H
