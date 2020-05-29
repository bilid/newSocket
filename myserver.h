#ifndef MYSERVER_H
#define MYSERVER_H
#include <QList>
#include <QTimer>
#include <QTcpServer>
#include <QTcpSocket>
#include  <systemonlist.h>
#include <sendmessagethread.h>
#include <QTimer>
class MyServer : public QTcpServer
{
    Q_OBJECT

public:
    // explicit MyServer(QObject *parent = 0);
    MyServer();

private:
    int flag=0;
    QByteArray data;
    SystemOnList *systemOnList;
    QTimer *timerOfUpdateOnList,*timerOfGetSendMessage,*timerOfSendMessage;
    bool messageOfFlag=false;
    QTcpSocket *sendsocket;
    QString socketText;
signals:


public slots:
public:
    virtual void incomingConnection(qintptr socketDescriptor);

};

#endif // MYSERVER_H
