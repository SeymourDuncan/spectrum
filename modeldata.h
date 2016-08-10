#ifndef MODELDATA_H
#define MODELDATA_H

#include <QObject>
#include "modeldata.h"
#include <memory>

class SpectrumObject;
class SpectrumClass;
class SpectrumSystem;

typedef std::vector<SpectrumObject*> SpectrumObjectVector;
typedef std::vector<SpectrumClass*> SpectrumClassVector;
typedef std::vector<SpectrumSystem*> SpectrumSystemVector;

//typedef std::shared_ptr<SpectrumObject> centralwidget;

class DataModel: public QObject{
    Q_OBJECT
public:
    DataModel(QObject* parent = nullptr): QObject(parent){

    }
private:
    bool m_bIsLoaded;
    SpectrumSystemVector m_vSpectrSystem;
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


class SpectrumObject: public DBEntity{
    Q_OBJECT
public:
    SpectrumObject(const int id, const QString name, QObject* parent = nullptr)
        : DBEntity(id, name, parent), m_bIsLoaded(false)
    {

    }
private:    
    int m_dCount;
    bool m_bIsLoaded  = false;
    std::vector<double> m_vL;
    std::vector<double> m_vK;
public:
    bool LoadData();
    double GetKByL(const double K);
};


#endif // MODELDATA_H
