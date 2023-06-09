#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtSql/QSqlDatabase>
#include <QMessageBox>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QTimer>

#include "editor.h"
#include "notesinform.h"
#include "database.h"


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_pushButton_2_clicked();
    void NewNote(NotesInform &inf);
    void on_pushButton_clicked();
    void EditedNote(NotesInform inf);
    void CloseNote(int NoteId);
    void DeleteNote(int NoteId);


    void on_lineEdit_textChanged(const QString &arg1);

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_lineEdit_2_textChanged(const QString &arg1);

    void timerEvent();

    void on_pushButton_3_clicked();

    void on_pushButton_7_clicked();

private:
    Ui::Widget *ui;
    Database base;
    QVector<NotesInform> getInformationDB();
    QVector<NotesInform> inf;
    void showNotes(QVector<NotesInform> &inf);
    void deleteAllItems(QTableWidget* wid);
    QVariantList fillVariantList(NotesInform inform);
    QVector<int> opened;
    QVector<Editor *> editors;
    void closeEvent(QCloseEvent *event);
    void WriteInBase();
    int getLastNoteId();
    void WriteCoordinates();
    void OpenEdit(QTableWidget* tab, QString str);
    void DeleteEdit(QTableWidget* tab);
    void ResearchLabel(QLabel* lab, QString arg1);

signals:
    void NoteIdReceive(int NoteId);

};

#endif // WIDGET_H
