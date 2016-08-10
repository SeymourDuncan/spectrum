#ifndef MYAPPLICATION_H
#define MYAPPLICATION_H

#include <QApplication>
#include <QSettings>
#include <QQmlApplicationEngine>
#include "connector.h"
#include "modeldata.h"

class MyApplication: public QApplication{
Q_OBJECT
private:
    QSettings* m_pSettings;
    DBConnector* m_pConnSettings;
    QQmlApplicationEngine* m_pEngine;
    DataModel* m_pModel;
public:
    MyApplication(int& argc, char** argv,
                  const QString& strOrg, const QString strAppname);

public:
    static MyApplication* TheApp(){
        return (MyApplication*)  qApp;
    }

    void SetQmlEngine(QQmlApplicationEngine*);
    void SetDefaultContext();
    QSettings* GetSettings();

public slots:
    void SaveConnectionSettings();

private:
   void LoadConnectionSettings();
};
#endif // MYAPPLICATION_H
