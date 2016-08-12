#ifndef DBTHREAD_H
#define DBTHREAD_H
#include <QThread>
#include <connector.h>

class DBThread: public QThread{
Q_OBJECT
public:
    void run();
public:
    void setConnectionData(const ConnectionData& data){
       m_Data = data;
    }
private:
    ConnectionData m_Data;
signals:
    void fireResult(bool rez);
};
#endif // DBTHREAD_H
