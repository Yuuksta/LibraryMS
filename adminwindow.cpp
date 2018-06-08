#include "adminwindow.h"
#include "ui_adminwindow.h"

AdminWindow::AdminWindow(QString aid, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdminWindow)
{
    ui->setupUi(this);
    this->AID = aid;
    addBook = new AddBook();
    addReader = new AddReader();
    modifyReader = new ModifyReader();
    connect(ui->bookSearchEdit, SIGNAL(textChanged(QString)), SLOT(ShowBookInfo()));
    connect(ui->readerSearchEdit, SIGNAL(textChanged(QString)), SLOT(ShowReaderInfo()));
    connect(ui->addBookButton, SIGNAL(clicked(bool)), SLOT(AddNewBook()));
    connect(ui->removeBookButton, SIGNAL(clicked(bool)), SLOT(RemoveOldBook()));
    connect(ui->addReaderButton, SIGNAL(clicked(bool)), SLOT(AddNewReader()));
    connect(ui->removeReaderButton, SIGNAL(clicked(bool)), SLOT(RemoveReader()));
    connect(ui->modifyReaderButton, SIGNAL(clicked(bool)), SLOT(ModifyAReader()));
    connect(addBook, SIGNAL(addOK()), SLOT(ShowBookInfo()));
    connect(addReader, SIGNAL(addOK()), SLOT(ShowReaderInfo()));
    connect(modifyReader, SIGNAL(modifyOK()), SLOT(ShowReaderInfo()));
    ShowBookInfo();
    ShowReaderInfo();
}

AdminWindow::~AdminWindow()
{
    delete ui;
}

void AdminWindow::ShowBookInfo() {
    QString inputString = ui->bookSearchEdit->text();
    QVector<QString> headerVector;
    headerVector.append("BookID");
    headerVector.append("BookName");
    headerVector.append("Author");
    headerVector.append("Price");
    headerVector.append("Mount");
    DataBase::ExecuteQueryModel(ui->bookTable, "select * from BOOK where BNAME like '%" + inputString + "%'", headerVector);
}

void AdminWindow::ShowReaderInfo() {
    QString inputString = ui->readerSearchEdit->text();
    QVector<QString> headerVector;
    headerVector.append("ID");
    headerVector.append("ReaderName");
    headerVector.append("Password");
    headerVector.append("Major");
    DataBase::ExecuteQueryModel(ui->readerTable, "select * from READER where RNAME like '%" + inputString + "%'", headerVector);
}

void AdminWindow::AddNewBook() {
    addBook->show();
}

void AdminWindow::RemoveOldBook() {
    int currentRow;
    QString currentBID;
    QString currentBookName;
    currentRow = ui->bookTable->currentIndex().row();
    QAbstractItemModel *model = ui->bookTable->model();
    QModelIndex indexBID = model->index(currentRow,0);
    QModelIndex indexBNAME = model->index(currentRow,1);
    currentBID = model->data(indexBID).toString();
    currentBookName = model->data(indexBNAME).toString();
    QMessageBox::StandardButton buttom = QMessageBox::question(NULL,"Warning","sure to remove book:\n" + currentBookName, QMessageBox::Yes | QMessageBox::No);
    if(buttom == QMessageBox::Yes) {
        DataBase::ExecuteQuery("delete from BOOK where BID = " + currentBID);
        ShowBookInfo();
    }
    else {
        qDebug() << "NO!";
    }
}

void AdminWindow::AddNewReader() {
    addReader->show();
}

void AdminWindow::RemoveReader() {
    int currentRow;
    QString currentRID;
    QString currentReaderName;
    currentRow = ui->readerTable->currentIndex().row();
    QAbstractItemModel *model = ui->readerTable->model();
    QModelIndex indexRID = model->index(currentRow,0);
    QModelIndex indexRNAME = model->index(currentRow,1);
    currentRID = model->data(indexRID).toString();
    currentReaderName = model->data(indexRNAME).toString();
    QMessageBox::StandardButton buttom = QMessageBox::question(NULL,"Warning","sure to remove reader:\n" + currentReaderName, QMessageBox::Yes | QMessageBox::No);
    if(buttom == QMessageBox::Yes) {
        DataBase::ExecuteQuery("delete from READER where RID = " + currentRID);
        ShowReaderInfo();
    }
    else {
        qDebug() << "NO!";
    }
}

void AdminWindow::ModifyAReader() {
    int currentRow;
    QString currentRID;
    currentRow = ui->readerTable->currentIndex().row();
    QAbstractItemModel *model = ui->readerTable->model();
    QModelIndex indexRID = model->index(currentRow,0);
    currentRID = model->data(indexRID).toString();
    modifyReader->SetRID(currentRID.toInt());
    modifyReader->Init();
    modifyReader->show();
    //qDebug() << currentRID;
}

