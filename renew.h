#ifndef RENEW_H
#define RENEW_H

#include <QWidget>
#include "database.h"

namespace Ui {
class Renew;
}

class Renew : public QWidget
{
    Q_OBJECT

public:
    explicit Renew(QWidget *parent = 0);
    ~Renew();

public slots:
    void Confirm();
    void SetBID(QString bid);
    void SetRID(QString rid);

private:
    Ui::Renew *ui;
    QString BID;
    QString RID;
};

#endif // RENEW_H
