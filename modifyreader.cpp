#include "modifyreader.h"
#include "ui_modifyreader.h"

ModifyReader::ModifyReader(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModifyReader)
{
    ui->setupUi(this);
    connect(ui->confirmButton, SIGNAL(clicked(bool)), SLOT(Confirm()));
}

ModifyReader::~ModifyReader()
{
    delete ui;
}

void ModifyReader::SetRID(int rid) {
    this->selectedRID = rid;
}

void ModifyReader::Confirm() {
    QString readerIDString = ui->readerIDEdit->text();
    QString readerNameString = ui->readerNameEdit->text();
    QString passwordString = ui->passwordEdit->text();
    QString majorString = ui->majorEdit->text();
    QString queryString = "update READER set RNAME = '" + readerNameString + "',RPASSWORD = '" + passwordString + "'," +
                          "MAJOR = '" + majorString + "' where RID = " + readerIDString;
    qDebug() << queryString;
    if(!DataBase::ExecuteQuery(queryString).isActive()){
        qDebug() << "fail to modify";
        QMessageBox::information(this,"Warning","Fail to modify");
    }
    else {
        emit modifyOK();
        this->close();
    }
}

void ModifyReader::Init() {
    QString queryString = "select * from READER where RID = " + QString::number(selectedRID);
    QSqlQuery result(DataBase::ExecuteQuery(queryString));
    result.next();
    ui->readerIDEdit->setText(result.value(0).toString());
    ui->readerIDEdit->setDisabled(true);
    ui->readerNameEdit->setText(result.value(1).toString());
    ui->passwordEdit->setText(result.value(2).toString());
    ui->majorEdit->setText(result.value(3).toString());
}

