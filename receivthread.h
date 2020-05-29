#ifndef RECEIVTHREAD_H
#define RECEIVTHREAD_H
#include <QThread>
#include <QObject>
#include <systemonlist.h>
#include <QTcpSocket>
class ReceivThread : public QThread
{
    Q_OBJECT
public:
    explicit ReceivThread(QObject *parent ,    QByteArray data,QTcpSocket *sk);

signals:
 isDone();
public slots:
protected:
    virtual void run();
private:
     SystemOnList *systemOnList;
     QString kindIDAddressCome;
     QTcpSocket *socket;

};

#endif // RECEIVTHREAD_H
