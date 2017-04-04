#include "DomNode.h"

#include "DomNodeType.h"

const std::vector< std::shared_ptr<DomAttributeType> >& IDomNode::GetAttributeTypes() const
{
    return GetType()->GetAttributes();
}

DomNode::DomNode(std::shared_ptr<DomNodeType> type) :
    type_(type)
{

}

DomNode::~DomNode()
{

}