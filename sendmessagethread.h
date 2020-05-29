#ifndef SENDMESSAGETHREAD_H
#define SENDMESSAGETHREAD_H
#include  <systemonlist.h>
#include <QObject>
#include <QThread>
#include "connectionpool.h"
#include <QDateTime>
#include <QDebug>
#include <QTcpSocket>
class SendMessageThread : public QThread
{
    Q_OBJECT
public:
    explicit SendMessageThread(QObject *parent = 0);

signals:
    void isDone();
    void sendMessage(QTcpSocket *sk,QString msg);
protected:
    virtual void run();
public slots:
private:
    SystemOnList *systemOnList;
    QTcpSocket *tcpSocket;
};

#endif // SENDMESSAGETHREAD_H
