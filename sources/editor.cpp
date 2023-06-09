#include "editor.h"
#include "ui_editor.h"
#include "reminderedit.h"
#include <QMessageBox>
#include <QDebug>
#include <QTextDocument>
#include <QColorDialog>
#include <QDateTime>
#include <QMenuBar>
#include <QImageReader>



Editor::Editor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Editor)
{
    ui->setupUi(this);
    for(int i = 8; i < 32; i += 2){
        ui->comboBox->addItem(QString::number(i));
    }

    ui->comboBox->setCurrentIndex(6);
    ui->plainTextEdit->setFont(QFont("Times new roman", 12));

    ui->lineEdit->setFont(QFont("Times new roman", 12));

    ui->fontComboBox->setCurrentIndex(306);

    datetime = QDateTime::currentDateTime();
    QTime time = datetime.time();
    time.setHMS(datetime.time().hour(), datetime.time().minute(), 0);
    datetime.setTime(time);
    today = datetime.addSecs(10800).toString();
    datetime.addDays(1).setTime(QTime(9,0));
    tomorrow = datetime.toString();
    next_week = datetime.addDays(6).toString();
    ui->comboBox_2->addItem("Нет напоминания");
    ui->comboBox_2->addItem("Сегодня( " + today + " )");
    ui->comboBox_2->addItem("Завтра( " + tomorrow + " )");
    ui->comboBox_2->addItem("На следующей неделе( " + next_week + " )");
    ui->comboBox_2->addItem("Выбрать дату и время");
    ui->comboBox_2->setCurrentIndex(0);

    QDir dir;
    QString path = dir.currentPath();
    QPixmap pixmap(path + "/debug/notes.png");
    QIcon icon(pixmap);
    this->setWindowIcon(icon);
    ui->pushButton_5->setFixedWidth(30);
    ui->pushButton_4->setFixedWidth(30);
    ui->pushButton->setFixedWidth(30);
    remember = new QLineEdit(ui->comboBox_2->currentText());

    this->setStyleSheet(
                        "QScrollArea{color: white}"
                        "#pushButton_7{background-color: #a3d9f7; font-size: 16px;}"
                        "#pushButton_8{background-color: #a3d9f7; font-size: 16px;}"
                        "#pushButton_9{background-color: #a3d9f7; font-size: 16px;}"
                        "#pushButton_2{background-color: white;}"
                        "#pushButton_6{background-color: white;}"
                        "#pushButton_5{background-color: white;}"
                        "#pushButton_4{background-color: white;}"
                        "#pushButton_3{background-color: white;}"
                        "#pushButton{background-color: white;}"
                        "QComboBox{background-color: white}"
                        "QPushButton::hover{color: red}");
    ui->pushButton_6->setIcon(QIcon("C://Users//aleks//Qtprojects//curs_work//text-color.png"));
    ui->pushButton_5->setIcon(QIcon("C://Users//aleks//Qtprojects//curs_work//bold.png"));
    ui->pushButton_2->setIcon(QIcon("C://Users//aleks//Qtprojects//curs_work//background-color.png"));
    ui->pushButton_4->setIcon(QIcon("C://Users//aleks//Qtprojects//curs_work//italic.png"));
    ui->pushButton->setIcon(QIcon("C://Users//aleks//Qtprojects//curs_work//underlined.png"));
    ui->pushButton_3->setIcon(QIcon("C://Users//aleks//Qtprojects//curs_work//underline-color.png"));

    ui->gridLayout_2->setAlignment(Qt::AlignLeft);
    ui->gridLayout_4->setAlignment(Qt::AlignLeft);

    connect(ui->plainTextEdit, SIGNAL(cursorPositionChanged()), this, SLOT(Edit_button_clicked()));
    connect(ui->lineEdit, SIGNAL(cursorPositionChanged(int,int)), this, SLOT(Edit_button_clicked()));
    connect(ui->lineEdit_2, SIGNAL(cursorPositionChanged(int,int)), this, SLOT(Edit_button_clicked()));

    ui->comboBox_2->setFont(QFont("Times New Roman", 12));
    ui->lineEdit_2->setFont(QFont("Times New Roman", 12));
    ui->fontComboBox->setFont(QFont("Times New Roman", 12));
    ui->comboBox->setFont(QFont("Times New Roman", 12));
    ui->comboBox->setEditable(true);
}

void Editor::closeEvent(QCloseEvent *event){
    if(Close){
        event->accept();
        return;
    }
    if(ui->plainTextEdit->toPlainText().isEmpty() ||
            ui->lineEdit->text().isEmpty() ||
            ui->lineEdit_2->text().isEmpty()){
        QMessageBox* box = new QMessageBox;
        box->setWindowTitle("Ошибка");
        box->setText("Заполните все формы");
        box->show();
        event->ignore();
        return;
    }

    if(ui->comboBox_2->isHidden()){
        QMessageBox::StandardButton reply;

        reply = QMessageBox::question(this, "Выход", "Закрыть записку?",
                                      QMessageBox::Yes|QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            fillInf();
            emit EditedNote(inf);
            emit CloseNote(inf.NoteId);
            event->accept();

        } else {
            event->ignore();
        }
        return;
    }

    if(JustClose){
        QMessageBox::StandardButton reply;

        reply = QMessageBox::question(this, "Выход", "Сохранить записку?",
                                      QMessageBox::Yes|QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            fillInf();
            if(New){
                emit NewNote(inf);
                emit CloseNote(inf.NoteId);
            }
            else{
                emit CloseNote(inf.NoteId);
                emit EditedNote(inf);
            }
        } else {
            emit CloseNote(inf.NoteId);
        }
    }
    else{
        emit CloseNote(inf.NoteId);
    }
    event->accept();
}

void Editor::setNew(bool boolNew){
    this->New = boolNew;
}

void Editor::fillInf(){
    QDateTime time;
    inf.Name = ui->lineEdit->text();
    inf.Date = time.currentDateTime().toString();
    switch(ui->comboBox_2->currentIndex()){
    case 0:
        inf.expan = "Напоминание не выставлено";
        inf.DateRemind = "Напоминание не выставлено";
        break;
    case 1:
        inf.DateRemind = today;
        break;
    case 2:
        inf.DateRemind = tomorrow;
        break;
    case 3:
        inf.DateRemind = next_week;
        break;
    default:
        ui->comboBox_2->setCurrentIndex(5);
        inf.DateRemind = ui->comboBox_2->currentText();
        break;
    }


    inf.fontFamily = ui->fontComboBox->currentText();
    QTextDocument *doc = ui->plainTextEdit->document();

    inf.Text = doc->toHtml();
    inf.textSize = ui->comboBox->currentText();
    inf.coord = this->pos();
    inf.width = this->width();
    inf.height = this->height();
    this->setWindowTitle(inf.Name);
}

Editor::~Editor()
{
    delete ui;
}

void Editor::setNoteId(int NoteId){
    inf.NoteId = NoteId;
}

void Editor::NoteIdReceived(int NoteId){
    New = false;
    inf.NoteId = NoteId;
}

void Editor::ShowEdit(){

    ui->pushButton_8->show();
    ui->pushButton_7->show();
    ui->comboBox->show();
    ui->fontComboBox->show();
    ui->pushButton->show();
    ui->pushButton_2->show();
    ui->pushButton_3->show();
    ui->pushButton_4->show();
    ui->pushButton_5->show();
    ui->pushButton_6->show();
    ui->pushButton_9->show();
    ui->lineEdit->setReadOnly(false);
    ui->lineEdit_2->setReadOnly(false);
    ui->plainTextEdit->setReadOnly(false);
    ui->comboBox_2->show();
    ui->lineEdit_2->show();
    ui->label->show();
    ui->label_2->show();
    ui->label_3->show();
    ui->line->show();
    ui->line_2->show();
    ui->line_3->show();
    ui->line_4->show();

}

void Editor::RememberMessage(){
    QMessageBox msgBox;
    msgBox.setGeometry(this->pos().x() + this->width() / 2,
                       this->pos().y() + this->height() / 2,
                       msgBox.width(), msgBox.height());
    msgBox.setWindowTitle("Напоминание");
    msgBox.setText(inf.expan);
    msgBox.exec();
}

void Editor::HideEdit(){
    ui->pushButton_8->hide();
    ui->pushButton_7->hide();
    ui->comboBox->hide();
    ui->fontComboBox->hide();
    ui->lineEdit->setReadOnly(true);
    ui->lineEdit_2->setReadOnly(true);
    ui->plainTextEdit->setReadOnly(true);
    ui->pushButton->hide();
    ui->pushButton_2->hide();
    ui->pushButton_3->hide();
    ui->pushButton_4->hide();
    ui->pushButton_5->hide();
    ui->pushButton_6->hide();
    ui->pushButton_9->hide();
    ui->lineEdit_2->hide();
    ui->label->hide();
    ui->label_2->hide();
    ui->label_3->hide();

    ui->line->hide();
    ui->line_2->hide();
    ui->line_3->hide();
    ui->comboBox_2->hide();
    ui->line_4->hide();

}

void Editor::resizeEvent(QResizeEvent *event){
    event->accept();
}

void Editor::Edit_button_clicked(){
    if(!Showed){

        s = size();
        Showed = true;
    }
    ShowEdit();

}

void Editor::on_pushButton_7_clicked()
{

    if(ui->plainTextEdit->toPlainText().isEmpty() ||
            ui->lineEdit->text().isEmpty() ||
            ui->lineEdit_2->text().isEmpty()){
        QMessageBox* box = new QMessageBox;
        box->setWindowTitle("Ошибка");
        box->setText("Заполните все формы");
        box->show();
        return;
    }

    if(ui->lineEdit->text().length() > 60){
        QMessageBox* box = new QMessageBox;
        box->setWindowTitle("Ошибка");
        box->setText("Слишком длинное название");
        box->show();
        return;
    }
    if(ui->lineEdit_2->text().length() > 60){
        QMessageBox* box = new QMessageBox;
        box->setWindowTitle("Ошибка");
        box->setText("Слишком длинное пояснение записки");
        box->show();
        return;
    }
    fillInf();
    HideEdit();



    //inf.width = s.width();
    //inf.height = s.height();
    //this->layout()->setGeometry(QRect(this->pos().x(), this->pos().y(), s.width(), s.height()));
    //this->resize(s);
    //this->setGeometry(this->pos().x(), this->pos().y(), s.width(), s.height());

    Showed = false;
    if(New){
        emit NewNote(inf);
    }
    else{
        emit EditedNote(inf);

    }

}

void Editor::setInformation(NotesInform inform){
    inf = inform;

    ui->lineEdit->setText(inform.Name);
    ui->plainTextEdit->textCursor().insertHtml(inform.Text);
    ui->fontComboBox->setCurrentText(inform.fontFamily);
    if(inform.expan == "Напоминание не выставлено"){
        ui->comboBox_2->setCurrentIndex(0);
    }
    else{
        ui->comboBox_2->setCurrentIndex(5);
    }
    ui->lineEdit_2->setText(inform.expan);
    ui->comboBox->setCurrentText(inform.textSize);
    ui->comboBox_2->addItem(inform.DateRemind);

    this->setWindowTitle(inform.Name);
    this->setGeometry(inform.coord.x(), inform.coord.y() + 39,inform.width, inform.height);
}

void Editor::on_pushButton_8_clicked()
{
    JustClose = false;
    QMessageBox::StandardButton reply;

    reply = QMessageBox::question(this, "Выйти", "Вы уверены, что хотите удалить записку?",
                                  QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        emit DeleteNote(inf.NoteId);
        this->close();
    } else {
    }

}


void Editor::DateReceived(QDateTime date){
    if(ui->comboBox_2->count() == 5){
        ui->comboBox_2->addItem(date.toString());
    }
    else{
        ui->comboBox_2->setItemText(5, date.toString());
    }
    ui->comboBox_2->setCurrentIndex(5);
}

void Editor::on_comboBox_2_activated(int index)
{
    if(index == 4){
        ReminderEdit *rem = new ReminderEdit;
        connect(rem, SIGNAL(sendData(QDateTime)), this, SLOT(DateReceived(QDateTime)));
        rem->show();
    }
}

void Editor::on_pushButton_clicked()
{
    QTextCharFormat form = ui->plainTextEdit->textCursor().charFormat();
    if(form.underlineStyle() == QTextCharFormat::SpellCheckUnderline)
        form.setUnderlineStyle(QTextCharFormat::NoUnderline);
    else
        form.setUnderlineStyle(QTextCharFormat::SpellCheckUnderline);
    ui->plainTextEdit->mergeCurrentCharFormat(form);

}

void Editor::on_pushButton_2_clicked()
{
    QTextCharFormat form = ui->plainTextEdit->textCursor().charFormat();
    form.setBackground(QColorDialog::getColor(Qt::black, this));
    ui->plainTextEdit->mergeCurrentCharFormat(form);
}

void Editor::on_pushButton_3_clicked()
{
    QTextCharFormat form = ui->plainTextEdit->textCursor().charFormat();
    form.setUnderlineStyle(QTextCharFormat::SpellCheckUnderline);
    form.setUnderlineColor(QColorDialog::getColor(Qt::white, this));
    ui->plainTextEdit->mergeCurrentCharFormat(form);
}

void Editor::on_fontComboBox_currentFontChanged(const QFont &f)
{
    QTextCharFormat form = ui->plainTextEdit->textCursor().charFormat();
    form.setFontFamily(f.toString());
    ui->plainTextEdit->mergeCurrentCharFormat(form);
}

void Editor::on_pushButton_4_clicked()
{
    QTextCharFormat form = ui->plainTextEdit->textCursor().charFormat();
    if(form.fontItalic())
        form.setFontItalic(false);
    else
        form.setFontItalic(true);
    ui->plainTextEdit->mergeCurrentCharFormat(form);
}

void Editor::on_comboBox_activated(const QString &arg1)
{
    QTextCharFormat form = ui->plainTextEdit->textCursor().charFormat();
    form.setFontPointSize(arg1.toInt());
    ui->plainTextEdit->mergeCurrentCharFormat(form);
}

void Editor::on_pushButton_5_clicked()
{
    QTextCharFormat form = ui->plainTextEdit->textCursor().charFormat();
    if(form.fontWeight() == QFont::Bold)
        form.setFontWeight(QFont::Thin);
    else
        form.setFontWeight(QFont::Bold);
    ui->plainTextEdit->mergeCurrentCharFormat(form);
}

void Editor::on_pushButton_6_clicked()
{
    QTextCharFormat form = ui->plainTextEdit->textCursor().charFormat();
    form.setForeground(QColorDialog::getColor(Qt::black, this));
    ui->plainTextEdit->mergeCurrentCharFormat(form);
}

void Editor::on_pushButton_9_clicked()
{
    if(!New){
        ui->plainTextEdit->clear();
        setInformation(inf);
        HideEdit();
    }
    else{
        Close = true;
        this->close();
    }

}
