#include "receivthread.h"
#include "connectionpool.h"
#include <QDateTime>
#include <QDebug>
ReceivThread::ReceivThread(QObject *parent,    QByteArray data,QTcpSocket *sk)
{


    this->kindIDAddressCome=data;
    this->socket=sk;

}

void ReceivThread::run()
{

    //更新单例在线设备表 但不能更新onlist数据库应为我不能来数据的时候才去更新！
    systemOnList=SystemOnList::getInstance();
    SystemOnList::DeviceOL deviceOL;
    deviceOL.socket=this->socket;//单例中添加socket
    deviceOL.kindIDAddress=this->kindIDAddressCome.section(",",0,2);
    //如果相同的设备号发来了信息就把在线列表中的设备号信息删除
    for(int i=0;i<systemOnList->OLList.size();i++)
    {
        if(deviceOL.socket->socketDescriptor()==systemOnList->OLList.at(i).socket->socketDescriptor()&&deviceOL.kindIDAddress==systemOnList->OLList.at(i).kindIDAddress)
        {
            systemOnList->OLList.removeAt(i);
            i--;
        }
    }
    systemOnList->OLList.append(deviceOL);

    //数据库中插入收到的数据
    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);
    QString kind =this->kindIDAddressCome.section(",",0,0);
    QString ID=this->kindIDAddressCome.section(",",1,1);
    QString address=this->kindIDAddressCome.section(",",2,2);
    QString come=this->kindIDAddressCome.section(",",3,3);
    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString sq=QString ("insert into bs(kind,ID,flag,come,time,address) values('%1','%2','c','%3','%4','%5')").arg(kind ).arg( ID).arg(come ).arg( currentTime).arg( address);
    if(!query.exec(sq))qDebug()<<"插入失败";
    ConnectionPool::closeConnection(db);
    emit isDone();
}
