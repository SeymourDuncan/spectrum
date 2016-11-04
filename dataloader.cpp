#include "dataloader.h"
#include "QSqlDatabase"
#include "QSqlQuery"
#include "constanthelper.h"
#include "QFile"
#include "QDebug"
#include "QSqlError"
#include "QSqlRecord"
#include <QLocale>

void DataLoader::LoadFromCsvFiles(QVariant list){

//    qDebug()<< QLocale::system().decimalPoint();
//    qDebug()<< QLocale().name();

    m_Files = list.toStringList();
    if (m_Files.count() < 1)
        return;

    // файлы есть, можем грузить
    QHash<QString, int> systems;
    QHash<QString, int> classes;
    
    if(!QSqlDatabase::contains(ConstantsHelper::CONNECTION_NAME)){
        return;
    }
    
    QSqlDatabase db = QSqlDatabase::database(ConstantsHelper::CONNECTION_NAME);
    int id;
    QString name;
    QSqlQuery query(db);

    // qDebug()<<ConstantsHelper::SELECT_ALL_SYSTEMS_QUERY;
    // готовим systems
    if (!query.exec(ConstantsHelper::SELECT_ALL_SYSTEMS_QUERY)){
        qDebug()<< db.lastError();
        return;
    };
    QSqlRecord rec = query.record();
    while(query.next()){
        id = query.value(rec.indexOf("ID")).toInt();
        name = query.value(rec.indexOf("NAME")).toString();
        systems.insert(name, id);
    }

    // готовим classes
    // TODO класс может быть любой. это плохо
    if (!query.exec(ConstantsHelper::SELECT_ALL_CLASSES_QUERY)){
        qDebug()<< db.lastError();
        return;
    };
    while(query.next()){
        id = query.value(rec.indexOf("ID")).toInt();
        name = query.value(rec.indexOf("NAME")).toString();
        classes.insert(name, id);
    }

    ObjectRec objRec;
    QVariant lastId = 0;
    for (QString filename : m_Files){

        QFile file(filename.remove(0, 8));
        if (!file.open(QIODevice::ReadOnly)) {
           qDebug() << file.errorString();
           continue;
        }

        auto line = file.readLine().split(';');

        if (line.count() < 3) {
            qDebug() << "Wrong file header";
            continue;
         }

        for (QString key: systems.keys()){
            qDebug() << key;
        }
        objRec.m_dSysId = systems.value(QString(line.at(0)), 0);
        objRec.m_dClassId = classes.value(QString(line.at(1)), 0);
        objRec.m_sObjName = line.at(2);
        objRec.m_sComment = line.value(3, "");

        // проверяем на валидность
        if ((objRec.m_dSysId * objRec.m_dClassId == 0) || objRec.m_sObjName == "") {
            qDebug() << "Wrong file data : " << filename;
            continue;
        }
        // TODO чтение свойств

        query.clear();
        query.prepare(ConstantsHelper::INSERT_OBJECT_QUERY);
        query.bindValue(":name", objRec.m_sObjName);
        query.bindValue(":class_id", objRec.m_dClassId);
        query.bindValue(":system_id", objRec.m_dSysId);
        query.bindValue(":comment", objRec.m_sComment);

        if (!query.exec()){
            qDebug() << "Failed write object" << filename;
            continue;
        }

        // получаем last id
        lastId = query.lastInsertId();
        if (!lastId.isValid()){
            qDebug() << "Failed to obtain last id" << filename;
            continue;
        }

        // заполняем значения
        query.clear();
        query.prepare(ConstantsHelper::INSERT_SPECTRUM_VALUE_QUERY);

        QVariantList objIdList;
        QVariantList lList;
        QVariantList kList;
        while(!file.atEnd()) {
           QString line = QString(file.readLine());
           line.remove(line.length() - 2, 2); // /r/n
           auto vals = line.split(';');
           if(vals.count() < 2)
               continue;

           objIdList.append(lastId);
           lList.append(vals.at(0).toDouble());
           kList.append(vals.at(1).toDouble());
        }

        query.addBindValue(objIdList);
        query.addBindValue(lList);
        query.addBindValue(kList);

        if(!query.execBatch()){
            qDebug() << db.lastError();
            continue;
        }
    }
    qDebug() << "Obj writing completed";
}

DataLoader::DataLoader(QObject* prnt): QObject(prnt){

}
