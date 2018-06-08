#ifndef MYDB_H
#define MYDB_H

#include <QObject>
#include <QSql>
#include <QSqlDatabase>
#include <QDebug>

class MyDB
{
public:

    QSqlQuery *ExecuteQuery(QString cmd);

signals:

public slots:

private:
    QSqlDatabase db;

};

#endif // MYDB_H
