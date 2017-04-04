#pragma once

#include <EditorLib/editorlib_global.h>
#include <EditorLib/Dom/DomNodeType.h>

#include <QVariant>

#include <memory>
#include <vector>

/// Interface for DomNode, types may implement this interface to function like a DomNode.
class EDITORLIB_EXPORT IDomNode
{
public:
    virtual std::shared_ptr<DomNodeType> GetType() const = 0;

    /// Enumerates the contained children.
    virtual std::vector< std::shared_ptr<IDomNode> >& GetChildren() = 0;
    /// Enumerates the contained children.
    virtual const std::vector< std::shared_ptr<IDomNode> >& GetChildren() const = 0;

    /// Returns the attribute info, default asks the type for it. Overrides may produce fields dynamically.
    virtual const std::vector< std::shared_ptr<DomAttributeType> >& GetAttributeTypes() const;

    /// Gets the current property values.
    virtual std::vector<QVariant>& GetAttributes() = 0;
    /// Gets the current property values.
    virtual const std::vector<QVariant>& GetAttributes() const = 0;

    virtual const std::vector<DomNodeChildInfo>& GetChildInfo() const = 0;
    virtual const std::vector<DomNodeChildListInfo>& GetChildListInfo() const = 0;
};

/// Generic data container patterned after the DomNode in SCE's ATF.
/// Fundamental base-type of most data exposed in editing controls and collections.
class EDITORLIB_EXPORT DomNode : public IDomNode
{
public:
    /// Construct an empty node for a dom type.
    DomNode(std::shared_ptr<DomNodeType> type);
    /// Destruct.
    virtual ~DomNode();

    std::shared_ptr<DomNodeType> GetType() const override { return type_; }

    std::vector< std::shared_ptr<IDomNode> >& GetChildren() override { return childNodes_; }
    const std::vector< std::shared_ptr<IDomNode> >& GetChildren() const override { return childNodes_; }

    std::vector<QVariant>& GetAttributes() override { return attributes_; }
    const std::vector<QVariant>& GetAttributes() const override { return attributes_; }

private:
    std::vector<QVariant> attributes_;
    std::vector< std::shared_ptr<IDomNode> > childNodes_;
    std::shared_ptr<DomNodeType> type_;
};
