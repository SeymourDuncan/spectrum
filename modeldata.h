#ifndef MODELDATA_H
#define MODELDATA_H

#include <QObject>
#include "modeldata.h"
#include <memory>
#include <QStringList>
#include <QAbstractListModel>

class SpectrumObject;
class SpectrumClass;
class SpectrumSystem;
class QSpectrumValuesModel;

typedef std::vector<SpectrumObject*> SpectrumObjectVector;
typedef std::vector<SpectrumClass*> SpectrumClassVector;
typedef std::vector<SpectrumSystem*> SpectrumSystemVector;

//typedef std::shared_ptr<SpectrumObject> centralwidget;

class DataModel: public QObject{
    Q_OBJECT
public:
    DataModel(QObject* parent = nullptr): QObject(parent), m_bIsLoaded(false),
    m_dSelSys(-1), m_dSelClass(-1), m_dSelObj(-1)
    {

    }
private:
    bool m_bIsLoaded;

    int m_dSelSys;
    int m_dSelClass;
    int m_dSelObj;
    SpectrumSystemVector m_vSpectrSystem;
public:
    // получение моделей
    Q_INVOKABLE QStringList GetSystemList();
    Q_INVOKABLE QStringList GetClassesBySystem();
    Q_INVOKABLE QStringList GetObjectsByClass();

    // выделение
    Q_INVOKABLE void SelectSystem(int index);
    Q_INVOKABLE void SelectClass(int index);
    Q_INVOKABLE void SelectObject(int index);

    Q_INVOKABLE QSpectrumValuesModel* GetSpectrumValuesModel();
public:
     bool LoadData();
     SpectrumSystemVector GetSystemVector();
};

class DBEntity: public QObject{
    Q_OBJECT
public:
    DBEntity(const int id, const QString name, QObject* parent = nullptr)
        : QObject(parent), m_dId(id), m_sName(name)
    {

    }
private:
    int m_dId;
    QString m_sName;

public:
    int GetID();
    QString GetName();
};

class SpectrumSystem: public DBEntity{
    Q_OBJECT
public:
    SpectrumSystem(const int id, const QString name, QObject* parent = nullptr)
        : DBEntity(id, name, parent), m_bIsLoaded(false)
    {

    }
private:
    bool m_bIsLoaded;
    SpectrumClassVector m_vSpectrClass;
public:
    bool LoadData();
    SpectrumClassVector GetClassVector();
};

class SpectrumClass: public DBEntity{
    Q_OBJECT
public:
    SpectrumClass(const int id, const QString name, QObject* parent = nullptr)
        : DBEntity(id, name, parent), m_bIsLoaded(false)
    {

    }
private:
    bool m_bIsLoaded;
    SpectrumObjectVector m_vSpectrObj;
public:
    bool LoadData();
    SpectrumObjectVector GetObjVector();
};

class QSpectrumValuesModel: public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles {
        ValueLRole = Qt::UserRole + 1,
        ValueKRole
    };

    std::vector<double> m_vL;
    std::vector<double> m_vK;

    QSpectrumValuesModel(QObject *parent = 0);

    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int, QByteArray> roleNames() const;

    bool LoadData(const int objId);
    //Q_INVOKABLE void add();

//private:
//    int m_dqCount;
};

//
class SpectrumObject: public DBEntity{
    Q_OBJECT
public:
    SpectrumObject(const int id, const QString name, QObject* parent = nullptr)
        : DBEntity(id, name, parent), m_bIsLoaded(false)
    {
        m_pModel = new QSpectrumValuesModel(this);
    }
private:    
    bool m_bIsLoaded  = false;
    QSpectrumValuesModel* m_pModel;
public:
    QSpectrumValuesModel* GetModel();
    double GetKByL(const double K);
};

#endif // MODELDATA_H
