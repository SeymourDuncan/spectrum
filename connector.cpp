#include "connector.h"
#include <QtSql/QSqlDatabase>
#include "constanthelper.h"

DBConnector::DBConnector(const QString host, const QString port, const QString user, const QString password, const QString database, QObject* parent/* = nullptr*/):
        m_sHost(host),
        m_sPort(port),
        m_sUser(user),
        m_sPassword(password),
        m_sDatabase(database),
        QObject(parent){

}

bool DBConnector::doConnect(){
    emit dataChanged();
    QSqlDatabase db =  QSqlDatabase::addDatabase("QMYSQL", ConstantsHelper::CONNECTION_NAME);
    db.setDatabaseName(m_sDatabase);
    db.setUserName(m_sUser);
    db.setPassword(m_sPassword);
    db.setHostName(m_sHost);
    db.setPort(m_sPort.toInt());

    if(!db.open()){
        emit connectionStatusChanged(false);
        return false;
    }

    emit connectionStatusChanged(true);
    return true;
}

QString DBConnector::Host() const{
    return m_sHost;
}
QString DBConnector::Port() const{
    return m_sPort;
}
QString DBConnector::User() const{
    return m_sUser;
}
QString DBConnector::Password() const{
    return m_sPassword;
}
QString DBConnector::Database() const{
    return m_sDatabase;
}

void DBConnector::setHost(const QString& value){
    m_sHost = value;
}
void DBConnector::setPort(const QString& value){
    m_sPort = value;
}
void DBConnector::setUser(const QString& value){
    m_sUser = value;
}
void DBConnector::setPassword(const QString& value){
    m_sPassword = value;
}
void DBConnector::setDatabase(const QString& value){
    m_sDatabase = value;
}
