#pragma once

#include <EditorLib/Controls/DragNotifiedTree.h>
#include <EditorLib/Adaptation/IAdaptableHost.h>

/// Adaptation tree widget for arbitrary display of hierarchies.
class EDITORLIB_EXPORT AdaptableTree : public DragNotifiedTree, public IAdaptableHost
{
public:
    AdaptableTree(const QUuid& hostID);
    virtual ~AdaptableTree();

    virtual QUuid GetHostID() const override { return hostID_; }

protected:
    /// DragNotifiedTree implementation
    virtual bool CanDropOn(QTreeWidgetItem* parent, QTreeWidgetItem* child);
    /// DragNotifiedTree implementation
    virtual void ItemDroppedOn(QTreeWidgetItem* parent, QTreeWidgetItem* child);
    /// DragNotifiedTree implementation
    virtual void ItemDroppedAt(QTreeWidgetItem* parent, int index, QTreeWidgetItem* child);

    QUuid hostID_;
};