#ifndef OPEN_H
#define OPEN_H

#include <QDialog>
#include <QMovie>
#include "widget.h"

namespace Ui {
class Open;
}

class Open : public QDialog
{
    Q_OBJECT

public:
    explicit Open(QWidget *parent = 0);
    ~Open();

private:
    Ui::Open *ui;
    QMovie *m;
};

#endif // OPEN_H
