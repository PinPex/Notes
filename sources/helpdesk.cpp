#include "helpdesk.h"
#include "ui_helpdesk.h"



HelpDesk::HelpDesk(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HelpDesk)
{
    currentFile.setFileName("C://Users//aleks//Qtprojects//curs_work//help.html");
    ui->setupUi(this);
    if(!currentFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning("Файл справки не найден!");
    }
    connect(ui->textBrowser, SIGNAL(anchorClicked(QUrl)), this, SLOT(changedUrl(QUrl)));
    ui->textBrowser->setHtml(QString::fromUtf8(currentFile.readAll()));
    this->setWindowTitle("Справка");
    QDir dir;
    QString path = dir.currentPath();
    QPixmap pixmap(path + "/debug/notes.png");
    QIcon icon(pixmap);
    this->setWindowIcon(icon);
    files.append("help.html");
    files.append("newnote.html");
    files.append("deletenote.html");
    files.append("noteedit.html");
    files.append("search.html");
    files.append("sortnotes.html");
}

void HelpDesk::changedUrl(QUrl url){
    currentFile.setFileName("C://Users//aleks//Qtprojects//curs_work//" + url.toString().remove(0, url.toString().lastIndexOf("/") + 1));
    if(!currentFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning("Файл справки не найден!");
    }
    ui->textBrowser->setHtml(QString::fromUtf8(currentFile.readAll()));
}

HelpDesk::~HelpDesk()
{
    delete ui;
}

void HelpDesk::on_pushButton_clicked()
{
    currentFile.setFileName("C://Users//aleks//Qtprojects//curs_work//help.html");
    if(!currentFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning("Файл справки не найден!");
    }
    ui->textBrowser->setHtml(QString::fromUtf8(currentFile.readAll()));
}

void HelpDesk::on_pushButton_3_clicked()
{
    if(files.indexOf(currentFile.fileName().remove(0, currentFile.fileName().lastIndexOf("/") + 1)) == files.size() - 1){
        currentFile.setFileName("C://Users//aleks//Qtprojects//curs_work//" + files[0]);
    }
    else{
        currentFile.setFileName("C://Users//aleks//Qtprojects//curs_work//" + files[files.indexOf(currentFile.fileName().remove(0, currentFile.fileName().lastIndexOf("/") + 1)) + 1]);
    }
    if(!currentFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning("Файл справки не найден!");
    }
    ui->textBrowser->setHtml(QString::fromUtf8(currentFile.readAll()));

}

void HelpDesk::on_pushButton_2_clicked()
{
    if(files.indexOf(currentFile.fileName().remove(0, currentFile.fileName().lastIndexOf("/") + 1)) == 0){
        currentFile.setFileName("C://Users//aleks//Qtprojects//curs_work//" + files[files.size() - 1]);
    }
    else{
        currentFile.setFileName("C://Users//aleks//Qtprojects//curs_work//" + files[files.indexOf(currentFile.fileName().remove(0, currentFile.fileName().lastIndexOf("/") + 1)) - 1]);
    }
    if(!currentFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning("Файл справки не найден!");
    }
    ui->textBrowser->setHtml(QString::fromUtf8(currentFile.readAll()));

}
