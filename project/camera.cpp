#include "camera.h"
#include "ui_camera.h"

Camera::Camera(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Camera)
{
    ui->setupUi(this);

    camera = new QCamera(this);  //创建摄像头对象
    viewfinder = new QCameraViewfinder(this);  //创建摄像头视图对象
    imagecapture = new QCameraImageCapture(camera);  //设置图像捕获内容

}

Camera::~Camera()
{
    delete ui;
}

void Camera::setLabel(QLabel *label)
{
    viewfinder->resize(label->size());  //设置与label标签一样大
    viewfinder->setParent(label);  //设置摄像头内容到label
    viewfinder->show();

}

void Camera::startCamera()
{
    //设置摄像头对象到摄像头
    camera->setViewfinder(viewfinder);
    camera->setCaptureMode(QCamera::CaptureStillImage);
    imagecapture->capture();
    camera->start();
}

void Camera::stopCamera()
{
    camera->stop();
}

QImage Camera::screenShort()
{
    //获取摄像头当前的帧
    QImage image = viewfinder->grab().toImage();
    if(image.isNull()){
        QMessageBox::warning(this,"警告","请重新识别","确认");
    }
    return image;
}
