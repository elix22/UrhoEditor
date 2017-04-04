#include "DomNodeType.h"

DomNodeChildInfo::DomNodeChildInfo()
{

}

DomNodeChildInfo::~DomNodeChildInfo()
{

}

DomNodeChildListInfo::DomNodeChildListInfo()
{

}

DomNodeChildListInfo::~DomNodeChildListInfo()
{

}

DomNodeType::DomNodeType(const QString& name, const std::vector< std::shared_ptr<DomAttributeType> >& attrs) :
    typeName_(name),
    attributes_(attrs)
{
    for (auto attr : attributes_)
        attributesByName_[attr->GetName().toStdString()] = attr;
}

DomNodeType::~DomNodeType()
{

}

void DomNodeType::AddAttribute(std::shared_ptr<DomAttributeType> attr)
{
    attributes_.push_back(attr);
    attributesByName_[attr->GetName().toStdString()] = attr;
}

std::shared_ptr<DomAttributeType> DomNodeType::GetAttribute(const QString& name) const
{
    auto found = attributesByName_.find(name.toStdString());
    if (found != attributesByName_.end())
        return found->second;
    return std::shared_ptr<DomAttributeType>();
}
