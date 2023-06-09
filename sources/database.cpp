#include "database.h"

void deleteAllTables(){
    QSqlQuery query;
    if(!query.exec( "DROP TABLE Notes")){
        qDebug() << "DataBase: error of drop " << "Notes";
        qDebug() << query.lastError().text();
    }
    if(!query.exec( "DROP TABLE NoteFormat")){
        qDebug() << "DataBase: error of drop " << "NoteFormat";
        qDebug() << query.lastError().text();
    }
    if(!query.exec( "DROP TABLE Reminders")){
        qDebug() << "DataBase: error of drop " << "Reminders";
        qDebug() << query.lastError().text();
    }
}


Database::Database()
{
}



void Database::connectToDataBase()
{
    QDir dir;
    QString path = dir.absolutePath();
    if(!QFile(path + "Notes.db").exists()){
        this->restoreDataBase();
    } else {
        this->openDataBase();
    }
}

bool Database::restoreDataBase()
{
    if(this->openDataBase()){
        return (this->createTable()) ? true : false;
    } else {
        qDebug() << "Не удалось восстановить базу данных";
        return false;
    }
    return false;
}

bool Database::openDataBase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("NotesDataBase");

    QDir dir;
    QString path = dir.absolutePath();
    db.setDatabaseName(path + "Notes.db");
    if(db.open()){
        return true;
    } else {
        return false;
    }
}

void Database::closeDataBase()
{
    db.close();
}

bool Database::createTable()
{
    QSqlQuery query;

    if(!query.exec( "CREATE TABLE Notes ("
                            "NoteId INTEGER PRIMARY KEY AUTOINCREMENT, "
                               "NOTE_NAME VARCHAR(255)    NOT NULL,"
                            "NOTE_CREATE_DATE VARCHAR(255)    NOT NULL,"
                            "TEXT MEDIUMTEXT NOT NULL"
                        " )"
                    )){
        qDebug() << "DataBase: error of create " << "Notes";
        qDebug() << query.lastError().text();
    }
    if(!query.exec( "CREATE TABLE NoteFormat ("
                            "NoteId INT, "
                               "FONT VARCHAR(255)    NOT NULL,"
                            "TEXT_SIZE VARCHAR(255)    NOT NULL,"
                    "OPENED VARCHAR(255) NOT NULL,"
                    "GEOMETRY VARCHAR(255) NOT NULL,"
                        " FOREIGN KEY (NoteId)  REFERENCES Notes (NoteId))"
                    )){
        qDebug() << "DataBase: error of create " << "NoteFormat";
        qDebug() << query.lastError().text();
    }
    if(!query.exec( "CREATE TABLE Reminders ("
                            "NoteId INT, "
                               "DATE_REMIND VARCHAR(255)    NOT NULL,"
                            "EXPLANATION VARCHAR(255)    NOT NULL,"
                        " FOREIGN KEY (NoteId)  REFERENCES Notes (NoteId))"
                    )){
        qDebug() << "DataBase: error of create " << "Reminders";
        qDebug() << query.lastError().text();
    }
}

bool Database::inserIntoTables(const QVariantList &data)
{

    QSqlQuery query;

    query.prepare("INSERT INTO Notes (NOTE_NAME,NOTE_CREATE_DATE,TEXT) VALUES(:name,:date,:text)");
    query.bindValue(":name", data[0].toString());
    query.bindValue(":date", data[1].toString());
    query.bindValue(":text", data[2].toString());

    if(!query.exec()){
        qDebug() << "error insert into " << "Notes";
        qDebug() << query.lastError().text();
    }

    QSqlTableModel model;
    model.setTable("Notes");
    model.select();

    query.prepare("INSERT INTO NoteFormat (NoteId,FONT,TEXT_SIZE,OPENED,GEOMETRY) VALUES(:id,:font,:text_size,:opened,:geom)");
    query.bindValue(":id", model.rowCount());
    query.bindValue(":font", data[5].toString());
    query.bindValue(":text_size", data[6].toString());

    if(data[7].toBool()){
        query.bindValue(":opened", "TRUE");
    }
    else{
        query.bindValue(":opened", "FALSE");
    }

    query.bindValue(":geom", data[8].toString());
    if(!query.exec()){
        qDebug() << "error insert into " << "NoteFormat";
        qDebug() << query.lastError().text();
    }

    query.prepare("INSERT INTO Reminders (NoteId, DATE_REMIND,EXPLANATION) VALUES(:id,:date,:explan)");
    query.bindValue(":id", model.rowCount());
    query.bindValue(":date", data[3].toString());
    query.bindValue(":explan", data[4].toString());

    if(!query.exec()){
        qDebug() << "error insert into " << "Reminders";
        qDebug() << query.lastError().text();
    }
}


bool Database::removeRecord(QString table_name, const int id)
{
    QSqlQuery query;

    query.prepare("DELETE FROM " + table_name + " WHERE NoteId= :ID ;");
    query.bindValue(":ID", id);

    if(!query.exec()){
        qDebug() << "error delete row " << table_name;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}


void Database::cleanAllTables(){
    QSqlQuery query;
    if(!query.exec("delete from Notes;")){
        qDebug() << "error delete row " << "Notes";
        qDebug() << query.lastError().text();
    }
    if(!query.exec("delete from NoteFormat;")){
        qDebug() << "error delete row " << "NoteFormat";
        qDebug() << query.lastError().text();
    }
    if(!query.exec("delete from Reminders;")){
        qDebug() << "error delete row " << "Reminders";
        qDebug() << query.lastError().text();
    }

}
