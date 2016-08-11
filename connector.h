#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QObject>
#include <QDebug>


struct ConnectionData{
    ConnectionData(const QString host, const QString port, const QString user, const QString password, const QString database)
        : m_sHost(host),
        m_sPort(port),
        m_sUser(user),
        m_sPassword(password),
        m_sDatabase(database)
    {

    }
    ConnectionData(){

    }
    QString m_sHost;
    QString m_sPort;
    QString m_sUser;
    QString m_sPassword;
    QString m_sDatabase;
};

class ConnectionWorker: public QObject{
Q_OBJECT
public:
    ConnectionWorker(const ConnectionData& data): QObject(nullptr) {
        m_Data = data;
    }
public slots:
    void process();

signals:
    void finished();
    void fireResult(bool rez);
public:
    ConnectionData m_Data;
};


class DBConnector: public QObject{
Q_OBJECT
    Q_PROPERTY(QString Host READ Host WRITE setHost)
    Q_PROPERTY(QString Port READ Port WRITE setPort)
    Q_PROPERTY(QString User READ User WRITE setUser)
    Q_PROPERTY(QString Password READ Password WRITE setPassword)
    Q_PROPERTY(QString Database READ Database WRITE setDatabase)
public:
    DBConnector(const QString host, const QString port, const QString user, const QString password, const QString database, QObject* parent = nullptr);
public:
    Q_INVOKABLE void tryConnect();
    void RunConnect();
public:
    QString Host() const;
    QString Port() const;
    QString User() const;
    QString Password() const;
    QString Database() const;
public:
    void setHost(const QString& value);
    void setPort(const QString& value);
    void setUser(const QString& value);
    void setPassword(const QString& value);
    void setDatabase(const QString& value);
private:
    bool m_bDataChanged;
    ConnectionData m_connData;
signals:
    void dataChanged();
    void connectionStatusChanged(bool val);
private slots:
    void obtainThreadResult(bool);
};

#endif // CONNECTOR_H
