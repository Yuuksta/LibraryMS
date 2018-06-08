#ifndef READERWINDOW_H
#define READERWINDOW_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSql>
#include "database.h"
#include "renew.h"

namespace Ui {
class ReaderWindow;
}

class ReaderWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ReaderWindow(QString uid, QWidget *parent = 0);
    ~ReaderWindow();
    void ShowAboutMe();
    bool checkQualification();

public slots:
    void ShowBookInfo();
    void ShowBorrowedBooks();
    void ShowLog();
    void ShowTicket();
    void Return();
    void RenewBook();
    void Borrow();
    void Refresh();
    void PayTicket();

private:
    Ui::ReaderWindow *ui;
    QString UID;
    Renew *renew;
};

#endif // READERWINDOW_H
