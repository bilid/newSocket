#include "updateonlist.h"
#include <QDateTime>
UpdateOnList::UpdateOnList(QObject *parent)
{

}

void UpdateOnList::run()
{

    //数据库中插入收到的数据
    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);
    systemOnList=SystemOnList::getInstance();
    QString qs;//查询语句
    //1
    for(int i=0;i<systemOnList->OLList.count();i++)
    {
        QString kind =systemOnList->OLList.at(i).kindIDAddress.section(",",0,0);
        QString ID =systemOnList->OLList.at(i).kindIDAddress.section(",",1,1);
        QString address =systemOnList->OLList.at(i).kindIDAddress.section(",",2,2);
        qs=QString("delete FROM onlist WHERE kind = '%1'  AND ID = '%2' AND address = '%3'").arg(kind).arg(ID).arg(address);

        if(!query.exec(qs))qDebug()<<"新来的链接，或者插入失败";
    }
    //2
    qs=QString("update onlist set flag = '0'");
    if(!query.exec(qs))qDebug()<<"更新失败";
    //3
    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    for(int i=0;i<systemOnList->OLList.count();i++)
    {
        QString kind =systemOnList->OLList.at(i).kindIDAddress.section(",",0,0);
        QString ID =systemOnList->OLList.at(i).kindIDAddress.section(",",1,1);
        QString address =systemOnList->OLList.at(i).kindIDAddress.section(",",2,2);
        qs=QString ("insert into onlist(kind,ID,flag,time,address) values('%1','%2','1','%3','%4')").arg( kind).arg(ID ).arg(currentTime).arg(address);


        if(!query.exec(qs))qDebug()<<"插入失败";
    }
    ConnectionPool::closeConnection(db);
    emit isDone();
}
