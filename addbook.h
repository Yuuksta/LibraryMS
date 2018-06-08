#ifndef ADDBOOK_H
#define ADDBOOK_H

#include <QWidget>
#include "database.h"

namespace Ui {
class AddBook;
}

class AddBook : public QWidget
{
    Q_OBJECT

public:
    explicit AddBook(QWidget *parent = 0);
    ~AddBook();

public slots:
    void Confirm();

signals:
    void addOK();

private:
    Ui::AddBook *ui;
};

#endif // ADDBOOK_H
