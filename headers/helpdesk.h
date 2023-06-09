#ifndef HELPDESK_H
#define HELPDESK_H

#include <QWidget>
#include <QUrl>
#include <QFile>
#include <QDir>

namespace Ui {
class HelpDesk;
}

class HelpDesk : public QWidget
{
    Q_OBJECT

public:
    explicit HelpDesk(QWidget *parent = 0);
    ~HelpDesk();

private:
    Ui::HelpDesk *ui;
    QFile currentFile;
    QVector<QString> files;

private slots:
    void changedUrl(QUrl url);
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_2_clicked();
};

#endif // HELPDESK_H
