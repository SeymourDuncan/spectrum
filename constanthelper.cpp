#include "constanthelper.h"

QString ConstantsHelper::CONNECTION_NAME = "MainConnection";
QString ConstantsHelper::SELECT_ALL_SYSTEMS_QUERY = "Select * from SYSTEM";
QString ConstantsHelper::SELECT_ALL_CLASSES_QUERY = "Select * from CLASS";

QString ConstantsHelper::SELECT_CLASSES_QUERY = "Select * from CLASS where SYSTEM = :system_id";
QString ConstantsHelper::SELECT_OBJECTS_QUERY = "Select * from object where Class = :class_id";
QString ConstantsHelper::SELECT_SPECTRUM_QUERY = "Select * from spectrumvalues where object = :object_id";
QString ConstantsHelper::INSERT_OBJECT_QUERY = "Insert into OBJECT(NAME, CLASS_ID, SYSTEM_ID, COMMENT) values(:name, :class_id, :system_id, :comment)";
QString ConstantsHelper::INSERT_SPECTRUM_VALUE_QUERY = "Insert into SPECTRUM_VALUE(OBJ_ID, L_VAL, K_VAL) values(?, ?, ?)";
