#include "dbthread.h"
#include "constanthelper.h"
#include <QtSql/QSqlDatabase>

void DBThread::run(){
    if (QSqlDatabase::contains(ConstantsHelper::CONNECTION_NAME)){
        QSqlDatabase::removeDatabase(ConstantsHelper::CONNECTION_NAME);
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", ConstantsHelper::CONNECTION_NAME);

    db.setDatabaseName(m_Data.m_sDatabase);
    db.setUserName(m_Data.m_sUser);
    db.setPassword(m_Data.m_sPassword);
    db.setHostName(m_Data.m_sHost);
    db.setPort(m_Data.m_sPort.toInt());

    bool rez = db.open();
    emit fireResult(rez);
}
