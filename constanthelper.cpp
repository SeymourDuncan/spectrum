#include "constanthelper.h"

QString ConstantsHelper::CONNECTION_NAME = "MainConnection";
QString ConstantsHelper::SELECT_SYSTEMS_QUERY = "Select * from system";
QString ConstantsHelper::SELECT_CLASSES_QUERY = "Select * from class where System = :system_id";
QString ConstantsHelper::SELECT_OBJECTS_QUERY = "Select * from object where Class = :class_id";
QString ConstantsHelper::SELECT_SPECTRUM_QUERY = "Select * from spectrumvalues where object = :object_id";
