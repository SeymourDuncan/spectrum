#ifndef CONSTANTHELPER_H
#define CONSTANTHELPER_H

#include <QString>
class ConstantsHelper
{
public:
    static QString CONNECTION_NAME;
    static QString SELECT_SYSTEMS_QUERY;
    static QString SELECT_CLASSES_QUERY;
    static QString SELECT_OBJECTS_QUERY;
    static QString SELECT_SPECTRUM_QUERY;
    static QString SELECT_ALL_SYSTEMS_QUERY;
    static QString SELECT_ALL_CLASSES_QUERY;

    static QString INSERT_OBJECT_QUERY;
    static QString INSERT_SPECTRUM_VALUE_QUERY;
};

#endif // CONSTANTHELPER_H
