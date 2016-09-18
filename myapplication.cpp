#include "myapplication.h"
#include <QQuickWindow>
#include <QQmlContext>

MyApplication::MyApplication(int& argc, char** argv,
            const QString& strOrg, const QString strAppname)
            : QApplication(argc, argv), m_pSettings(0), m_bDataLoaded(false){

    setOrganizationName(strOrg);
    setApplicationName(strAppname);
    // инициализируем объект настроек. TODO: менеджер настроек?
    m_pSettings = new QSettings(strOrg, strAppname, this);

    // грузим настройки подключения из реестра и создаем коннектор
    LoadConnectionSettings();
    // создаем модель данных
    m_pModel = new DataModel(this);
}

void MyApplication::Init(){
    m_pDBConnector->RunConnect();
}

DataModel* MyApplication::GetData(){
    if (!m_bDataLoaded){
        m_pModel->LoadData();
    }
    m_bDataLoaded = true;
    return m_pModel;
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

// для работы с подключением
    m_pEngine->rootContext()->setContextProperty("DBConnector", m_pDBConnector);
// для вызова startBuzy
    m_pEngine->rootContext()->setContextProperty("ApplicationContext", this);
//  данные из БД
    m_pEngine->rootContext()->setContextProperty("DataContainerContext", m_pModel);
}

void MyApplication::SetQmlEngine(QQmlApplicationEngine* engine){
    m_pEngine = engine;   
    SetDefaultContext();        
}


