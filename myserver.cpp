#include "myserver.h"
#include <QDebug>
#include <QDateTime>
#include  <systemonlist.h>
#include <receivthread.h>
#include <updateonlist.h>
MyServer::MyServer( )
{

    //根据单例对象更新OnList表
    timerOfUpdateOnList=new QTimer(this);
    connect(timerOfUpdateOnList,&QTimer::timeout,[=](){
        UpdateOnList *updateThread=new  UpdateOnList(this);
        connect(updateThread,&UpdateOnList::isDone,[=](){
            updateThread->quit();
            updateThread->wait();
            delete updateThread;
        });
        updateThread->start();
    });
    timerOfUpdateOnList->start(2000);
    //开辟线程 根据单例在线设备列表读取bs表 发送要改的
    timerOfGetSendMessage=new QTimer(this);
    connect(timerOfGetSendMessage,&QTimer::timeout,[=](){
        SendMessageThread  *sendMessageThread =new SendMessageThread(this);
        //因为tcpSocket不能在不同线程接收所以得用信号与槽接受数据
        connect(sendMessageThread,&SendMessageThread::sendMessage,[=](QTcpSocket *skt,QString message){

            //信号与槽是在一个线程里的所以不能在槽中定义发送对象，所以需要再定义一个定时器
            messageOfFlag=true;
            this->sendsocket=skt;
            this->socketText=message;
            qDebug()<<"要发送了";

        });
        //关闭线程函数
        connect(sendMessageThread,&SendMessageThread::isDone,[=](){
            sendMessageThread->quit();
            sendMessageThread->wait();
            delete sendMessageThread;
        });
        sendMessageThread->start();
    });
    timerOfGetSendMessage->start(1500);

    //    //发送message
    timerOfSendMessage=new QTimer(this);
    connect(timerOfSendMessage,&QTimer::timeout,[=](){
        //        this->sendsocket->write(this->socketText.toUtf8());
        if(messageOfFlag==true)
        {
            qDebug()<<this->socketText<<"*********"<<this->sendsocket->socketDescriptor();
            this->sendsocket->write(this->socketText.toUtf8());
            messageOfFlag=false;
        }

    });
    timerOfSendMessage->start(2000);



}
void MyServer::incomingConnection(qintptr socketDescriptor)
{

    QTcpSocket *socket =new QTcpSocket;//只要有新的socket传进来就新建一个socket对象
    socket->setSocketDescriptor(socketDescriptor);//设置当前socket的通信号
    connect(socket,&QTcpSocket::readyRead,[=](){//当有信息传入就开始读
        while(socket-> bytesAvailable()>0)//一个字节一个字节开始读
        {
            QByteArray buf = socket->read(1);
            if(!buf.isEmpty())
            {
                if(flag==0)
                {
                    if(buf[0]=='{')
                    {
                        flag=1;
                    }
                }
                else if(flag==1)
                {
                    if(buf[0]=='}')
                    {

                        flag=0;//清空标志位代表已经不在{}中
                        //在此处可以直接创建一个线程用来处理，处理接收的线程
                        ReceivThread *receivThread =new ReceivThread(0,data,socket);
                        connect(receivThread,&ReceivThread::isDone,[=](){
                            //线程处理函数函数处理完之后关闭线程

                            receivThread->quit();
                            receivThread->wait();
                            delete receivThread;
                        });
                        receivThread->start();
                        data.clear();//清除接收到的信息
                    }
                    else
                    {
                        data.append(buf[0]);
                    }
                }
            }
        }
    });
    connect(socket,&QTcpSocket::disconnected,[=](){
        systemOnList=SystemOnList::getInstance();
        for(int i=0;i<systemOnList->OLList.count();i++)
        {
            if(systemOnList->OLList.at(i).socket->socketDescriptor()==socket->socketDescriptor())
            {
                systemOnList->OLList.removeAt(i);
                i--;
            }
        }

    });
}


