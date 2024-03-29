#ifndef DATEBASE_H
#define DATEBASE_H


#include <QObject>
#include <QtSql/QSql>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDatabase>
#include <QFile>
#include <QDate>
#include <QDebug>

/* Директивы имен таблицы, полей таблицы и базы данных */
#define DATABASE_HOSTNAME   "NameDataBase"
#define DATABASE_NAME       "Name.db"

#define TABLE                   "NameTable"         // Название таблицы
#define TABLE_FNAME             "FisrtName"         // Вторая колонка
#define TABLE_SNAME             "SurName"           // Третья колонка
#define TABLE_NIK               "Nik"               // Четвертая колонка

// Первая колонка содержит Autoincrement ID

class database
{
    Q_OBJECT
public:
    database();

    ~database();

    /* Методы для непосредственной работы с классом
     * Подключение к базе данных и вставка записей в таблицу
     * */
    void connectToDataBase();

private:
    // Сам объект базы данных, с которым будет производиться работа
    QSqlDatabase    db;

private:
    /* Внутренние методы для работы с базой данных
     * */
    bool openDataBase();        // Открытие базы данных
    bool restoreDataBase();     // Восстановление базы данных
    void closeDataBase();       // Закрытие базы данных
    bool createTable();         // Создание базы таблицы в базе данных

public slots:
    bool inserIntoTable(const QVariantList &data);      // Добавление записей в таблицу
    bool inserIntoTable(const QString &fname, const QString &sname, const QString &nik);
    bool removeRecord(const int id); // Удаление записи из таблицы по её id
};

#endif // DATEBASE_H
