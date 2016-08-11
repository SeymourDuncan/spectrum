#include "myapplication.h"
#include <QQuickWindow>
#include <QQmlContext>



MyApplication::MyApplication(int& argc, char** argv,
            const QString& strOrg, const QString strAppname)
            : QApplication(argc, argv), m_pSettings(0){

    setOrganizationName(strOrg);
    setApplicationName(strAppname);
    // инициализируем объект настроек. TODO: менеджер настроек?
    m_pSettings = new QSettings(strOrg, strAppname, this);
    // грузим настройки подключения из реестра
    LoadConnectionSettings();
}

void MyApplication::Init(){
    m_pDBConnector->RunConnect();
}

QSettings* MyApplication::GetSettings(){
    return m_pSettings;
}

void MyApplication::LoadConnectionSettings(){
    m_pSettings->beginGroup("/Settings");
        m_pSettings->beginGroup("/Connection");
            m_pDBConnector = new DBConnector(m_pSettings->value("Host", "").toString(),
                                                         m_pSettings->value("Port", "").toString(),
                                                         m_pSettings->value("User", "").toString(),
                                                         m_pSettings->value("Password", "").toString(),
                                                         m_pSettings->value("Database", "").toString(),
                                                        this);
        m_pSettings->endGroup();
    m_pSettings->endGroup();
    QObject::connect(m_pDBConnector, SIGNAL(dataChanged()), SLOT(SaveConnectionSettings()));
}

void MyApplication::SaveConnectionSettings(){
    m_pSettings->beginGroup("/Settings");
        m_pSettings->beginGroup("/Connection");
            m_pSettings->setValue("Host", m_pDBConnector->Host());
            m_pSettings->setValue("Port", m_pDBConnector->Port());
            m_pSettings->setValue("User", m_pDBConnector->User());
            m_pSettings->setValue("Password", m_pDBConnector->Password());
            m_pSettings->setValue("Database", m_pDBConnector->Database());
        m_pSettings->endGroup();
    m_pSettings->endGroup();
}

void MyApplication::SetDefaultContext(){
    if (!m_pEngine)
        return;
    if (!m_pDBConnector)
        return;

    m_pEngine->rootContext()->setContextProperty("DBConnector", m_pDBConnector);
    m_pEngine->rootContext()->setContextProperty("ApplicationContext", this);
}

void MyApplication::SetQmlEngine(QQmlApplicationEngine* engine){
    m_pEngine = engine;   
    SetDefaultContext();
    // пока соединю тут контекст
//    QQuickWindow* item = qobject_cast<QQuickWindow *>(engine->rootObjects().first());
}


