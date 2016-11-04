#ifndef DATALOADER_H
#define DATALOADER_H
#include <QList>
#include <QVariant>
#include <QHash>


class DataLoader: public QObject{
    Q_OBJECT
public:
    DataLoader::DataLoader(QObject* prnt = nullptr);
public:
    Q_INVOKABLE void LoadFromCsvFiles(QVariant list);
private:
    QStringList m_Files;
};


struct ObjectRec{
    QString m_sObjName;
    int m_dClassId;
    int m_dSysId;
    QString m_sComment;
};

#endif // DATALOADER_H
