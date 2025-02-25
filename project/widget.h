#ifndef WIDGET_H
#define WIDGET_H

#include "camera.h"
#include "register.h"
#include "pass.h"
#include "passupdt.h"

#include <QVariant>

#include <QWidget>
#include <QIcon>
#include <QMovie>  //动画
#include <QDebug>
#include <QBuffer>
#include <QPixmap>
#include <QImage>
#include <QString>
#include <QByteArray>

#include <QSqlDatabase> //数据库类
#include <QSqlQuery>    //执行sql语句类
#include <QSqlError>    //sql错误信息类

#include <QTextToSpeech>    //语音
#include <QApplication>
#include <QVoice>

#include <QNetworkAccessManager> //网络访问管理类
#include <QNetworkRequest>  //网络请求
#include <QNetworkReply>  //网络响应

#include <QJsonDocument>  //Json文档
#include <QJsonObject>    //Json对象
#include <QJsonArray>     //Json数组
#include <QJsonValue>     //Json值

extern QSqlDatabase db;
extern int log_id;
extern QString access_token;
extern QString face_token;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:

    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void RebackFacemessge(QNetworkReply *);

    void get_reply(QNetworkReply *);

    void Baiduface();

    QString ImageToString(const QImage &image);  //将QImage转换为QSting

    void RebackFaceSearch(QNetworkReply *reply);

    void on_faceBtn_clicked();

    void on_quitBtn_clicked();

    void on_addUserBtn_clicked();

    void on_delBtn_clicked();

    void Delface();

    void RebackDelface(QNetworkReply *reply);

    void on_openBtn_clicked();

    void on_closeBtn_clicked();

    void on_passBtn_clicked();
    
    void on_passBtn_2_clicked();

private:
    Ui::Widget *ui;
    Camera camera;  //实例化camera对象

    QSqlQuery *query;
    QString code;
    QString sql;

    QNetworkAccessManager *manager;
    QNetworkAccessManager *http;
    QNetworkAccessManager *faceSearch;
    QNetworkAccessManager *delface;
    QString ima;
    QImage image;
    bool flag;
    QString user_id;
    QString group_id;

    QTextToSpeech *speech;
    QString open_face = "人脸识别成功，欢迎光临";
    QString open_err = "人脸识别失败，请重新识别";
};


#endif // WIDGET_H
