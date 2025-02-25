#include "passupdt.h"
#include "ui_passupdt.h"

PassUpdt::PassUpdt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PassUpdt)
{
    ui->setupUi(this);

    //语音播报
    speech = new QTextToSpeech(this);

    QIcon windowicon(":/1.jpg");
    this->setWindowIcon(windowicon);
    this->setWindowTitle("修改密码");

    //背景图
    QPixmap pix(":/2.jpg");
    QPixmap pic = pix.scaled(this->size());
    QPalette pal;
    pal.setBrush(QPalette::Background, QBrush(pic));
    this->setPalette(pal);

    lineEdit = new QLineEdit(this);
    lineEdit->move(180, 180);
    lineEdit->setEnabled(false);
    lineEdit = ui->lineEdit_old;

    query = new QSqlQuery(db);


}

PassUpdt::~PassUpdt()
{
    delete ui;
}

void PassUpdt::on_pushButton_1_clicked()
{
    lineEdit->setText(lineEdit->text()+"1");
}

void PassUpdt::on_pushButton_2_clicked()
{
    lineEdit->setText(lineEdit->text()+"2");
}

void PassUpdt::on_pushButton_3_clicked()
{
    lineEdit->setText(lineEdit->text()+"3");
}

void PassUpdt::on_pushButton_4_clicked()
{
    lineEdit->setText(lineEdit->text()+"4");
}

void PassUpdt::on_pushButton_5_clicked()
{
    lineEdit->setText(lineEdit->text()+"5");
}

void PassUpdt::on_pushButton_6_clicked()
{
    lineEdit->setText(lineEdit->text()+"6");
}

void PassUpdt::on_pushButton_7_clicked()
{
    lineEdit->setText(lineEdit->text()+"7");
}

void PassUpdt::on_pushButton_8_clicked()
{
    lineEdit->setText(lineEdit->text()+"8");
}

void PassUpdt::on_pushButton_9_clicked()
{
    lineEdit->setText(lineEdit->text()+"9");
}

void PassUpdt::on_pushButton_0_clicked()
{
    lineEdit->setText(lineEdit->text()+"0");
}

void PassUpdt::on_pushButton_ok_clicked()
{
    ui->lineEdit_new->setEnabled(true);
    ui->lineEdit_old->setEnabled(true);
    QString code_new = ui->lineEdit_new->text();
    QString code_old = ui->lineEdit_old->text();
    if(code_new.isEmpty()||code_old.isEmpty())
    {
        QMessageBox::warning(this,"密码更新","输入的新密码或旧密码不能为空");
        clearlineEdit(); //删除内容
        resetLineEdit();
        return;
    }
    //query->clear(); // 清空上一条查询的状态
    QString sql = "select * from user_code";

    if(!query->exec(sql))
    {
        QMessageBox::warning(this, "插入","Error: "+query->lastError().text());
        clearlineEdit(); //删除内容
        resetLineEdit();
        return;
    }
    QString old;
    while (query->next()) { //遍历查询的结果，结果中有几条记录循环几次
        //获取这条记录的每个值
        qDebug() << query->value(0).toString() << "  value";
        old = query->value(0).toString();
    }


    qDebug() << old << "  old";
    if(code_old != old)
    {
        QMessageBox::warning(this,"密码更新","输入的旧密码错误");
        clearlineEdit(); //删除内容
        resetLineEdit();
        return;
    }
    //query->clear(); // 清空上一条查询的状态
    QString sql1 = QString("update user_code SET passwd='%1' where passwd='%2' ")
            .arg(code_new).arg(code_old);
    if(!query->exec(sql1))
    {
        QMessageBox::warning(this, "插入","Error: "+query->lastError().text());
        clearlineEdit(); //删除内容
        resetLineEdit();
        return;
    }
    speech->say(up);
    QMessageBox::information(this,"修改密码","修改成功");

    clearlineEdit(); //删除内容
    resetLineEdit();
}

void PassUpdt::on_pushButton_delete_clicked()
{
    QString currentText = lineEdit->text();
    if (!currentText.isEmpty())
    {
        currentText.chop(1); //删除最后一位
        lineEdit->setText(currentText);
    }
}

void PassUpdt::on_pushButton_return_clicked()
{
    this->hide();
//    Widget *widget= new Widget;
//    widget->show();
}

void PassUpdt::on_pushButton_old_clicked()
{
    ui->lineEdit_old->setEnabled(true);
    ui->lineEdit_new->setEnabled(false);
    lineEdit = ui->lineEdit_old;
}

void PassUpdt::on_pushButton_new_clicked()
{
    ui->lineEdit_new->setEnabled(true);
    ui->lineEdit_old->setEnabled(false);
    lineEdit = ui->lineEdit_new;
}

void PassUpdt::clearlineEdit()
{
    //删除内容
    QString currentText = ui->lineEdit_new->text();
    currentText.remove(ui->lineEdit_new->text());  // 使用remove函数删除
    ui->lineEdit_new->setText(currentText);
    currentText = ui->lineEdit_old->text();
    currentText.remove(ui->lineEdit_old->text());  // 使用remove函数删除
    ui->lineEdit_old->setText(currentText);
}

void PassUpdt::resetLineEdit()
{
    ui->lineEdit_old->setEnabled(true);
    ui->lineEdit_new->setEnabled(false);
    lineEdit = ui->lineEdit_old;
}
