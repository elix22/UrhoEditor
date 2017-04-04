#include "UrhoSceneCmds.h"

#include "../../UrhoEditor.h"

#include <EditorLib/Selectron.h>

using namespace Urho3D;

namespace UrhoUI
{

namespace SceneCommands
{

    AddComponentCmd::AddComponentCmd(Urho3D::Node* node, std::shared_ptr<SerializableDataSource> comp) :
        node_(node),
        comp_(comp)
    {
        SetText(QString("Add %1 to %2").arg(comp->GetObject()->GetTypeName().CString(), node->GetName().CString()));
    }

    void AddComponentCmd::Undo()
    {
        node_->RemoveComponent((Component*)comp_->GetObject().Get());
        SimpleSmartCommand::Undo();
    }

    void AddComponentCmd::Redo()
    {
        if (!IsFirstRedo())
            node_->AddComponent((Component*)comp_->GetObject().Get(), comp_->GetID(), Urho3D::CreateMode::LOCAL);
        SimpleSmartCommand::Redo();
    }

    void AddComponentCmd::MadeCurrent()
    {
        SprueEditor::UrhoEditor::GetInstance()->GetObjectSelectron()->NotifyDataChangedSoft(0x0, comp_);
    }

    DeleteComponentCmd::DeleteComponentCmd(Urho3D::Node* node, std::shared_ptr<SerializableDataSource> comp) :
        node_(node),
        comp_(comp)
    {
        SetText(QString("Delete %1 from %2").arg(comp->GetObject()->GetTypeName().CString(), node->GetName().CString()));
    }

    void DeleteComponentCmd::Undo()
    {
        node_->AddComponent((Component*)comp_->GetObject().Get(), comp_->GetID(), Urho3D::CreateMode::LOCAL);
        SprueEditor::UrhoEditor::GetInstance()->GetObjectSelectron()->NotifyDataChanged(0x0, comp_);
        SimpleSmartCommand::Undo();
    }

    void DeleteComponentCmd::Redo()
    {
        node_->RemoveComponent((Component*)comp_->GetObject().Get());
        SimpleSmartCommand::Redo();
    }

    void DeleteComponentCmd::MadeCurrent()
    {
        SprueEditor::UrhoEditor::GetInstance()->GetObjectSelectron()->NotifyDataChanged(0x0, 0x0);
    }

    MoveComponentOwnerCmd::MoveComponentOwnerCmd(Urho3D::Node* oldParent, Urho3D::Node* newParent, std::shared_ptr<SerializableDataSource> comp) :
        oldNode_(oldParent),
        newNode_(newParent),
        comp_(comp)
    {
        SetText(QString("Move %1 %2").arg(comp->GetObject()->GetTypeName().CString(), comp->GetID()));
    }

    void MoveComponentOwnerCmd::Undo()
    {
        newNode_->RemoveComponent((Component*)comp_->GetObject().Get());
        oldNode_->AddComponent((Component*)comp_->GetObject().Get(), comp_->GetID(), Urho3D::CreateMode::LOCAL);
        SimpleSmartCommand::Undo();
    }

    void MoveComponentOwnerCmd::Redo()
    {
        oldNode_->RemoveComponent((Component*)comp_->GetObject().Get());
        newNode_->AddComponent((Component*)comp_->GetObject().Get(), comp_->GetID(), Urho3D::CreateMode::LOCAL);
        SimpleSmartCommand::Redo();
    }

    void MoveComponentOwnerCmd::MadeCurrent()
    {
        SprueEditor::UrhoEditor::GetInstance()->GetObjectSelectron()->NotifyDataChanged(0x0, comp_);
    }

    AddNodeCmd::AddNodeCmd(Urho3D::Node* parent, std::shared_ptr<SerializableDataSource> child) :
        parent_(parent),
        self_(child)
    {
        SetText(QString("Add Node to %1").arg(parent->GetName().CString()));
    }

    void AddNodeCmd::Undo()
    {
        parent_->RemoveChild((Node*)self_->GetObject().Get());
        SimpleSmartCommand::Undo();
    }

    void AddNodeCmd::Redo()
    {
        if (!IsFirstRedo())
            parent_->AddChild((Node*)self_->GetObject().Get());
        SimpleSmartCommand::Redo();
    }

    void AddNodeCmd::MadeCurrent()
    {
        SprueEditor::UrhoEditor::GetInstance()->GetObjectSelectron()->NotifyDataChanged(0x0, self_);
    }

    DeleteNodeCmd::DeleteNodeCmd(Urho3D::Node* parent, std::shared_ptr<SerializableDataSource> self) :
        parent_(parent),
        self_(self)
    {
        SetText(QString("Delete node %1 from %2").arg(((Node*)self->GetObject().Get())->GetName().CString(), parent->GetName().CString()));
        for (unsigned i = 0; i < parent_->GetNumChildren(); ++i)
        {
            if (parent_->GetChild(i) == self->GetObject())
            {
                index_ = i;
                break;
            }
        }
    }

    void DeleteNodeCmd::Undo()
    {
        parent_->AddChild((Node*)self_->GetObject().Get(), index_);
        SprueEditor::UrhoEditor::GetInstance()->GetObjectSelectron()->NotifyDataChanged(0x0, self_);
        SimpleSmartCommand::Undo();
    }

    void DeleteNodeCmd::Redo()
    {
        parent_->RemoveChild((Node*)self_->GetObject().Get());
        SimpleSmartCommand::Redo();
    }

    void DeleteNodeCmd::MadeCurrent()
    {
        SprueEditor::UrhoEditor::GetInstance()->GetObjectSelectron()->NotifyDataChanged(0x0, 0x0);
    }

    MoveNodeCmd::MoveNodeCmd(Urho3D::Node* oldParent, Urho3D::Node* newParent, std::shared_ptr<SerializableDataSource> self) :
        oldParent_(oldParent),
        newParent_(newParent),
        self_(self)
    {
        SetText(QString("Move node %1 to node %2").arg(self->GetObject()->GetTypeName().CString(), newParent->GetName().CString()));
        for (unsigned i = 0; i < oldParent_->GetNumChildren(); ++i)
        {
            if (oldParent_->GetChild(i) == self->GetObject())
            {
                oldIndex_ = i;
                break;
            }
        }
    }
    void MoveNodeCmd::Undo()
    {
        newParent_->RemoveChild((Node*)self_->GetObject().Get());
        oldParent_->AddChild((Node*)self_->GetObject().Get(), oldIndex_);
        SimpleSmartCommand::Undo();
    }

    void MoveNodeCmd::Redo()
    {
        oldParent_->RemoveChild((Node*)self_->GetObject().Get());
        newParent_->AddChild((Node*)self_->GetObject().Get());
        SimpleSmartCommand::Redo();
    }

    void MoveNodeCmd::MadeCurrent()
    {
        SprueEditor::UrhoEditor::GetInstance()->GetObjectSelectron()->NotifyDataChanged(0x0, self_);
    }
}

}