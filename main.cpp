#include "mainwindow.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile qss("Style.qss");
    qss.open(QFile::ReadOnly);
    qApp->setStyleSheet(qss.readAll());
    qss.close();
    DataBase::db = QSqlDatabase::addDatabase("QMYSQL");
    DataBase::db.setHostName("127.0.0.1");
    DataBase::db.setPort(3306);
    DataBase::db.setUserName("root");
    DataBase::db.setPassword("WSZ246879");
    DataBase::db.setDatabaseName("LibraryDB");
    DataBase::db.open();
    MainWindow w;
    w.show();

    return a.exec();
}
