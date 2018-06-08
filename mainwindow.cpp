#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->loginButton, SIGNAL(clicked(bool)), SLOT(Login()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Login()
{
    QString userIDString = ui->idEdit->text();
    QString passwordString = ui->pswEdit->text();
    if(ui->readerTag->isChecked()) {
        QSqlQuery result(DataBase::ExecuteQuery("select RPASSWORD from READER where RID = " + userIDString));
        result.next();
        qDebug() << result.value(0).toString();
        if(passwordString == result.value(0).toString() && !passwordString.isEmpty() && !userIDString.isEmpty()) {
            readerWindow = new ReaderWindow(userIDString);
            readerWindow->show();
            this->hide();
        }
        else {
            qDebug() << "wrong password";
            QMessageBox::information(this,"Info","Wrong Password","OK");
        }
    }

    if(ui->adminTag->isChecked()) {
        QSqlQuery result(DataBase::ExecuteQuery("select APASSWORD from ADMIN where AID = " + userIDString));
        result.next();
        qDebug() << result.value(0).toString();
        if(passwordString == result.value(0).toString() && !passwordString.isEmpty() && !userIDString.isEmpty()) {
            adminWindow = new AdminWindow(userIDString);
            adminWindow->show();
            this->hide();
        }
        else {
            qDebug() << "wrong password";
            QMessageBox::information(this,"Info","Wrong Password","OK");
        }
    }
}
