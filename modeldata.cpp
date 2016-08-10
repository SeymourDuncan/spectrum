#include "modeldata.h"
#include "constanthelper.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QVariant>

int DBEntity::GetID(){
    return m_dId;
}

QString DBEntity::GetName(){
    return m_sName;
}

bool DataModel::LoadData(){
    if(!QSqlDatabase::contains(ConstantsHelper::CONNECTION_NAME)){
        return false;
    }

    QSqlQuery query;

    if (!query.exec(ConstantsHelper::SELECT_SYSTEMS_QUERY)){
        return false;
    };

    QSqlRecord rec = query.record();
    int id;
    QString name;
    while(query.next()){
        id = query.value(rec.indexOf("ID")).toInt();
        name = query.value(rec.indexOf("NAME")).toString();
        SpectrumSystem* spSys = new SpectrumSystem(id, name, this);
        m_vSpectrSystem.push_back(spSys);
    }
    return true;
}

SpectrumSystemVector DataModel::GetSystemVector(){
    if (!m_bIsLoaded){
        LoadData();
        m_bIsLoaded = true;
    }

    return m_vSpectrSystem;
}

bool SpectrumSystem::LoadData(){
    if(!QSqlDatabase::contains(ConstantsHelper::CONNECTION_NAME)){
        return false;
    }

    QSqlQuery query;
    query.prepare(ConstantsHelper::SELECT_CLASSES_QUERY);
    query.bindValue(":system_id", GetID());

    if (!query.exec()){
        return false;
    };

    QSqlRecord rec = query.record();
    int id;
    QString name;
    while(query.next()){
        id = query.value(rec.indexOf("ID")).toInt();
        name = query.value(rec.indexOf("NAME")).toString();
        SpectrumClass* spClass = new SpectrumClass(id, name, this);
        m_vSpectrClass.push_back(spClass);
    }
    return true;
}

SpectrumClassVector SpectrumSystem::GetClassVector(){
    if (!m_bIsLoaded){
        LoadData();
        m_bIsLoaded = true;
    }

    return m_vSpectrClass;
}

bool SpectrumClass::LoadData(){
    if(!QSqlDatabase::contains(ConstantsHelper::CONNECTION_NAME)){
        return false;
    }

    QSqlQuery query;
    query.prepare(ConstantsHelper::SELECT_OBJECTS_QUERY);
    query.bindValue(":class_id", GetID());

    if (!query.exec()){
        return false;
    };

    QSqlRecord rec = query.record();
    int id;
    QString name;
    while(query.next()){
        id = query.value(rec.indexOf("ID")).toInt();
        name = query.value(rec.indexOf("NAME")).toString();
        SpectrumObject* spObj = new SpectrumObject(id, name, this);
        m_vSpectrObj.push_back(spObj);
    }

    return true;
}

SpectrumObjectVector SpectrumClass::GetObjVector(){
    if (!m_bIsLoaded){
        LoadData();
        m_bIsLoaded = true;
    }

    return m_vSpectrObj;
}

bool SpectrumObject::LoadData(){
    if(!QSqlDatabase::contains(ConstantsHelper::CONNECTION_NAME)){
        return false;
    }

    QSqlQuery query;
    query.prepare(ConstantsHelper::SELECT_SPECTRUM_QUERY);
    query.bindValue(":object_id", GetID());

    if (!query.exec()){
        return false;
    };

    QSqlRecord rec = query.record();
    int id;
    double l;
    double k;
    while(query.next()){
        id = query.value(rec.indexOf("ID")).toInt();
        l = query.value(rec.indexOf("L")).toDouble();
        k = query.value(rec.indexOf("K")).toDouble();
        m_vL.push_back(l);
        m_vK.push_back(k);
        m_dCount++;
    }

    return true;
}

double SpectrumObject::GetKByL(const double K){
    return K;
}
