#ifndef CAMERA_H
#define CAMERA_H

#include <QWidget>
#include <QCamera>   //摄像头信息
#include <QCameraImageCapture>  //图片捕获
#include <QCameraViewfinder>  //摄像头对象
#include <QLabel>
#include <QMessageBox>

namespace Ui {
class Camera;
}

class Camera : public QWidget
{
    Q_OBJECT

public:
    explicit Camera(QWidget *parent = 0);
    ~Camera();
    void setLabel(QLabel *label);
    void startCamera();
    void stopCamera();
    QImage screenShort();

private:
    Ui::Camera *ui;

    QCamera *camera;
    QCameraViewfinder *viewfinder;
    QCameraImageCapture *imagecapture;
};

#endif // CAMERA_H
