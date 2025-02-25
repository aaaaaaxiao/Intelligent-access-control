#ifndef PASS_H
#define PASS_H

#include <QWidget>

#include <QMessageBox>
#include <QThread>
#include <QTimer>
#include <QSqlDatabase> //数据库类
#include <QSqlQuery>    //执行sql语句类
#include <QSqlError>    //sql错误信息类

#include <QTextToSpeech>    //语音
#include <QApplication>
#include <QVoice>
#include <QDebug>

#include "widget.h"
#include "open.h"           //开门

extern QString code;
//extern QSqlQuery *query;
extern QSqlDatabase db;

namespace Ui {
class Pass;
}

class Pass : public QWidget
{
    Q_OBJECT

public:
    explicit Pass(QWidget *parent = 0);
    ~Pass();

private slots:
//    void timerEvent(QTimerEvent *event);

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

private:
    Ui::Pass *ui;

    QTextToSpeech *speech;

    QSqlQuery *query;
    int s = 60;
    bool xx = false;
    int timer;
    int key = 1;

    QString open1 = "密码正确，欢迎光临";
};

#endif // PASS_H
