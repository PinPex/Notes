#include "widget.h"
#include "ui_widget.h"
#include "editor.h"
#include "helpdesk.h"





Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->lineEdit->setFont(QFont("Times new roman", 14));
    ui->lineEdit->setPlaceholderText("Поиск...");
    ui->lineEdit_2->setFont(QFont("Times new roman", 14));
    ui->lineEdit_2->setPlaceholderText("Поиск...");

    this->setWindowTitle("Записки");

    this->setWindowIcon(QIcon(QPixmap(QDir::currentPath() + "/debug/notes.png")));

    ui->label->hide();
    ui->label_2->hide();

    base.connectToDataBase();
    inf = getInformationDB();

    showNotes(inf);

    this->setStyleSheet("#pushButton_2{color:black; font-size: 16px; background-color: #a3d9f7}"
                        "#pushButton{color:black; font-size: 16px; background-color: #a3d9f7}"
                        "#pushButton_5{color:black; font-size: 16px; background-color: #a3d9f7}"
                        "#pushButton_6{color:black; font-size: 16px; background-color: #a3d9f7}"
                        "#pushButton_4{color:black; font-size: 16px; background-color: #a3d9f7}"
                        "#pushButton_7{color:black; font-size: 16px; background-color: #a3d9f7}"
                        "#pushButton_3{background-color: white}"
                        "QTabWidget::tab-bar {min-width: 999999px;}"
                        "QTabBar::tab{background-color: blue; color: white; font-size: 16px; border: 1px solid black; min-height: 24px;}"
                        "QTabBar::tab:hover{color: red; border-color: red}"
                        "#tab{border: 1px solid black;}"
                        "#tab_2{border: 1px solid black;}"
                        "#Widget{background-color: white}"
                        );


    ui->pushButton_3->setIcon(QIcon("C://Users//aleks//Qtprojects//curs_work//help.png"));

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),
              this, SLOT(timerEvent()));
    timer->start(1000);

}

void Widget::timerEvent(){
    if(inf.size() == 0)
        return;

    for(int i = 0; i < inf.size(); ++i){
        if(inf[i].DateRemind == "Напоминание не выставлено"){
            continue;
        }
        QDateTime cur_time = QDateTime::currentDateTime();
        QDateTime time = QDateTime::fromString(inf[i].DateRemind);
        if(cur_time.toString() == time.toString()){
            if(opened.indexOf(inf[i].NoteId) != -1){
                for(int j = 0; j < editors.size(); ++j){
                    if(inf[i].NoteId == editors[j]->inf.NoteId){
                        editors[j]->showNormal();
                        editors[j]->activateWindow();
                        editors[j]->RememberMessage();
                    }
                }
            }
            else{
                Editor* edit = new Editor;
                edit->setInformation(inf[i]);
                editors.append(edit);
                edit->HideEdit();

                edit->show();
                edit->RememberMessage();
                connect(edit, SIGNAL(EditedNote(NotesInform)), this, SLOT(EditedNote(NotesInform)));
                connect(edit, SIGNAL(CloseNote(int)), this, SLOT(CloseNote(int)));
                connect(edit, SIGNAL(DeleteNote(int)), this, SLOT(DeleteNote(int)));
                opened.append(inf[i].NoteId);
            }
        }
    }
}

void Widget::WriteInBase(){
    base.cleanAllTables();
    for(int i = 0; i < inf.size(); ++i){
        base.inserIntoTables(fillVariantList(inf[i]));
    }
}

void Widget::WriteCoordinates(){
    for(int i = 0; i < opened.size(); ++i){
        for(int j = 0; j < editors.size(); ++j){
            if(opened[i] == editors[j]->inf.NoteId){
                for(int g = 0; g < inf.size(); ++g){
                    if(inf[g].NoteId == editors[j]->inf.NoteId){
                        inf[g].coord = editors[j]->pos();
                        inf[g].width = editors[j]->width();
                        inf[g].height = editors[j]->height();
                    }
                }
            }
        }
    }
}

void Widget::closeEvent(QCloseEvent *event){
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Выйти", "Вы уверены, что хотите закрыть приложение?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        WriteCoordinates();
        WriteInBase();
        qApp->quit();
    } else {
        event->ignore();
    }
}

QVector<NotesInform> Widget::getInformationDB(){
    QSqlTableModel model;
    model.setTable("Notes");
    model.select();
    QVector<NotesInform> inform;
    if(model.rowCount() == 0){ //Check empty database
        return inform;
    }
    inform.resize(model.rowCount()); //Get information from Notes
    for (int i = 0; i < inform.size(); ++i) {
        inform[i].NoteId = i;
        inform[i].Name = model.record(i).value("NOTE_NAME").toString();
        inform[i].Date = model.record(i).value("NOTE_CREATE_DATE").toString();
        inform[i].Text = model.record(i).value("TEXT").toString();
    }
    model.setTable("NoteFormat");
    model.select();
    for (int i = 0; i < inform.size(); ++i) { //Get information from NoteFormat
        inform[i].fontFamily = model.record(i).value("FONT").toString();
        inform[i].textSize = model.record(i).value("TEXT_SIZE").toString();

        QStringList vec = model.record(i).value("GEOMETRY").toString().split("*");
        inform[i].coord = QPoint(vec[0].toInt(), vec[1].toInt());
        inform[i].width = vec[2].toInt();
        inform[i].height = vec[3].toInt();

        if(model.record(i).value("OPENED").toString() == "TRUE"){
            opened.append(inform[i].NoteId);
        }
    }

    model.setTable("Reminders");
    model.select();
    for (int i = 0; i < inform.size(); ++i) { //Get information from Reminders
        inform[i].DateRemind = model.record(i).value("DATE_REMIND").toString();
        inform[i].expan = model.record(i).value("EXPLANATION").toString();
    }
    for(int i = 0; i < inform.size(); ++i){ //Create opened editors
        if(opened.indexOf(inform[i].NoteId) != -1){
            Editor* edit = new Editor;
            edit->setInformation(inform[i]);
            editors.append(edit);
            edit->HideEdit();
            edit->show();

            connect(edit, SIGNAL(EditedNote(NotesInform)), this, SLOT(EditedNote(NotesInform)));
            connect(edit, SIGNAL(CloseNote(int)), this, SLOT(CloseNote(int)));
            connect(edit, SIGNAL(DeleteNote(int)), this, SLOT(DeleteNote(int)));
        }
    }

    return inform;
}

void Widget::deleteAllItems(QTableWidget* wid){
    wid->clear();
    wid->setRowCount(0);
}

void Widget::showNotes(QVector<NotesInform> &inf){

    deleteAllItems(ui->tableWidget);
    deleteAllItems(ui->tableWidget_2);

    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setShowGrid(true);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList({"Название записки","Дата изменения", ""}));

    ui->tableWidget_2->setColumnCount(3);
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_2->setShowGrid(true);
    ui->tableWidget_2->setHorizontalHeaderLabels(QStringList({"Объяснение напоминания","Дата напоминания", ""}));
    for(int i = 0; i < inf.size(); ++i){

        QTableWidgetItem *item = new QTableWidgetItem();
        item->data(Qt::CheckStateRole);
        item->setCheckState(Qt::Unchecked);

        ui->tableWidget->insertRow(i);
        ui->tableWidget->setItem(i,0, new QTableWidgetItem(inf[i].Name));
        ui->tableWidget->setItem(i,1, new QTableWidgetItem(inf[i].Date));
        ui->tableWidget->setItem(i, 2, item);

        QTableWidgetItem *item_2 = new QTableWidgetItem();
        item_2->data(Qt::CheckStateRole);
        item_2->setCheckState(Qt::Unchecked);

        ui->tableWidget_2->insertRow(i);
        ui->tableWidget_2->setItem(i,0, new QTableWidgetItem(inf[i].expan));
        ui->tableWidget_2->setItem(i, 1, new QTableWidgetItem(inf[i].DateRemind));
        ui->tableWidget_2->setItem(i, 2, item_2);

    }
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_2->horizontalHeader()->setStretchLastSection(true);

    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget_2->resizeColumnsToContents();

    ui->tableWidget->setSortingEnabled(true);
    ui->tableWidget_2->setSortingEnabled(true);
    ui->tableWidget->setAutoScroll(false);
    ui->tableWidget_2->setAutoScroll(false);
    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tableWidget_2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tableWidget->setFont(QFont("Times new roman", 10));
    ui->tableWidget_2->setFont(QFont("Times new roman", 10));


}


Widget::~Widget()
{
    delete ui;
}

QVariantList Widget::fillVariantList(NotesInform inform){
    QVariantList list;
    list.append(inform.Name);
    list.append(inform.Date);
    list.append(inform.Text);
    list.append(inform.DateRemind);
    list.append(inform.expan);
    list.append(inform.fontFamily);
    list.append(inform.textSize);
    if(opened.indexOf(inform.NoteId) != -1){
        list.append(true);
    }
    else{
        list.append(false);
    }
    list.append(QString::number(inform.coord.x()) + "*" + QString::number(inform.coord.y()) + "*" +
                QString::number(inform.width) + "*" + QString::number(inform.height));
    return list;
}

int Widget::getLastNoteId(){
    int n;
    if(!inf.empty()){
        n = inf.last().NoteId + 1;
    }
    else{
        n = 0;
    }

    inf.append(NotesInform(n));
    return n;
}

void Widget::NewNote(NotesInform &inf){
    for(int i = 0; i < this->inf.size(); ++i){
        if(this->inf[i].Name == inf.Name){
            for(int j = 0; j < editors.size(); ++j){
                if(inf.NoteId == editors[j]->inf.NoteId){
                    editors[j]->setNew(true);
                }
            }
            QMessageBox *box = new QMessageBox;
            box->setText("Запись с таким именем уже существует");
            box->setWindowTitle("Ошибка");
            box->show();

            return;
        }
    }

    for(int j = 0; j < editors.size(); ++j){
        if(inf.NoteId == editors[j]->inf.NoteId){
            editors[j]->setNew(false);
        }
    }
    for(int i = 0; i < this->inf.size(); ++i){
        if(inf.NoteId == this->inf[i].NoteId){
            this->inf[i] = inf;
        }
    }
    showNotes(this->inf);
    opened.append(inf.NoteId);
    QMessageBox* box_1 = new QMessageBox;
    box_1->setWindowTitle("Успешно");
    box_1->setText("Записка успешно сохранена");
    box_1->show();
}

void Widget::CloseNote(int NoteId){
    if(opened.indexOf(NoteId) != -1){
        opened.remove(opened.indexOf(NoteId));
    }
}

void Widget::DeleteNote(int NoteId){
    for(int i = 0; i < inf.size(); ++i){
        if(inf[i].NoteId == NoteId){
            inf.remove(i,1);

            showNotes(inf);
            break;
        }
    }
    for(int i = 0; i < editors.size(); ++i){
        if(editors[i]->inf.NoteId == NoteId){
            editors.remove(i, 1);
        }
    }
}

void Widget::on_pushButton_2_clicked()
{
    Editor* edit = new Editor;
    edit->setNoteId(getLastNoteId());
    editors.append(edit);
    edit->setNew(true);
    edit->resize(0, 0);

    connect(editors.last(), SIGNAL(NewNote(NotesInform&)), this, SLOT(NewNote(NotesInform&)));
    connect(editors.last(), SIGNAL(CloseNote(int)), this, SLOT(CloseNote(int)));
    connect(editors.last(), SIGNAL(DeleteNote(int)), this, SLOT(DeleteNote(int)));
    connect(editors.last(), SIGNAL(EditedNote(NotesInform)), this, SLOT(EditedNote(NotesInform)));

    connect(this, SIGNAL(NoteIdReceive(int)), editors.last(), SLOT(NoteIdReceived(int)));
    edit->show();
}

void Widget::EditedNote(NotesInform inf){
    this->inf[inf.NoteId] = inf;
    showNotes(this->inf);
}

void Widget::OpenEdit(QTableWidget* tab, QString str){
    int count = 0;
    for(int i = 0; i < tab->rowCount(); ++i){
        if(tab->item(i,2)->checkState() == Qt::Checked){
            count++;
        }
    }

    if(count == 0){
        QMessageBox *box = new QMessageBox;
        box->setText("Необходимо выбрать строки\n(нажать галочки напротив записей, которые вы хотите открыть)");
        box->setWindowTitle("Ошибка");
        box->show();
        return;
    }

    for(int i = 0; i < tab->rowCount(); ++i){
        if(tab->item(i,2)->checkState() == Qt::Checked){
            int cur;
            for(int g = 0; g < inf.size(); ++g){
                if(str == "name"){
                    if(tab->item(i,0)->text() == inf[g].Name){
                        cur = g;
                        break;
                    }
                }
                else{
                    if(tab->item(i,0)->text() == inf[g].expan){
                        cur = g;
                        break;
                    }
                }

            }


            if(opened.indexOf(inf[cur].NoteId) == -1){
                Editor* edit = new Editor;
                edit->setInformation(inf[cur]);
                editors.append(edit);
                edit->HideEdit();
                edit->show();

                connect(edit, SIGNAL(EditedNote(NotesInform)), this, SLOT(EditedNote(NotesInform)));
                connect(edit, SIGNAL(CloseNote(int)), this, SLOT(CloseNote(int)));
                connect(edit, SIGNAL(DeleteNote(int)), this, SLOT(DeleteNote(int)));
                opened.append(inf[cur].NoteId);
            }
            else{
                for(int j = 0; j < editors.size(); ++j){
                    if(editors[j]->inf.NoteId == inf[cur].NoteId){
                        editors[j]->showNormal();
                        editors[j]->activateWindow();
                    }
                }
            }
            tab->item(i,2)->setCheckState(Qt::Unchecked);
        }
    }
}

void Widget::on_pushButton_clicked()
{
    OpenEdit(ui->tableWidget, "name");
}

void Widget::ResearchLabel(QLabel *lab, QString arg1){
    if(arg1 == ""){
        showNotes(this->inf);
    }
    else{
        QVector<NotesInform> result;
        if(ui->label == lab){
            for(int i = 0; i < inf.size(); ++i){
                if(inf[i].Name.toLower().indexOf(arg1.toLower()) != -1){
                    result.append(inf[i]);
                }
            }
        }
        else{
            for(int i = 0; i < inf.size(); ++i){
                if(inf[i].expan.toLower().indexOf(arg1.toLower()) != -1){
                    result.append(inf[i]);
                }
            }
        }
        if(result.size() == 0){
            lab->show();
        }
        else{
            lab->hide();
        }
        showNotes(result);
    }
}

void Widget::on_lineEdit_textChanged(const QString &arg1)
{
    ResearchLabel(ui->label, arg1);
}

void Widget::on_lineEdit_2_textChanged(const QString &arg1)
{
    ResearchLabel(ui->label_2, arg1);
}


void Widget::on_pushButton_4_clicked()
{
    OpenEdit(ui->tableWidget_2, "exp");
}

void Widget::DeleteEdit(QTableWidget* tab){

    int count = 0;
        QStringList list;
        for(int i = 0; i < tab->rowCount(); ++i){
            if(tab->item(i,2)->checkState() == Qt::Checked){
                if(list.size() < 3)
                    list.append(inf[i].Name);
                count++;
            }
        }
        if(count == 0){
            QMessageBox *box = new QMessageBox;
            box->setText("Необходимо выбрать строки\n(нажать галочки напротив записей, которые вы хотите удалить)");
            box->setWindowTitle("Ошибка");
            box->show();
            return;
        }

        QMessageBox::StandardButton reply;
        QString str = "Вы уверены, что хотите удалить " + QString::number(count);
        if(count == 1) str += " элемент?:\n";
        else str += " элементов?:\n";
        for(int i = 0; i < list.size(); ++i){
            str += list[i] + "\n";
        }
        if(count > 3) str += "...";
        reply = QMessageBox::question(this, "Удалить", str,
                                      QMessageBox::Yes|QMessageBox::No);


        if (reply == QMessageBox::Yes) {
            for(int i = 0; i < tab->rowCount(); ++i){
                if(tab->item(i, 2)->checkState() == Qt::Checked){
                    for(int j = 0; j < editors.size(); ++j){
                        if(editors[j]->inf.NoteId == inf[i].NoteId){
                            editors[j]->Close = true;
                            editors[j]->close();
                            editors.remove(j, 1);
                        }
                    }
                    tab->removeRow(i);
                    inf.remove(i, 1);
                    i--;
                }
            }
            showNotes(inf);
        }
        else{
            for(int i = 0; i < tab->rowCount(); ++i){
                if(tab->item(i,2)->checkState() == Qt::Checked){
                    tab->item(i,2)->setCheckState(Qt::Unchecked);
                }
            }
        }
}

void Widget::on_pushButton_5_clicked()
{
    DeleteEdit(ui->tableWidget);
}

void Widget::on_pushButton_6_clicked()
{
    DeleteEdit(ui->tableWidget_2);
}

void Widget::on_pushButton_3_clicked()
{
    HelpDesk* help = new HelpDesk;
    help->show();
}

void Widget::on_pushButton_7_clicked()
{
    Editor* edit = new Editor;
    edit->setNoteId(getLastNoteId());
    editors.append(edit);
    edit->setNew(true);
    edit->resize(0, 0);
    connect(editors.last(), SIGNAL(NewNote(NotesInform&)), this, SLOT(NewNote(NotesInform&)));
    connect(editors.last(), SIGNAL(CloseNote(int)), this, SLOT(CloseNote(int)));
    connect(editors.last(), SIGNAL(DeleteNote(int)), this, SLOT(DeleteNote(int)));
    connect(editors.last(), SIGNAL(EditedNote(NotesInform)), this, SLOT(EditedNote(NotesInform)));
    connect(this, SIGNAL(NoteIdReceive(int)), editors.last(), SLOT(NoteIdReceived(int)));
    edit->show();
}
