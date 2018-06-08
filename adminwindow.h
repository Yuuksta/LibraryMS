#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include "database.h"
#include "addbook.h"
#include "addreader.h"
#include "modifyreader.h"

namespace Ui {
class AdminWindow;
}

class AdminWindow : public QWidget
{
    Q_OBJECT

public:
    explicit AdminWindow(QString aid, QWidget *parent = 0);
    ~AdminWindow();

public slots:
    void ShowBookInfo();
    void ShowReaderInfo();
    void AddNewBook();
    void RemoveOldBook();
    void AddNewReader();
    void RemoveReader();
    void ModifyAReader();

private:
    Ui::AdminWindow *ui;
    QString AID;
    AddBook *addBook;
    AddReader *addReader;
    ModifyReader *modifyReader;
};

#endif // ADMINWINDOW_H
