#include "widget.h"
#include "ui_widget.h"
#include "open.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    query = new QSqlQuery(db);

    //创建并打开SQlite数据库
    db = QSqlDatabase::addDatabase("QSQLITE");
    //设置数据库的库名
    db.setDatabaseName("user_code.db");
    //打开数据库
    if(!db.open())
    {
        qDebug()<<"database open: "<<db.lastError().text();//获取错误类里面的内容
        return;
    }
    query = new QSqlQuery(db);
    //创表
    QString sql = "create table if not exists user_code(passwd text)";
    if( !query->exec(sql))  //执行sql语句，判断创表是否成功
    {
        qDebug() << "create table: " << query->lastError().text();
        return;
    }

//    //设置初始密码
//    code = QString::number(1234);    //初始密码

//    //获取密码
//    sql = "select * from user_code";
//    if(!query->exec(sql))
//    {
//        QMessageBox::warning(this, "密码获取","Error: "+query->lastError().text());
//        return;
//    }
//    QString old_pswd;
//    while (query->next()) { //遍历查询的结果，结果中有几条记录循环几次
//        //获取这条记录的每个值
//        qDebug() << query->value(0).toString() << "  value";
//        old_pswd = query->value(0).toString();
//    }

//    if(old_pswd.isEmpty())
//    {
//        sql = QString("insert into user_code values('%1')")
//                .arg(code);
//        if(!query->exec(sql))   //执行sql语句
//        {
//            QMessageBox::warning(this,"初始密码设置","Error: "+query->lastError().text());
//            return;
//        }
//    }

    QIcon windowicon(":/1.jpg");
    this->setWindowIcon(windowicon);
    this->setWindowTitle("智能门禁");


    QPalette p = this->palette();
    p.setBrush(QPalette::Background,(QPixmap(":/2.jpg")));
    setPalette(p);

    QPixmap pixmap(":/3.jpg");
    ui->label->setPixmap(pixmap);
    ui->label->setScaledContents(true);

    //摄像头
//    camera.setLabel(ui->label);
//    camera.startCamera();

    //语音播报
    speech = new QTextToSpeech(this);

    //API和密钥
    QString api_key = "LOLPzePXUJ2j7wjfJZlDTMXQ";
    QString secret_key = "lt6F1mHtS9NNvm1wsqAb3ThWT5AaJpfa";

    //发送post请求，请求access_token
    //1. 初始化QNetworkAccessManager对象。
    manager = new QNetworkAccessManager(this);
    //定义post请求
    QNetworkRequest request;
    //2. 设置请求URL。
    QString url = QString("https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials&client_id=%1&client_secret=%2&")
            .arg(api_key).arg(secret_key);
    request.setUrl(QUrl(url));
    //3. 设置请求头Header
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    //4. 设置请求主体
    //5. 连接消息返回。当收到响应时触发finished信号
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(get_reply(QNetworkReply*)));
    //6. 发送POST请求。
    manager->post(request, "");   //request, body
}

Widget::~Widget()
{
    delete ui;
}

void Widget::RebackFacemessge(QNetworkReply *)
{

}

void Widget::get_reply(QNetworkReply *reply)
{
    //获取token，解析json格式
    QByteArray byte = reply->readAll();
    QByteArray bytearry= QString(byte).toUtf8();
    QJsonDocument document= QJsonDocument::fromJson(bytearry);
    if(document.isObject()){
        QJsonObject json = document.object();
        if(json.contains("access_token")){
            QJsonValue jsonval = json.value("access_token");
            access_token = jsonval.toString();
            qDebug() << "access_token = " << access_token;
        }
    }
}

void Widget::Baiduface()
{
    //搜索百度人脸库
    faceSearch = new QNetworkAccessManager;
    QString url = "https://aip.baidubce.com/rest/2.0/face/v3/search?access_token=";
    QNetworkRequest request;
    request.setUrl(QUrl(url+access_token));
    //设置头部
    request.setRawHeader("Content-Type", "application/json");
    //请求body参数
    QJsonObject json2;
    json2["image"] = ima;
    json2["image_type"] = "BASE64";
    json2["group_id_list"] = "1";
    json2["quality_control"] = "NORMAL";
    json2["match_threshold"] = 80;
    QByteArray data2 = QJsonDocument(json2).toJson();
    connect(faceSearch,&QNetworkAccessManager::finished,this,&Widget::RebackFaceSearch);
    faceSearch->post(request,data2);

}

QString Widget::ImageToString(const QImage &image)
{
    // 将 QImage 转换为 QString
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");  // 保存为 PNG 格式的字节数组
    return QString::fromLatin1(byteArray.toBase64());
}

void Widget::RebackFaceSearch(QNetworkReply *reply)
{
    QByteArray byte = reply->readAll();
    ui->textEdit->setText(byte);
    QByteArray bytearry = QString(byte).toUtf8();
    QJsonDocument docunment = QJsonDocument::fromJson(bytearry);
    if(docunment.isObject()){
        QJsonObject json = docunment.object();
        qDebug()<<"json:"<<json;
//        if(json.contains("log_id")){
//            log_id = json.value("log_id").toVariant().toULongLong();
//            qDebug() << "log_id = " << log_id;
//        }
        if(json.contains("error_msg")){
            QString data = json.value("error_msg").toString();
            if(data == "Open api daily request limit reached"){
                speech->say(open_err);
                QMessageBox::critical(this,"错误","今日识别次数已用完","确认");
                flag=false;
            }else if(data =="Open api request limit reached"){
                speech->say(open_err);
                QMessageBox::critical(this,"错误","请求速率过快，请重新识别","确认");
                flag=false;
            }else if(data =="Open api total request limit reached"){
                speech->say(open_err);
                QMessageBox::critical(this,"错误","免费次数已用完，请付费开通","确认");
                flag=false;
            }else if(data =="Invalid parameter"){
                speech->say(open_err);
                QMessageBox::critical(this,"错误","无效接口","确认");
                flag=false;
            }else if(data =="match user is not found"){
                speech->say(open_err);
                QMessageBox::critical(this,"错误","用户未注册，请注册后识别","确认");
                flag=false;
            }else if(data =="face is fuzzy"){
                speech->say(open_err);
                QMessageBox::critical(this,"错误","人脸模糊，请重新识别","确认");
                flag=false;
            }else if(data =="mouth is occlusion"){
                speech->say(open_err);
                QMessageBox::critical(this,"错误","人脸不存在，请注册","确认");
                flag=false;
            }else if(data =="chin contour is occlusion"){
                speech->say(open_err);
                QMessageBox::critical(this,"错误","请勿遮挡","确认");
                flag=false;
            }else if(data =="pic not has face"){
                speech->say(open_err);
                QMessageBox::critical(this,"错误","未检测到人脸，请重新识别","确认");
                flag=false;
            }else if(data =="nose is occlusion"){
                speech->say(open_err);
                QMessageBox::critical(this,"错误","请勿遮挡","确认");
                flag=false;
            }else if(data =="left cheek is occlusion"){
                speech->say(open_err);
                QMessageBox::critical(this,"错误","请勿遮挡","确认");
                flag=false;
            }


        }
//        if(flag == false)
//            return;
        if(json.contains("result")){
            QJsonObject obj = json.value("result").toObject();
            if(obj.contains("face_token")){
                face_token = obj.value("face_token").toString();
                qDebug() << "face_token = " << face_token;
            }
            if(obj.contains("user_list")){
                QJsonArray array = obj.value("user_list").toArray();
                for(int i=0;i<array.size();i++){
                    QJsonObject obj2 = array.at(i).toObject();
                    if(obj2.contains("user_id")){
                        user_id = obj2.value("user_id").toString();
                        qDebug() << "user_id = " << user_id;
                    }
                    if(obj2.contains("group_id")){
                        group_id = obj2.value("group_id").toString();
                        qDebug() << "group_id = " << group_id;
                    }
                    if(obj2.contains("score")){
                        double score = obj2.value("score").toDouble();
                        if(score<80){
                            qDebug()<<"识别失败";
                            speech->say(open_err);
                            QMessageBox::information(this,"人脸识别","识别失败","确认");
                            return;
                        }else {
                            qDebug()<<"识别成功";
                            speech->say(open_face);
                            QMessageBox::information(this,"人脸识别","识别成功","确认");
                            Open open;
                            if(open.exec() == QDialog::Accepted){

                            }
                        }
                    }
                }
            }
        }
    }

}

void Widget::on_faceBtn_clicked()
{
    image.fill(Qt::white);
    image = camera.screenShort();
    ima = ImageToString(image);
//    qDebug() << "ima" << ima;
    QPixmap pixmap = QPixmap::fromImage(image);
    ui->label_image->setPixmap(pixmap);
    //ui->label_image->setScaledContents(true);
    Baiduface();
}

void Widget::on_quitBtn_clicked()
{
    camera.stopCamera();
    this->close();
}

void Widget::on_addUserBtn_clicked()
{
    this->hide();
    Register reg;
    if(reg.exec() == QDialog::Accepted){

    }
}

void Widget::on_delBtn_clicked()
{
//    image.fill(Qt::white);
//    image = camera.screenShort();
//    ima = ImageToString(image);
//    QPixmap pixmap = QPixmap::fromImage(image);
//    ui->label_image->setPixmap(pixmap);
//    ui->label_image->setScaledContents(true);
//    Baiduface();
    Delface();
}

void Widget::Delface()
{
    delface = new QNetworkAccessManager;
    QString url = "https://aip.baidubce.com/rest/2.0/face/v3/faceset/face/delete?access_token=";
    QNetworkRequest request;
    request.setUrl(QUrl(url+access_token));
    //设置头部
    request.setRawHeader("Content-Type", "application/json");
    //请求body参数
    QJsonObject json3;
    json3["log_id"] = log_id;
    json3["user_id"] = user_id;
    json3["group_id"] = group_id;
    json3["face_token"] = face_token;
    QByteArray data3 = QJsonDocument(json3).toJson();
    connect(delface,&QNetworkAccessManager::finished,this,&Widget::RebackDelface);
    delface->post(request,data3);
}

void Widget::RebackDelface(QNetworkReply *reply)
{
    QByteArray byte = reply->readAll();
//    ui->textEdit->setText(byte);
    QByteArray bytearry = QString(byte).toUtf8();
    QJsonDocument docunment = QJsonDocument::fromJson(bytearry);
    if(docunment.isObject()){
        QJsonObject json = docunment.object();
        qDebug()<<"json:"<<json;
        if(json.contains("error_code")){
            if(json.value("error_code") == 0){
                 QMessageBox::information(this,"人脸删除","删除成功","确认");
                 qDebug() << "删除成功";
            }
            else
                QMessageBox::critical(this,"人脸删除","删除失败","确认");
        }
    }
}

void Widget::on_openBtn_clicked()
{
    QPixmap pixmap(":/3.jpg");
    ui->label->setPixmap(pixmap);
    ui->label->setScaledContents(true);
    ui->label->show();
    ui->label_image->show();
    //摄像头
    camera.setLabel(ui->label);
    camera.startCamera();
}

void Widget::on_closeBtn_clicked()
{
    camera.stopCamera();
    ui->label->hide();
    ui->label_image->hide();
//    ui->label->clear();

    QPixmap pixmap(":/3.jpg");
    ui->label->setPixmap(pixmap);
    ui->label->setScaledContents(true);
}

void Widget::on_passBtn_clicked()
{
    Pass *pass;
    pass = new Pass;
    pass->show();
}

void Widget::on_passBtn_2_clicked()
{
    PassUpdt *up;
    up = new PassUpdt;
    up->show();
}
