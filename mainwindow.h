#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSql>
#include <QSqlDatabase>
#include <QDebug>
#include "adminwindow.h"
#include "readerwindow.h"
#include "database.h"
#include <QMessageBox>
#include <QVector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void Login();

private:
    Ui::MainWindow *ui;
    AdminWindow *adminWindow;
    ReaderWindow *readerWindow;
};

#endif // MAINWINDOW_H
