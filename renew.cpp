#include "renew.h"
#include "ui_renew.h"

Renew::Renew(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Renew)
{
    ui->setupUi(this);
    connect(ui->confirmButton, SIGNAL(clicked(bool)), SLOT(Confirm()));
    ui->dateTimeEdit->setMinimumDateTime(QDateTime::currentDateTime());
}

Renew::~Renew()
{
    delete ui;
}

void Renew::Confirm() {
    QDateTime ddl;
    QString ddlString;
    QString currentLID;
    QSqlQuery result;

    //get currentLID
    result = DataBase::ExecuteQuery("select COUNT(*) from LOG");
    result.next();
    currentLID = QString::number(result.value(0).toInt() + 1);
    qDebug() << "currentLID:" + currentLID;
    result.clear();

    // get ddl String
    ddl = ui->dateTimeEdit->dateTime();
    ddlString = QString::number(ddl.date().year()) + "-" + QString::number(ddl.date().month()) + "-" +
            QString::number(ddl.date().day()) + " " + QString::number(ddl.time().hour()) + ":" +
            QString::number(ddl.time().minute()) + ":" + QString::number(ddl.time().second());
    qDebug() << "ddlString:" + ddlString;
    DataBase::ExecuteQuery("update BORROW set DDL = '" + ddlString + "' "
                           "where BID = " +  this->BID + " and RID = " + this->RID);
    DataBase::ExecuteQuery("insert into LOG values(" + currentLID + "," + this->RID + "," + this->BID + ",now(),'renew')");
    QMessageBox::information(this, "Info", "Renew Successfully");
    this->close();
}

void Renew::SetBID(QString bid) {
    this->BID = bid;
}

void Renew::SetRID(QString rid) {
    this->RID = rid;
}
