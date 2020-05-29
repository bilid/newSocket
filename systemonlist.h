#ifndef SYSTEMONLIST_H
#define SYSTEMONLIST_H
#include <QTcpSocket>

class SystemOnList
{
private:
    SystemOnList(){}//私有构造函数
    static SystemOnList *systemOnList;
public:
    static SystemOnList* getInstance()
    {
        if(systemOnList==nullptr)
            systemOnList=new SystemOnList();
        return systemOnList;
    }
    typedef struct  DeviceOL
    {
        QTcpSocket *socket;
        QString kindIDAddress;

    }DeviceOL;
    QList<DeviceOL> OLList;
};

#endif // SYSTEMONLIST_H
