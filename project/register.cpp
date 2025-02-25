#include "register.h"
#include "ui_register.h"

Register::Register(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);

    QIcon regicon(":/1.jpg");
    this->setWindowTitle("人脸录入");
    this->setWindowIcon(regicon);
    token = access_token;

    QPalette p = this->palette();
    p.setBrush(QPalette::Background,(QPixmap(":/2.jpg")));
    setPalette(p);

    camera.setLabel(ui->label);
    camera.startCamera();

    //语音播报
    speech = new QTextToSpeech(this);
}

Register::~Register()
{
    delete ui;
}

void Register::on_pushButton_clicked()
{
    image.fill(Qt::white);
    image = camera.screenShort();
    imastring = ImageToString(image);
    QPixmap pixmap = QPixmap::fromImage(image);
    ui->label_image->setPixmap(pixmap);
    //ui->label_image->setScaledContents(true);

    if(ui->lineEdit_name->text().isEmpty() || ui->lineEdit_port->text().isEmpty()){
        QMessageBox::warning(this,"警告","请输入账号或密码","确认");
        return;
    }
    bool Number = false;
    ui->lineEdit_name->text().toInt(&Number);
    if(Number == false){
        QMessageBox::warning(this,"非法操作","账号格式不正确，请重新输入","确认");
        return;
    }
    bool isNumber=false;
    ui->lineEdit_port->text().toInt(&isNumber);
    if(isNumber == false){
        QMessageBox::warning(this,"非法操作","密码格式不正确，请重新输入","确认");
        return;
    }

    http = new QNetworkAccessManager;
    //百度人脸识别注册url
    QString url = "https://aip.baidubce.com/rest/2.0/face/v3/faceset/user/add?access_token=";
    QNetworkRequest request;
    request.setUrl(QUrl(url+token));
    request.setRawHeader("Content-Type", "application/json");
    QJsonObject json;

    json["image"]=imastring;
    json["image_type"]="BASE64";
    json["group_id"]="1";
    json["user_id"]=QString("%1_%2").arg(ui->lineEdit_name->text()).arg(ui->lineEdit_port->text().toInt());
    json["quality_control"]="NORMAL";
    QByteArray data = QJsonDocument(json).toJson();
    connect(http,&QNetworkAccessManager::finished,this,&Register::RebackFacemessge);
    http->post(request,data);

//    this->accept();
}

void Register::on_pushButton_2_clicked()
{
    this->reject();
    Widget *w = new Widget;
    w->show();
}

void Register::RebackFacemessge(QNetworkReply *reply)
{
    //获取到信息，就清空用户输入的信息
    ui->lineEdit_name->clear();
    ui->lineEdit_port->clear();

    QByteArray byte = reply->readAll();
    ui->textEdit->setText(byte);
    QByteArray bytearry = QString(byte).toUtf8();
    QJsonDocument docunment = QJsonDocument::fromJson(bytearry);
    if(docunment.isObject()){
        QJsonObject json = docunment.object();
        qDebug()<<"json"<<json;
        if(json.contains("log_id")){
            log_id = json.value("log_id").toString().toUInt();
            qDebug() << "log_id = " << log_id;
        }
        if(json.contains("error_msg")){
            QString data = json.value("error_msg").toString();
            if(data == "SUCCESS"){
                QMessageBox::information(this,"注册","恭喜！人脸库添加成功");
                speech->say(reg_face);
                return;
            }else if(data == "param[user_id] format error"){
                QMessageBox::warning(this,"警告","用户名或账号格式不正确","确认");
                return;
            }else if(data == "param[quality_control]format error"){
                QMessageBox::critical(this,"错误","人脸质量过低，请重新注册");
                return;
            }else if(data == "add face fail"){
                QMessageBox::critical(this,"错误","人脸图片添加失败，请重新注册");
                return;
            }else if(data == "user is already exist"){
                QMessageBox::critical(this,"错误","用户已存在，请不要重复注册");
                return;
            }else {
                QMessageBox::critical(this,"错误","人脸库注册失败，请重新注册");
                return;
            }
        }
    }
}

QString Register::ImageToString(const QImage &image)
{
    // 将 QImage 转换为 QString
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");  // 保存为 PNG 格式的字节数组
    return QString::fromLatin1(byteArray.toBase64());
}
