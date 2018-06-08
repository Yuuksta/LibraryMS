#include "mydb.h"

MyDB::MyDB()
{

}

MyDB::~MyDB()
{
    delete db;
}

QSqlQuery *MyDB::ExecuteQuery(QString cmd) {
    QSqlQuery *query(cmd);
    return query;
}
