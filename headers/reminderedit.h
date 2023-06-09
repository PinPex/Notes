#ifndef REMINDEREDIT_H
#define REMINDEREDIT_H

#include <QWidget>
#include <QDateTime>


namespace Ui {
class ReminderEdit;
}

class ReminderEdit : public QWidget
{
    Q_OBJECT

public:
    explicit ReminderEdit(QWidget *parent = 0);
    ~ReminderEdit();
signals:
    void sendData(QDateTime time);
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::ReminderEdit *ui;
};

#endif // REMINDEREDIT_H
