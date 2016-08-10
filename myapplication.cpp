#include "myapplication.h"
#include <QQuickWindow>
#include <QQmlContext>


MyApplication::MyApplication(int& argc, char** argv,
            const QString& strOrg, const QString strAppname)
            : QApplication(argc, argv), m_pSettings(0){

    setOrganizationName(strOrg);
    setApplicationName(strAppname);

    m_pSettings = new QSettings(strOrg, strAppname, this);

    LoadConnectionSettings();

}

QSettings* MyApplication::GetSettings(){
    return m_pSettings;
}

void MyApplication::LoadConnectionSettings(){
    m_pSettings->beginGroup("/Settings");
        m_pSettings->beginGroup("/Connection");
            m_pConnSettings = new DBConnector(m_pSettings->value("Host", "").toString(),
                                                         m_pSettings->value("Port", "").toString(),
                                                         m_pSettings->value("User", "").toString(),
                                                         m_pSettings->value("Password", "").toString(),
                                                         m_pSettings->value("Database", "").toString(),
                                                        this);
        m_pSettings->endGroup();
    m_pSettings->endGroup();
    QObject::connect(m_pConnSettings, SIGNAL(dataChanged()), SLOT(SaveConnectorSettings()));
}

void MyApplication::SaveConnectionSettings(){
    m_pSettings->beginGroup("/Settings");
        m_pSettings->beginGroup("/Connection");
            m_pSettings->setValue("Host", m_pConnSettings->Host());
            m_pSettings->setValue("Port", m_pConnSettings->Port());
            m_pSettings->setValue("User", m_pConnSettings->User());
            m_pSettings->setValue("Password", m_pConnSettings->Password());
            m_pSettings->setValue("Database", m_pConnSettings->Database());
        m_pSettings->endGroup();
    m_pSettings->endGroup();
}

void MyApplication::SetDefaultContext(){
    if (!m_pEngine)
        return;
    if (!m_pConnSettings)
        return;

    m_pEngine->rootContext()->setContextProperty("ConnectionData", m_pConnSettings);
}

void MyApplication::SetQmlEngine(QQmlApplicationEngine* engine){
    m_pEngine = engine;

    SetDefaultContext();
    // пока соединю тут контекст
//    QQuickWindow* item = qobject_cast<QQuickWindow *>(engine->rootObjects().first());
}


