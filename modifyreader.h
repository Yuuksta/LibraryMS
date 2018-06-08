#ifndef MODIFYREADER_H
#define MODIFYREADER_H

#include <QWidget>
#include "database.h"
namespace Ui {
class ModifyReader;
}

class ModifyReader : public QWidget
{
    Q_OBJECT

public:
    explicit ModifyReader(QWidget *parent = 0);
    ~ModifyReader();
    void SetRID(int rid);
public slots:
    void Confirm();
    void Init();
signals:
    void modifyOK();

private:
    Ui::ModifyReader *ui;
    int selectedRID;
};

#endif // MODIFYREADER_H
