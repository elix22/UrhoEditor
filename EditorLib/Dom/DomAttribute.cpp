#include "DomAttribute.h"

DomAttributeType::DomAttributeType(const QString& name, QMetaType::Type type, QVariant defVal, unsigned flags, DomAttrValidator validator) :
    name_(name),
    type_(type),
    defaultValue_(defVal),
    flags_(flags),
    validator_(validator)
{

}