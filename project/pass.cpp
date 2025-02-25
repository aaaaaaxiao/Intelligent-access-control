#include "pass.h"
#include "ui_pass.h"

Pass::Pass(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Pass)
{
    ui->setupUi(this);

    QIcon windowicon(":/1.jpg");
    this->setWindowIcon(windowicon);
    this->setWindowTitle("密码开门");

    //背景图
    QPixmap pix(":/2.jpg");
    QPixmap pic = pix.scaled(this->size());
    QPalette pal;
    pal.setBrush(QPalette::Background, QBrush(pic));
    this->setPalette(pal);
    ui->lineEdit->setEchoMode(QLineEdit::Password);//密码显示模式

    query = new QSqlQuery(db);

    //语音播报
    // 创建文本到语音对象
    speech = new QTextToSpeech;
}

Pass::~Pass()
{
    delete ui;
}

//void Pass::timerEvent(QTimerEvent *event)
//{
//    xx = false;
//    if(s > 0)
//    {
//        ui->label_time->setText(QString::number(s));
//        s--;
//    }
//    else if(0 == s)
//    {
//        s = 60;
//        xx = true;
//        killTimer(timer);   //关闭定时器
//        key = 1;
//        ui->pushButton_return->setEnabled(true);
//        ui->pushButton_ok->setEnabled(true);
//        ui->label_time->setText("请再次输入!");
//    }
//    update();   //刷新
//}



void Pass::on_pushButton_1_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+"1");
}

void Pass::on_pushButton_2_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+"2");
}

void Pass::on_pushButton_3_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+"3");
}

void Pass::on_pushButton_4_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+"4");
}

void Pass::on_pushButton_5_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+"5");
}

void Pass::on_pushButton_6_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+"6");
}

void Pass::on_pushButton_7_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+"7");
}

void Pass::on_pushButton_8_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+"8");
}

void Pass::on_pushButton_9_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+"9");
}

void Pass::on_pushButton_0_clicked()
{
    ui->lineEdit->setText(ui->lineEdit->text()+"0");
}

void Pass::on_pushButton_ok_clicked()
{
    //获取密码
    QString sql = "select * from user_code";

    if(!query->exec(sql))
    {
        QMessageBox::warning(this, "密码获取","Error: "+query->lastError().text());
        return;
    }
    QString old_pswd;
    while (query->next()) { //遍历查询的结果，结果中有几条记录循环几次
        //获取这条记录的每个值
        qDebug() << query->value(0).toString() << "  value";
        old_pswd = query->value(0).toString();
    }


    QString pswd = ui->lineEdit->text();
    if(key >= 3) //输入次数>3次
    {
        QMessageBox::warning(this,"错误","输入密码错误次数大于三次，请再60s后重新输入");
        QString text4 = "输入密码错误次数大于三次，请再60秒后重新输入";// 设置要播报的文本
        speech->say(text4);// 播报文本
        timer = startTimer(1000);   //打开定时器
        ui->pushButton_return->setEnabled(false);
        ui->pushButton_ok->setEnabled(false);
        return;
    }
    if(pswd.isEmpty())
    {
        QMessageBox::warning(this,"错误","输入的账号与密码不能为空");
        key++;
        QString text3 = "输入的账号与密码不能为空！";// 设置要播报的文本
        speech->say(text3);// 播报文本
        return;
    }

    if(old_pswd == pswd)
    {
        QMessageBox::information(this,"解锁","密码正确！");
        // 设置要播报的文本
        Open open;
        if(open.exec() == QDialog::Accepted){

        }
        // 播报文本
        speech->say(open1);


    }
    else
    {
        QMessageBox::warning(this,"错误","密码输入错误");
        key++;
        //删除内容
        QString currentText = ui->lineEdit->text();
        currentText.remove(ui->lineEdit->text());  // 使用remove函数删除
        ui->lineEdit->setText(currentText);

        QString text2 = "密码错误！";// 设置要播报的文本
        speech->say(text2);// 播报文本
    }
}

void Pass::on_pushButton_delete_clicked()
{
    QString currentText = ui->lineEdit->text();
    if (!currentText.isEmpty()) {
        currentText.chop(1); //删除最后一位
        ui->lineEdit->setText(currentText);
    }
}

void Pass::on_pushButton_return_clicked()
{
    this->hide();
//    Widget *widget= new Widget;
//    widget->show();
}
