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

DataModel::DataModel(QObject* parent): QObject(parent), m_bIsLoaded(false),
        m_dSelSys(-1), m_dSelClass(-1), m_dSelObj(-1)
{
    m_pDataLoader = new DataLoader(this);
}

DataLoader* DataModel::GetDataLoader(){
    return m_pDataLoader;
}

void DataModel::SelectSystem(int index){
    m_dSelSys = index;
    m_pSelSys = nullptr;

    auto sv = GetSystemVector();
    if (sv.size() <= m_dSelSys)
        return;

    m_pSelSys = sv.at(m_dSelSys);
}

void DataModel::SelectClass(int index){
    m_dSelClass = index;
    m_pSelClass = nullptr;

    if (!m_pSelSys)
        return;

    auto cv = m_pSelSys->GetClassVector();
    if (cv.size() <= m_dSelClass)
        return;

    m_pSelClass = cv.at(m_dSelClass);

}

void DataModel::SelectObject(int index){
    m_dSelObj = index;
    m_pSelObj = nullptr;

    if (!m_pSelClass)
        return;

    auto ov = m_pSelClass->GetObjVector();
    if (ov.size() <= m_dSelObj)
        return;

    m_pSelObj = ov.at(m_dSelObj);
}

QStringList DataModel::GetSystemList(){
    QStringList lst;

    auto sv = GetSystemVector();

    if (sv.size() == 0)
        return lst;

    for(auto sys: sv){
        lst.append(sys->GetName());
    }

    return lst;
}

QStringList DataModel::GetClassList(){
    QStringList lst;

    if (!m_pSelSys)
        return lst;

    auto clv = m_pSelSys->GetClassVector();

    for(auto cl: clv){
        lst.append(cl->GetName());
    }

    return lst;
}

QStringList DataModel::GetObjectList(){
    QStringList lst;

    if (!m_pSelClass)
        return lst;

    auto ov = m_pSelClass->GetObjVector();
    for(auto obj: ov){
        lst.append(obj->GetName());
    }

    return lst;
}

QSpectrumValuesModel* DataModel::GetSpectrumValuesModel(){
    if (!m_pSelObj)
        return nullptr;

    auto model = m_pSelObj->GetModel();

    return model;
}

QVector<qreal> DataModel::GetLVector(){
    QVector<qreal> res;

    if (!m_pSelObj)
        return res;

    return m_pSelObj->GetModel()->GetLVector();
}

QVector<qreal> DataModel::GetKVector(){
    QVector<qreal> res;

    if (!m_pSelObj)
        return res;

    return m_pSelObj->GetModel()->GetKVector();
}

bool DataModel::LoadData(){

    if(!QSqlDatabase::contains(ConstantsHelper::CONNECTION_NAME)){
        return false;
    }
    QSqlDatabase db = QSqlDatabase::database(ConstantsHelper::CONNECTION_NAME);

    QSqlQuery query(db);

    qDebug()<<ConstantsHelper::SELECT_ALL_SYSTEMS_QUERY;
    if (!query.exec(ConstantsHelper::SELECT_ALL_SYSTEMS_QUERY)){
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

QVector<qreal> QSpectrumValuesModel::GetLVector(){
    return m_vL;
}

QVector<qreal> QSpectrumValuesModel::GetKVector(){
    return m_vK;
}
