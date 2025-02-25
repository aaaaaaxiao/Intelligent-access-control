#ifndef PASSUPDT_H
#define PASSUPDT_H

#include <QWidget>

#include <QTextToSpeech>    //语音
#include <QApplication>
#include <QVoice>

#include <QWidget>
#include <QDebug>
#include <QLineEdit>
#include <QMessageBox>
#include <QSqlDatabase> //数据库类
#include <QSqlQuery>    //执行sql语句类
#include <QSqlError>    //sql错误信息类

#include "widget.h"

//extern QSqlQuery *query;
extern QSqlDatabase db;

namespace Ui {
class PassUpdt;
}

class PassUpdt : public QWidget
{
    Q_OBJECT

public:
    explicit PassUpdt(QWidget *parent = 0);
    ~PassUpdt();

private slots:
    void on_pushButton_1_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_0_clicked();

    void on_pushButton_ok_clicked();

    void on_pushButton_delete_clicked();

    void on_pushButton_return_clicked();

    void on_pushButton_old_clicked();

    void on_pushButton_new_clicked();

private:
    Ui::PassUpdt *ui;

    void clearlineEdit();
    void resetLineEdit();

    QLineEdit *lineEdit;
    QSqlQuery *query;
    //QSqlQuery *query2;

    QTextToSpeech *speech;
    QString up = "修改密码成功";
};

#endif // PASSUPDT_H
