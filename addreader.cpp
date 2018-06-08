#include "addreader.h"
#include "ui_addreader.h"

AddReader::AddReader(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddReader)
{
    ui->setupUi(this);
    connect(ui->confirmButton, SIGNAL(clicked(bool)), SLOT(Confirm()));
}

AddReader::~AddReader()
{
    delete ui;
}

void AddReader::Confirm() {
    QString ridString = ui->readerIDEdit->text();
    QString readerNameString = ui->readerNameEdit->text();
    QString passwordString = ui->passwordEdit->text();
    QString majorString = ui->majorEdit->text();
    QString queryString = "";
    queryString = "insert "
                  "into READER "
                  "values(" + ridString + ",'" + readerNameString + "','" + passwordString + "','" + majorString + "')";
    qDebug() << queryString;
    if(!DataBase::ExecuteQuery(queryString).isActive()){
        qDebug() << "fail to insert";
        QMessageBox::information(this,"Warning","Invalid input");
    }
    else {
        emit addOK();
        this->close();
    }
}
