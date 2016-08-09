#include "connector.h"

ConnectionSettings::ConnectionSettings(const QString host, const QString port, const QString user,
                       const QString password, const QString database, QObject* parent/* = nullptr*/):
        m_sHost(host),
        m_sPort(port),
        m_sUser(user),
        m_sPassword(password),
        m_sDatabase(database),
        QObject(parent){

}

bool ConnectionSettings::doConnect(){
    emit dataChanged();
    return true;
}

void ConnectionSettings::doDisconnect(){
    emit disconnect();
}


QString ConnectionSettings::Host() const{
    return m_sHost;
}
QString ConnectionSettings::Port() const{
    return m_sPort;
}
QString ConnectionSettings::User() const{
    return m_sUser;
}
QString ConnectionSettings::Password() const{
    return m_sPassword;
}
QString ConnectionSettings::Database() const{
    return m_sDatabase;
}

void ConnectionSettings::setHost(const QString& value){
    m_sHost = value;
}
void ConnectionSettings::setPort(const QString& value){
    m_sPort = value;
}
void ConnectionSettings::setUser(const QString& value){
    m_sUser = value;
}
void ConnectionSettings::setPassword(const QString& value){
    m_sPassword = value;
}
void ConnectionSettings::setDatabase(const QString& value){
    m_sDatabase = value;
}
