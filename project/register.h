#ifndef REGISTER_H
#define REGISTER_H

#include <QDialog>
#include "camera.h"
#include "widget.h"
#include <QNetworkAccessManager>

#include <QTextToSpeech>    //语音
#include <QApplication>
#include <QVoice>

extern int log_id;
extern QString face_token;

namespace Ui {
class Register;
}

class Register : public QDialog
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = 0);
    ~Register();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void RebackFacemessge(QNetworkReply *reply);

    QString ImageToString(const QImage &image);

private:
    Ui::Register *ui;

    QString token;
    Camera camera;  //实例化camera对象
    QNetworkAccessManager *http;
    QString imastring;
    QImage image;

    QTextToSpeech *speech;
    QString reg_face = "添加人脸成功";
};

#endif // REGISTER_H
