#include "modeldata.h"
#include "constanthelper.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QVariant>

#include <QDebug>

int DBEntity::GetID(){
    return m_dId;
}

QString DBEntity::GetName(){
    return m_sName;
}

void DataModel::SelectSystem(int index){
    m_dSelSys = index;
}

void DataModel::SelectClass(int index){
    m_dSelClass = index;
}

void DataModel::SelectObject(int index){
    m_dSelObj = index;
}

QStringList DataModel::GetSystemList(){
    QStringList lst;

    auto sv = GetSystemVector();

    if (sv.size() == 0)
        return lst;

    for(auto sys: GetSystemVector()){
        lst.append(sys->GetName());
    }

    return lst;
}

QStringList DataModel::GetClassesBySystem(){
    QStringList lst;

    if (m_dSelSys < 0)
        return lst;

    auto sv = GetSystemVector();

    if (sv.size() == 0)
        return lst;

    auto system = sv.at(m_dSelSys);

    if (system){
        auto clv = system->GetClassVector();

        for(auto cl: clv){
            lst.append(cl->GetName());
        }
    }

    return lst;
}

QStringList DataModel::GetObjectsByClass(){
    QStringList lst;

    if ((m_dSelSys < 0) || (m_dSelClass < 0))
        return lst;

    auto sv = GetSystemVector();

    if (sv.size() == 0)
        return lst;

    auto system = sv.at(m_dSelSys);

    auto clv = system->GetClassVector();
    if (clv.size() == 0)
        return lst;

    auto cl = clv.at(m_dSelClass);
    if (cl){
        auto objs = cl->GetObjVector();
        for(auto obj: objs){
            lst.append(obj->GetName());
        }
    }
    return lst;
}

QSpectrumValuesModel* DataModel::GetSpectrumValuesModel(){
    if ((m_dSelSys < 0) || (m_dSelClass < 0)  || (m_dSelObj < 0))
        return nullptr;

    auto sv = GetSystemVector();

    if (sv.size() == 0)
        return nullptr;

    auto system = sv.at(m_dSelSys);

    auto clv = system->GetClassVector();
    if (clv.size() == 0)
        return nullptr;

    auto cl = clv.at(m_dSelClass);

    auto objs = cl->GetObjVector();
    if (objs.size() == 0)
        return nullptr;

    SpectrumObject* obj = objs.at(m_dSelObj);

    auto model = obj->GetModel();

    return model;
}

bool DataModel::LoadData(){

    if(!QSqlDatabase::contains(ConstantsHelper::CONNECTION_NAME)){
        return false;
    }
    QSqlDatabase db = QSqlDatabase::database(ConstantsHelper::CONNECTION_NAME);

    QSqlQuery query(db);

    qDebug()<<ConstantsHelper::SELECT_SYSTEMS_QUERY;
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
        m_bIsLoaded = LoadData();
    }

    return m_vSpectrSystem;
}

bool SpectrumSystem::LoadData(){
    if(!QSqlDatabase::contains(ConstantsHelper::CONNECTION_NAME)){
        return false;
    }
    QSqlDatabase db = QSqlDatabase::database(ConstantsHelper::CONNECTION_NAME);
    QSqlQuery query(db);

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
        m_bIsLoaded = LoadData();
    }
    return m_vSpectrClass;
}

bool SpectrumClass::LoadData(){
    if(!QSqlDatabase::contains(ConstantsHelper::CONNECTION_NAME)){
        return false;
    }

    QSqlDatabase db = QSqlDatabase::database(ConstantsHelper::CONNECTION_NAME);
    QSqlQuery query(db);

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
        m_bIsLoaded = LoadData();
    }

    return m_vSpectrObj;
}

double SpectrumObject::GetKByL(const double K){
    return K;
}

QSpectrumValuesModel* SpectrumObject::GetModel(){
    if (!m_bIsLoaded){
        m_bIsLoaded = m_pModel->LoadData(GetID());
    }

    return m_pModel;
}

//

int QSpectrumValuesModel::rowCount(const QModelIndex &parent) const{
    return m_vL.size();
}

QVariant QSpectrumValuesModel::data(const QModelIndex &index, int role) const{
    if (!index.isValid()) {
        return QVariant();
    }

    int idx = index.row();
    switch (role) {
    case ValueLRole:
        return m_vL.at(idx);
    case ValueKRole:
        return m_vK.at(idx);
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> QSpectrumValuesModel::roleNames() const{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
    roles[ValueLRole] = "valueL";
    roles[ValueKRole] = "valueK";

    return roles;
}

bool QSpectrumValuesModel::LoadData(const int objId){
    m_vL.clear();
    m_vK.clear();

    if(!QSqlDatabase::contains(ConstantsHelper::CONNECTION_NAME)){
        return false;
    }

    QSqlDatabase db = QSqlDatabase::database(ConstantsHelper::CONNECTION_NAME);
    QSqlQuery query(db);

    query.prepare(ConstantsHelper::SELECT_SPECTRUM_QUERY);
    query.bindValue(":object_id", objId);

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
        //m_dqCount++;
    }

    return true;
}

QSpectrumValuesModel::QSpectrumValuesModel(QObject *parent): QAbstractListModel(parent){

}
