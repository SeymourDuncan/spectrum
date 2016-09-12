#include "connector.h"
#include "constanthelper.h"
#include "myapplication.h"
#include "dbthread.h"

DBConnector::DBConnector(const QString host, const QString port, const QString user, const QString password, const QString database, QObject* parent/* = nullptr*/):
        m_connData(host, port, user, password, database),
        m_bDataChanged(false),
        QObject(parent){
}


void DBConnector::RunConnect(){
    DBThread* thread = new DBThread();
    thread->setConnectionData(m_connData);
    QObject::connect(thread, SIGNAL(started()), MyApplication::TheApp(), SIGNAL(startBusy()));
    QObject::connect(thread, SIGNAL(finished()), MyApplication::TheApp(), SIGNAL(finishBusy()));
    QObject::connect(thread, SIGNAL(fireResult(bool)), this, SLOT(obtainThreadResult(bool)));

    thread->start();
}

void DBConnector::obtainThreadResult(bool val){
    // получили результат от потока. Сообщаем всем
    emit connectionStatusChanged(val);
}

void DBConnector::tryConnect(){
    // если данные не менялись, коннект не делаем
    if (!m_bDataChanged)
        return;

    // сообщим что данные изменились
    emit dataChanged();

    m_bDataChanged = false;

    // выполняем подключение
    RunConnect();
}

QString DBConnector::Host() const{
    return m_connData.m_sHost;
}
QString DBConnector::Port() const{
    return m_connData.m_sPort;
}
QString DBConnector::User() const{
    return m_connData.m_sUser;
}
QString DBConnector::Password() const{
    return m_connData.m_sPassword;
}
QString DBConnector::Database() const{
    return m_connData.m_sDatabase;
}

void DBConnector::setHost(const QString& value){
    int diff = QString::compare(m_connData.m_sHost, value, Qt::CaseInsensitive);
    if (diff == 0){
        return;
    }
    m_connData.m_sHost = value;
    m_bDataChanged = true;

}
void DBConnector::setPort(const QString& value){
    int diff = QString::compare(m_connData.m_sPort, value, Qt::CaseInsensitive);
    if (diff == 0){
        return;
    }
    m_connData.m_sPort = value;
    m_bDataChanged = true;
}

void DBConnector::setUser(const QString& value){
    int diff = QString::compare(m_connData.m_sUser, value, Qt::CaseInsensitive);
    if (diff == 0){
        return;
    }
    m_connData.m_sUser = value;
    m_bDataChanged = true;
}
void DBConnector::setPassword(const QString& value){    
    int diff = QString::compare(m_connData.m_sPassword, value, Qt::CaseInsensitive);
    if (diff == 0){
        return;
    }
    m_connData.m_sPassword = value;
    m_bDataChanged = true;
}

void DBConnector::setDatabase(const QString& value){    
    int diff = QString::compare(m_connData.m_sDatabase, value, Qt::CaseInsensitive);
    if (diff == 0){
        return;
    }
    m_connData.m_sDatabase = value;
    m_bDataChanged = true;
}
