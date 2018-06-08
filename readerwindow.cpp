#include "readerwindow.h"
#include "ui_readerwindow.h"

ReaderWindow::ReaderWindow(QString uid, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReaderWindow)
{
    ui->setupUi(this);
    this->UID = uid;
    connect(ui->borrowBookSearchEdit, SIGNAL(textChanged(QString)), SLOT(ShowBookInfo()));
    connect(ui->returnBookSearchEdit, SIGNAL(textChanged(QString)), SLOT(ShowBorrowedBooks()));
    connect(ui->logSearchEdit, SIGNAL(textChanged(QString)), SLOT(ShowLog()));
    connect(ui->ticketSearchEdit, SIGNAL(textChanged(QString)), SLOT(ShowTicket()));
    connect(ui->borrowBookButton, SIGNAL(clicked(bool)), SLOT(Borrow()));
    connect(ui->returnBookButton, SIGNAL(clicked(bool)), SLOT(Return()));
    connect(ui->renewBookButton, SIGNAL(clicked(bool)), SLOT(RenewBook()));
    connect(ui->payButtom, SIGNAL(clicked(bool)), SLOT(PayTicket()));
    ui->dateTimeEdit->setMinimumDateTime(QDateTime::currentDateTime());
    Refresh();
}

ReaderWindow::~ReaderWindow()
{
    delete ui;
}

void ReaderWindow::ShowBookInfo() {
    QString inputString = ui->borrowBookSearchEdit->text();
    QVector<QString> headerVector;
    headerVector.append("BookID");
    headerVector.append("BookName");
    headerVector.append("Author");
    headerVector.append("Price");
    headerVector.append("Mount");
    DataBase::ExecuteQueryModel(ui->bookTable, "select * from BOOK where BNAME like '%" + inputString + "%'", headerVector);
}

void ReaderWindow::ShowBorrowedBooks() {
    QString inputString = ui->returnBookSearchEdit->text();
    QString queryString = "select BORROW.BID, BNAME, BAUTHOR, BPRICE, BDATE, DDL "
                          "from BOOK, BORROW "
                          "where RID = " + this->UID + " "
                          "and BORROW.BID = BOOK.BID "
                          "and BNAME like '%" + inputString + "%'";
    qDebug() << queryString;
    QVector<QString> headerVector;
    headerVector.append("BookID");
    headerVector.append("BookName");
    headerVector.append("Author");
    headerVector.append("Price");
    headerVector.append("Date");
    headerVector.append("DeadLine");
    DataBase::ExecuteQueryModel(ui->borrowBookTable, queryString, headerVector);
}

void ReaderWindow::ShowLog() {
    QString inputString = ui->logSearchEdit->text();
    QVector<QString> headerVector;
    QString queryString = "select BOOK.BID, BNAME, DATE, ACTION "
                          "from BOOK, LOG "
                          "where BOOK.BID = LOG.BID "
                          "and RID = " + this->UID + " "
                          "and BNAME like '%" + inputString + "%'";
    qDebug() << queryString;
    headerVector.append("BookID");
    headerVector.append("BookName");
    headerVector.append("ActionDate");
    headerVector.append("ActionType");
    DataBase::ExecuteQueryModel(ui->logTable, queryString, headerVector);
}

void ReaderWindow::ShowTicket() {
    QVector<QString> headerVector;
    QString queryString = "select FID, FMOUNT, FDATE, HASPAID "
                          "from FINE "
                          "where RID = " + this->UID;
    headerVector.append("FineNumber");
    headerVector.append("FineMount");
    headerVector.append("FineDate");
    headerVector.append("HasPaid");
    DataBase::ExecuteQueryModel(ui->ticketTable, queryString, headerVector);
}

bool ReaderWindow::checkQualification() {
    //check if there should be a ticket
    int booksNum = 0; //sum of borrowed books
    int count = 0; //count valid books
    QVector<qint8> tagVector; //3 condition to borrow
    tagVector.append(0);
    tagVector.append(0);
    tagVector.append(0);
    QDateTime nowDateTime = QDateTime::currentDateTime();

    //check if borrowed books have reached MAXBORROWMOUNT
    QSqlQuery result(DataBase::ExecuteQuery("select COUNT(*) from BORROW where RID = " + this->UID));
    result.next();
    qDebug() << result.value(0);
    booksNum = result.value(0).toInt();
    if(booksNum >= MAXNBORROWMOUNT) {
        QMessageBox::information(this, "Warning", "Book number reached the upper limit");
    }
    else tagVector[0] = 1; // condition 1 meet
    result.clear();

    //check if borrowed books are valid
    result = DataBase::ExecuteQuery("select DDL from BORROW where RID = " + this->UID);
    while(result.next()) {
        qDebug() << result.value(0);
        if(result.value(0).toDateTime() <= nowDateTime) {
            QMessageBox::information(this, "Warning", "You have overdue books\nplease return them first");
            tagVector[1] = 0;
            break;
        }
        else count++;
    }
    if(count == booksNum) { // all the books are valid
        tagVector[1] = 1; // condition 2 meet
    }
    result.clear();

    //check if there's no ticket to pay
    result = DataBase::ExecuteQuery("select COUNT(*) from FINE where HASPAID = 'No' and RID = " + this->UID);
    result.next();
    if(result.value(0).toInt() != 0) { // unpaid ticket exist
        QMessageBox::information(this, "Warning", "Unpaid ticket exist");
        tagVector[2] = 0;
    }
    else tagVector[2] = 1;

    // check if 3 condition meet
    if(tagVector.at(0) == 1 && tagVector.at(1) == 1 && tagVector.at(2) == 1) {
        return true;
    }
    else return false;
}

void ReaderWindow::Borrow() {
    int currentRow;
    int mount;
    QString currentLID;
    QSqlQuery result;
    QString currentBID;
    QDateTime ddl;
    QString ddlString;

    if(checkQualification()) {
        // get current BID
        currentRow = ui->bookTable->currentIndex().row();
        QAbstractItemModel *model = ui->bookTable->model();
        QModelIndex indexBID = model->index(currentRow,0);
        currentBID = model->data(indexBID).toString();
        qDebug() << "currentBID:" + currentBID;

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

        //borrow
        result = DataBase::ExecuteQuery("select MOUNT,BNAME from BOOK where BID = " + currentBID);
        result.next();
        mount = result.value(0).toInt();
        if(mount > 0) {
            QMessageBox::StandardButton buttom =
                    QMessageBox::question(NULL,"Warning","sure to borrow:\n" + result.value(1).toString(),
                                          QMessageBox::Yes | QMessageBox::No);
            if(buttom == QMessageBox::Yes) {
                qDebug() << "Borrow!";
                //minus book mount
                DataBase::ExecuteQuery("update BOOK set MOUNT = " + QString::number(mount - 1) + " where BID = " + currentBID);

                //update borrow table
                DataBase::ExecuteQuery("insert into BORROW values(" + this->UID +"," + currentBID + ",now(),'" + ddlString + "')");

                //update log table
                DataBase::ExecuteQuery("insert into LOG values(" + currentLID + "," + this->UID + "," +
                                       currentBID + ",now(),'borrow')");
                Refresh();
            }
            else {
                qDebug() << "NO!";
            }
        }
        else {
            QMessageBox::information(this, "Warning", "this book is understock");
        }
    }
}

void ReaderWindow::Return() {
    int currentRow;
    bool isOverdue;
    QString bookPrice;
    QString currentLID;
    QString currentBID;
    QString currentFID;
    QSqlQuery result;

    //get currentBID
    currentRow = ui->borrowBookTable->currentIndex().row();
    QAbstractItemModel *model = ui->borrowBookTable->model();
    QModelIndex indexBID = model->index(currentRow,0);
    currentBID = model->data(indexBID).toString();
    qDebug() << "currentBID:" + currentBID;

    //get book price
    result = DataBase::ExecuteQuery("select BPRICE from BOOK where BID = " + currentBID);
    result.next();
    bookPrice = result.value(0).toString();
    result.clear();
    qDebug() << "bookPrice:" + bookPrice;

    //get currentFID
    result = DataBase::ExecuteQuery("select COUNT(*) from FINE");
    result.next();
    currentFID = QString::number(result.value(0).toInt() + 1);
    qDebug() << "currentFID:" + currentFID;
    result.clear();

    //get currentLID
    result = DataBase::ExecuteQuery("select COUNT(*) from LOG");
    result.next();
    currentLID = QString::number(result.value(0).toInt() + 1);
    qDebug() << "currentLID:" + currentLID;
    result.clear();

    //check if book is overdue
    result = DataBase::ExecuteQuery("select DDL from BORROW where RID = " + this->UID + " and BID = " + currentBID);
    result.next();
    if(result.value(0).toDateTime() < QDateTime::currentDateTime()) {
        qDebug() << "Overdue";
        isOverdue = true;
    }


    //start return
    QMessageBox::StandardButton buttom = QMessageBox::question(NULL,"Warning","is that book broken?", QMessageBox::Yes | QMessageBox::No);
    if(buttom == QMessageBox::Yes) { //book is broken
        DataBase::ExecuteQuery("insert into FINE values(" + currentFID + "," + this->UID + "," + bookPrice + ",now(),'No')");
        DataBase::ExecuteQuery("delete from BORROW where BID = " + currentBID + " and RID = " + this->UID);
        DataBase::ExecuteQuery("insert into LOG values(" + currentLID + "," + this->UID + "," + currentBID + ",now(),'broken')");
        QMessageBox::information(this, "Info", "pay your ticket please");
    }
    else { //return book
        DataBase::ExecuteQuery("update BOOK set MOUNT = MOUNT + 1 where BID = " + currentBID);
        DataBase::ExecuteQuery("delete from BORROW where BID = " + currentBID + " and RID = " + this->UID);
        DataBase::ExecuteQuery("insert into LOG values(" + currentLID + "," + this->UID + "," + currentBID + ",now(),'return')");
        if(isOverdue) {
            DataBase::ExecuteQuery("insert into FINE values(" + currentFID + "," + this->UID + ",1.00,now(),'No')");
        }
    }
    Refresh();
}

void ReaderWindow::RenewBook() {
    if(checkQualification()) {
        QString currentBID;
        int currentRow;
        //get currentBID
        currentRow = ui->borrowBookTable->currentIndex().row();
        QAbstractItemModel *model = ui->borrowBookTable->model();
        QModelIndex indexBID = model->index(currentRow,0);
        currentBID = model->data(indexBID).toString();
        qDebug() << "currentBID:" + currentBID;

        renew = new Renew();
        renew->SetBID(currentBID);
        renew->SetRID(this->UID);
        renew->show();
    }
}


void ReaderWindow::PayTicket() {
    int currentRow;
    QString currentFID;

    //get currentBID
    currentRow = ui->ticketTable->currentIndex().row();
    QAbstractItemModel *model = ui->ticketTable->model();
    QModelIndex indexFID = model->index(currentRow,0);
    currentFID = model->data(indexFID).toString();
    qDebug() << "currentFID:" + currentFID;

    //pay the ticket
    DataBase::ExecuteQuery("update FINE set HASPAID = 'Yes' where FID = " + currentFID);
    QMessageBox::information(this, "Info", "Pay Successfully");
    Refresh();
}

void ReaderWindow::Refresh() {
    ShowLog();
    ShowTicket();
    ShowBorrowedBooks();
    ShowBookInfo();
}
