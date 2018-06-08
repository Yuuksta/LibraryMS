#include "database.h"

QSqlDatabase DataBase::db;

DataBase::DataBase()
{

}

QSqlQuery DataBase::ExecuteQuery(QString cmd) {
    QSqlQuery query(cmd);
    //qDebug() << query.value(0);
    return query;
}

void DataBase::ExecuteQueryModel(QTableView *table , QString cmd, QVector<QString> headerDataVector) {
    QSqlQueryModel *model = new QSqlQueryModel(table);
    model->setQuery(cmd);
    int size = headerDataVector.size();
    for(int i = 0; i < size; i++) {
        model->setHeaderData(i,Qt::Horizontal, headerDataVector.at(i));
    }
    table->setModel(model);
    table->resizeColumnsToContents();
}

void DataBase::closeDB() {
    DataBase::db.close();
}

