#ifndef DATABASE_H
#define DATABASE_H

#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlQueryModel>
#include <QTableView>
#include <QVector>
#include <QMessageBox>
#include <QAbstractItemModel>
#include <QDateTime>

#define MAXNBORROWMOUNT 5

class DataBase
{
public:
    DataBase();
    static QSqlQuery ExecuteQuery(QString cmd);
    static void ExecuteQueryModel(QTableView *table, QString cmd, QVector<QString> headerDataVector);
    static void closeDB();
public:
    static QSqlDatabase db;
};

#endif // DATABASE_H
