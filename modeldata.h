#ifndef MODELDATA_H
#define MODELDATA_H

#include <QObject>
#include "dataloader.h"
#include <memory>
#include <QStringList>
#include <QAbstractListModel>


class SpectrumObject;
class SpectrumClass;
class SpectrumSystem;

typedef std::vector<SpectrumObject*> SpectrumObjectVector;
typedef std::vector<SpectrumClass*> SpectrumClassVector;
typedef std::vector<SpectrumSystem*> SpectrumSystemVector;


//
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

//
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

//
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

//
class QSpectrumValuesModel: public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles {
        ValueLRole = Qt::UserRole + 1,
        ValueKRole
    };

    QVector<qreal> m_vL;
    QVector<qreal> m_vK;

    QSpectrumValuesModel(QObject *parent = 0);

    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int, QByteArray> roleNames() const;

    QVector<qreal> GetLVector();
    QVector<qreal> GetKVector();

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

//
class DataModel: public QObject{
    Q_OBJECT
public:
    DataModel(QObject* parent = nullptr);
private:
    bool m_bIsLoaded;

    int m_dSelSys;
    int m_dSelClass;
    int m_dSelObj;

    SpectrumSystem* m_pSelSys;
    SpectrumClass* m_pSelClass;
    SpectrumObject* m_pSelObj;
    SpectrumSystemVector m_vSpectrSystem;

private:
    DataLoader* m_pDataLoader;
public:    
    // получение моделей
    Q_INVOKABLE QStringList GetSystemList();
    Q_INVOKABLE QStringList GetClassList();
    Q_INVOKABLE QStringList GetObjectList();

    // выделение
    Q_INVOKABLE void SelectSystem(int index);
    Q_INVOKABLE void SelectClass(int index);
    Q_INVOKABLE void SelectObject(int index);

    // для графика
    Q_INVOKABLE QVector<qreal> GetLVector();
    Q_INVOKABLE QVector<qreal> GetKVector();

    // загрузчик данных
    DataLoader* GetDataLoader();

    Q_INVOKABLE QSpectrumValuesModel* GetSpectrumValuesModel();
public:
     bool LoadData();
     SpectrumSystemVector GetSystemVector();
};




#endif // MODELDATA_H
