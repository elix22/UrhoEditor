#pragma once

#include <EditorLib/editorlib_global.h>

#include <QMetaType>
#include <QString>
#include <QVariant>

#include <functional>

typedef std::function<bool(const QVariant&)> DomAttrValidator;

/// An attribute type describes the intended nature of the data in a field of a DomNode.
class EDITORLIB_EXPORT DomAttributeType
{
public:
    DomAttributeType(const QString& name, QMetaType::Type type, QVariant defaultVal, unsigned flags, DomAttrValidator validator = 0x0);

    /// Returns the pretty name of this attribute.
    QString GetName() const { return name_; }
    /// If the type is none or VoidStar then it is assumed that there is ChildInfo or ChildListInfo that needs to be queried.
    QMetaType::Type GetType() const { return type_; }
    unsigned GetFlags() const { return flags_; }

private:
    QString name_;
    QMetaType::Type type_;
    QVariant defaultValue_;
    DomAttrValidator validator_;
    unsigned flags_ = 0;
};