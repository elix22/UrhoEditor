#include "AdaptableTree.h"

AdaptableTree::AdaptableTree(const QUuid& hostID) :
    DragNotifiedTree(),
    hostID_(hostID)
{

}

AdaptableTree::~AdaptableTree()
{

}

bool AdaptableTree::CanDropOn(QTreeWidgetItem* parent, QTreeWidgetItem* child)
{
    return false;
}

void AdaptableTree::ItemDroppedOn(QTreeWidgetItem* parent, QTreeWidgetItem* child)
{

}

void AdaptableTree::ItemDroppedAt(QTreeWidgetItem* parent, int index, QTreeWidgetItem* child)
{

}