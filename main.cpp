#include <QCoreApplication>
#include <myserver.h>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    MyServer *server=new MyServer();
    server->listen(QHostAddress::Any,65000);//9988为端口号
    if(server->isListening())
        qDebug()<<"正在监听";
    return a.exec();
}
