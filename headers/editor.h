#ifndef EDITOR_H
#define EDITOR_H

#include <QWidget>
#include <QTextDocument>
#include <QPushButton>
#include <QDateTime>
#include "database.h"
#include <QCheckBox>
#include <QLineEdit>
#include <QTextBlock>
#include <QFileDialog>
#include <QImage>
#include <QTableWidgetItem>

#include "notesinform.h"



namespace Ui {
class Editor;
}

class Editor : public QWidget
{
    Q_OBJECT

public:
    explicit Editor(QWidget *parent = 0);
    ~Editor();
    NotesInform inf;
    void setNew(bool boolNew);
    void setInformation(NotesInform inform);
    bool JustClose = true;
    bool Close = false;
    void ShowEdit();
    void HideEdit();
    void RememberMessage();
    void setNoteId(int NoteId);
private slots:
    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_comboBox_2_activated(int index);
    void DateReceived(QDateTime date);
    void NoteIdReceived(int NoteId);
    void Edit_button_clicked();
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_fontComboBox_currentFontChanged(const QFont &f);

    void on_pushButton_4_clicked();

    void on_comboBox_activated(const QString &arg1);

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_9_clicked();

private:
    Ui::Editor *ui;

    QDateTime datetime;
    QString today;
    QString tomorrow;
    QString next_week;
    QPushButton* push;
    QLineEdit* remember;
    QSize s;

    bool New = false;
    bool Showed = false;
    void fillInf();
    void closeEvent(QCloseEvent *event);
    void resizeEvent(QResizeEvent* event);

signals:
    void NewNote(NotesInform &inform);
    void EditedNote(NotesInform inf);
    void CloseNote(int NoteId);
    void DeleteNote(int NoteId);
};

#endif // EDITOR_H
