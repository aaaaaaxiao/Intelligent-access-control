#include "widget.h"
#include <QApplication>
#include <QSqlDatabase> //数据库类

QString access_token;
QSqlDatabase db;
int log_id;
QString face_token;

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);

    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}
