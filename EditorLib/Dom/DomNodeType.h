#pragma once

#include <EditorLib/editorlib_global.h>
#include <EditorLib/Dom/DomAttribute.h>

#include <QString>

#include <memory>
#include <unordered_map>
#include <vector>
#include <functional>

class DomNodeType;
class IDomNode;

typedef std::function<std::shared_ptr<IDomNode>()> DomNodeFactory;

/// A DomNode may contain child nodes that are individual objects instead of lists.
/// DomNodeChildInfo contains additional information to assist in dealing with the attribute for the child.
struct EDITORLIB_EXPORT DomNodeChildInfo
{
    DomNodeChildInfo();
    ~DomNodeChildInfo();

    /// List of allowed types
    std::vector< std::shared_ptr<DomNodeType> > types_;
};

/// Indicates that a specific attribute is to be treated as an IDomNode
struct EDITORLIB_EXPORT DomNodeChildListInfo : public DomNodeChildInfo
{
    DomNodeChildListInfo();
    ~DomNodeChildListInfo();

    int minimumCount_ = 0;
    int maximumCount_ = INT_MAX;
};

/// Describes the nature of a DomNode object.
class EDITORLIB_EXPORT DomNodeType
{
public:
    DomNodeType(const QString& name, const std::vector< std::shared_ptr<DomAttributeType> >& attrs);
    virtual ~DomNodeType();

    QString GetTypeName() const { return typeName_; }

    /// Adds an attribute to this node type.
    void AddAttribute(std::shared_ptr<DomAttributeType> attr);
    /// Get the type (if available) for a specific attribute of this object.
    std::shared_ptr<DomAttributeType> GetAttribute(const QString& name) const;
    
    /// Get all of the attributes on the object.
    std::vector< std::shared_ptr<DomAttributeType> > GetAttributes() { return attributes_; }
    /// Get all of the attributes on the object.
    const std::vector< std::shared_ptr<DomAttributeType> > GetAttributes() const { return attributes_; }

private:
    QString typeName_;
    std::vector< std::shared_ptr<DomNodeType> > acceptedChildTypes_;
    std::vector< std::shared_ptr<DomAttributeType> > attributes_;
    std::unordered_map<std::string, std::shared_ptr<DomAttributeType> > attributesByName_;
};