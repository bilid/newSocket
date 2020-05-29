#ifndef UPDATEONLIST_H
#define UPDATEONLIST_H
#include <QThread>
#include <QObject>
#include  <systemonlist.h>
#include "connectionpool.h"
#include <QDateTime>
#include <QDebug>
class UpdateOnList : public QThread
{
    Q_OBJECT
public:
    explicit UpdateOnList(QObject *parent = 0);
signals:
    isDone();

protected:
    virtual void run();
private:
    SystemOnList *systemOnList;

public slots:
};

#endif // UPDATEONLIST_H
