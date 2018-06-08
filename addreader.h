#ifndef ADDREADER_H
#define ADDREADER_H

#include <QWidget>
#include "database.h"

namespace Ui {
class AddReader;
}

class AddReader : public QWidget
{
    Q_OBJECT

public:
    explicit AddReader(QWidget *parent = 0);
    ~AddReader();
public slots:
    void Confirm();
signals:
    void addOK();
private:
    Ui::AddReader *ui;
};

#endif // ADDREADER_H
