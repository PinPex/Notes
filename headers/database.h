#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QFile>
#include <QDate>
#include <QDir>
#include <QDebug>

class Database
{
public:
    Database();

    void connectToDataBase();

    QSqlDatabase    db;

    bool openDataBase();        // Открытие базы данных
    bool restoreDataBase();     // Восстановление базы данных
    void closeDataBase();       // Закрытие базы данных
    bool createTable();
    void cleanAllTables();

public slots:
    bool inserIntoTables(const QVariantList &data);
    //bool inserIntoTable(const QString &fname, const QString &sname, const QString &nik);
    bool removeRecord(QString table_name, const int id);
};

#endif // DATABASE_H
