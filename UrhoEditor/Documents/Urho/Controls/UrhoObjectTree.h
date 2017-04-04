#pragma once

#include <EditorLib/Controls/DragNotifiedTree.h>
#include <EditorLib/Selectron.h>

#include <Urho3D/Scene/Component.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Scene/Scene.h>

#include <vector>

namespace SprueEditor
{
    class UrhoEventHandler;
}

namespace UrhoUI
{

    /// General purpose tree that can work with Nodes or UI elements (plus whatever else ends up be decided
    class UrhoObjectTree : public DragNotifiedTree, public SelectronLinked
    {
        Q_OBJECT
    public:
        UrhoObjectTree(bool forUI);
        virtual ~UrhoObjectTree();

        void SetModel(Urho3D::Serializable* scene);

        static Urho3D::Serializable* GetSceneObject(QTreeWidgetItem*);
        Urho3D::Serializable* GetSelected() const;

        QTreeWidgetItem* GetItem(const Urho3D::Serializable* object);
        void UpdateItem(Urho3D::Serializable* object);

    protected:
        virtual void Link(Selectron* sel);

    protected slots:
        void ItemSelectionChanged();

    protected:
        virtual void contextMenuEvent(QContextMenuEvent*) Q_DECL_OVERRIDE;
        virtual void keyReleaseEvent(QKeyEvent* evt) Q_DECL_OVERRIDE;
        
        virtual bool CanDropOn(QTreeWidgetItem*, QTreeWidgetItem*) override;
        virtual void ItemDroppedOn(QTreeWidgetItem*, QTreeWidgetItem*) override;
        virtual void ItemDroppedAt(QTreeWidgetItem*, int, QTreeWidgetItem*) override;

    private:
        void Populate(QTreeWidgetItem* item, Urho3D::Object* object);
        void SetItemData(QTreeWidgetItem* item, Urho3D::Serializable* object) const;
        std::string GetTypeName(const Urho3D::StringHash& hash) const;

        bool isUI_;
        std::vector<QTreeWidgetItem*> roots_;
        SprueEditor::UrhoEventHandler* eventHandler_ = 0x0;
    };

    Q_DECLARE_METATYPE(Urho3D::Serializable*);

}