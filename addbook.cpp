#include "addbook.h"
#include "ui_addbook.h"

AddBook::AddBook(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddBook)
{
    ui->setupUi(this);
    connect(ui->confirmButtom, SIGNAL(clicked(bool)), SLOT(Confirm()));
}

AddBook::~AddBook()
{
    delete ui;
}

void AddBook::Confirm() {
    QString bookNameString = ui->bookNameEdit->text();
    QString authorString = ui->authorEdit->text();
    QString priceString = ui->priceEdit->text();
    QString mountString = ui->mountEdit->text();
    QString queryString = "";
    int maxNum;
    QSqlQuery result(DataBase::ExecuteQuery("select max(BID) from BOOK"));
    result.next();
    maxNum = result.value(0).toInt();
    maxNum++;
    queryString = "insert "
                  "into BOOK(BID,BNAME,BAUTHOR,BPRICE,MOUNT) "
                  "values(" + QString::number(maxNum) + ",'" + bookNameString + "','"
                            + authorString + "'," + priceString + ","
                            + mountString + ")";
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
