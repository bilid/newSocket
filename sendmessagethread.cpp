#include "sendmessagethread.h"

SendMessageThread::SendMessageThread(QObject *parent)
{

}

void SendMessageThread::run()
{
    //数据库中插入收到的数据
    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);
    systemOnList=SystemOnList::getInstance();
    QString qs;//查询语句
    QString sendtext;
    QString kind,ID,address;
    //读取单例中的在线设备列表，轮询发送
    for(int i=0;i<systemOnList->OLList.count();i++)
    {
        kind=systemOnList->OLList.at(i).kindIDAddress.section(",",0,0);
        ID=systemOnList->OLList.at(i).kindIDAddress.section(",",1,1);
        address=systemOnList->OLList.at(i).kindIDAddress.section(",",2,2);
        qs=QString("select * from bs where kind = '%1' and ID = '%2'and flag = 's'and address = '%3'  order by  time desc limit 1").arg(kind).arg(ID).arg(address);
        query.exec(qs);
        if(query.next())
        {

            sendtext=QString("{%1,%2}"). arg(query.value("ID").toString() ).arg(query.value("send").toString());
            emit sendMessage(systemOnList->OLList.at(i).socket,sendtext);
            qs=QString("update bs set flag = 'c' where kind = '%1' and ID = '%2'").arg( kind).arg( ID);
            if(!query.exec(qs))
                qDebug()<<"没改成";
        }
    }
    ConnectionPool::closeConnection(db);
    emit isDone();
}
