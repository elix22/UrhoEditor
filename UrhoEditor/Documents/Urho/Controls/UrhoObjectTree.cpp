#include "UrhoObjectTree.h"

#include "../../Data/UrhoDataSources.h"

#include "../../GlobalAccess.h"
#include "../../Commands/Urho/UrhoSceneCmds.h"
#include "../../UrhoHelpers.h"

#include "../../Views/RenderWidget.h"
#include "../../Views/ViewManager.h"

#include <EditorLib/DocumentBase.h>
#include <EditorLib/DocumentManager.h>
#include <EditorLib/Localization/Localizer.h>

#include <qevent.h>
#include <QHeaderView>
#include <QMenu>
#include <QItemDelegate>

#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/Object.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/Scene/Serializable.h>
#include <Urho3D/UI/UIElement.h>
#include <Urho3D/UI/UIEvents.h>

using namespace Urho3D;

namespace UrhoUI
{
    /// Forcibly paints all text in the column to that of the ForegroundRole data color. Work-around for styling issue.
    class UrhoTreeDelegate : public QItemDelegate
    {
    public:
        UrhoTreeDelegate(UrhoObjectTree* tree) : 
            QItemDelegate(0x0), 
            tree_(tree)
        {
        }

        void paint(QPainter* painter, const QStyleOptionViewItem& opts, const QModelIndex& index) const
        {
            QStyleOptionViewItem viewOpt(opts);
            QColor foreColor = index.data(Qt::ForegroundRole).value<QColor>();
            if (foreColor.isValid())
                viewOpt.palette.setColor(QPalette::HighlightedText, foreColor);
            QItemDelegate::paint(painter, viewOpt, index);
        }

        UrhoObjectTree* tree_;
    };



    // List of Urho3D object names, should eventually be moved to an external file (to support 3rd party)
    std::map<const char*, const char*> UrhoSceneObjectTypeNames = 
    {
        { "Scene", "Scene" },
        { "Node", "Node"},
        { "Script Instance", "ScriptInstance" },
        { "Script Instance", "Script" },
        { "Animation Controller", "AnimationController" },
        { "Camera", "Camera" },
        { "Collision Shape", "CollisionShape" },
        { "Collision Shape 2D", "CollisionShape2D" },
        { "Constraint", "Constraint" },
        { "Constraint 2D", "Constraint2D" },
        { "Crowd Agent", "CrowdAgent" },
        { "Crowd Manager", "CrowdManager" },
        { "Debug Renderer", "DebugRenderer" },
        { "Nav Area", "NavArea" },
        { "Navigable", "Navigable" },
        { "Navigation Mesh", "NavigationMesh" },
        { "Network Priority", "NetworkPriority" },
        { "Obstacle", "Obstacle" },
        { "Octree", "Octree" },
        { "Off Mesh Connection", "OffMeshConnection" },
        { "Physics World", "PhysicsWorld" },
        { "Physics World 2D", "PhysicsWorld2D" },
        { "Rigid Body", "RigidBody" },
        { "Smoothed Transform", "SmoothedTransform" },
        { "Sound Listener", "SoundListener" },
        { "Sound Source", "SoundSource" },
        { "Spline Path", "SplinePath" },
        { "Terrain", "Terrain" },
        { "Tile Map 2D", "TileMap2D" },
        { "Tile Map Layer 2D", "TileMapLayer2D" },
    };

    std::map<Urho3D::StringHash, const char*> UrhoIcons = 
    {
        { Urho3D::StringHash("Octree"), ":/Images/urho/NodeGrid.png" },
        { Urho3D::StringHash("DebugRenderer"), ":/Images/urho/NodeGrid.png" },
        { Urho3D::StringHash("PhysicsWorld"), ":/Images/urho/NodeGrid.png" },
        { Urho3D::StringHash("PhysicsWorld2D"), ":/Images/urho/NodeGrid.png" },


        { Urho3D::StringHash("Terrain"), ":/Images/urho/Terrain.png" },
        { Urho3D::StringHash("SplinePath"), ":/Images/urho/SplinePath.png" },
        { Urho3D::StringHash("ScriptInstance"), ":/Images/urho/Script.png" },
        { Urho3D::StringHash("AngelScriptInstance"), ":/Images/urho/Script.png" },
        { Urho3D::StringHash("LuaScriptInstance"), ":/Images/urho/Script.png" },

        { Urho3D::StringHash("Camera"), ":/Images/urho/Camera.png" },
        { Urho3D::StringHash("Light"), ":/Images/urho/Light.png" },
        { Urho3D::StringHash("Zone"), ":/Images/urho/Octree.png" },
        { Urho3D::StringHash("AnimationController"), ":/Images/urho/Timer.png" },
        { Urho3D::StringHash("NavigationMesh"), ":/Images/urho/NavMesh.png" },
        { Urho3D::StringHash("RibbonTrail"), ":/Images/urho/SplinePath.png" },
        { Urho3D::StringHash("AnimatedModel"), ":/Images/urho/AnimatedMesh.png" },
        { Urho3D::StringHash("StaticModel"), ":/Images/urho/Octree.png" },
        { Urho3D::StringHash("StaticModelGroup"), ":/Images/urho/Octree.png" },
        { Urho3D::StringHash("BillboardSet"), ":/Images/urho/Billboard.png" },
        { Urho3D::StringHash("CustomGeometry"), ":/Images/urho/Octree.png" },
        { Urho3D::StringHash("Skybox"), ":/Images/urho/StaticModel.png" },
        { Urho3D::StringHash("ParticleEmitter"), ":/Images/urho/ParticleEffect.png" },
        { Urho3D::StringHash("ParticleEmitter2D"), ":/Images/urho/ParticleEffect.png" },
        { Urho3D::StringHash("DecalSet"), ":/Images/urho/Billboard.png" },

        
        { Urho3D::StringHash("SoundSource"),   ":/Images/urho/SoundSource.png" },
        { Urho3D::StringHash("SoundSource3D"), ":/Images/urho/SoundSource.png" },
        { Urho3D::StringHash("SoundListener"), ":/Images/urho/SoundListener.png" },

        { Urho3D::StringHash("NetworkPriority"), ":/Images/urho/Network.png" },

        { Urho3D::StringHash("Text3D"), ":/Images/urho/Text.png" },

        { Urho3D::StringHash("Navigable"), ":/Images/urho/NavMesh.png" },
        { Urho3D::StringHash("NavigationMesh"), ":/Images/urho/NavMesh.png" },
        { Urho3D::StringHash("DynamicNavigationMesh"), ":/Images/urho/NavMesh.png" },
        { Urho3D::StringHash("CrowdAgent"), ":/Images/urho/CrowdAgent.png" },
        { Urho3D::StringHash("CrowdManager"), ":/Images/urho/Crowdmanager.png" },
        { Urho3D::StringHash("OffMeshConnection"), ":/Images/urho/connection.png" },
        { Urho3D::StringHash("NavArea"), ":/Images/urho/WindowBox.png" },
        { Urho3D::StringHash("Obstacle"), ":/Images/urho/Obstacle.png" },
        
        { Urho3D::StringHash("RigidBody"), ":/Images/urho/RigidBody.png" },
        { Urho3D::StringHash("CollisionShape"), ":/Images/urho/CollisionShape.png" },
        { Urho3D::StringHash("CollisionShape2D"), ":/Images/urho/CollisionShape.png" },
        { Urho3D::StringHash("RigidBody2D"), ":/Images/urho/RigidBody.png" },
        { Urho3D::StringHash("Constraint"), ":/Images/urho/Joint.png" },

        { Urho3D::StringHash("StaticSprite2D"), ":/Images/urho/Billboard.png" },
        { Urho3D::StringHash("AnimatedSprite2D"), ":/Images/urho/Billboard.png" },
        { Urho3D::StringHash("TileMap2D"), ":/Images/urho/Terrain.png" },
        
        { Urho3D::StringHash("CollisionBox2D"), ":/Images/urho/CollisionShape.png" },
        { Urho3D::StringHash("CollisionChain2D"), ":/Images/urho/CollisionShape.png" },
        { Urho3D::StringHash("CollisionCircle2D"), ":/Images/urho/CollisionShape.png" },
        { Urho3D::StringHash("CollisionPolygon2D"), ":/Images/urho/CollisionShape.png" },
        { Urho3D::StringHash("CollisionEdge2D"), ":/Images/urho/CollisionShape.png" },
        { Urho3D::StringHash("ConstraintDistance2D"), ":/Images/urho/Joint.png" },
        { Urho3D::StringHash("ConstraintFriction2D"), ":/Images/urho/Joint.png" },
        { Urho3D::StringHash("ConstraintGear2D"), ":/Images/urho/Joint.png" },
        { Urho3D::StringHash("ConstraintMotor2D"), ":/Images/urho/Joint.png" },
        { Urho3D::StringHash("ConstraintMouse2D"), ":/Images/urho/Joint.png" },
        { Urho3D::StringHash("ConstraintPrismatic2D"), ":/Images/urho/Joint.png" },
        { Urho3D::StringHash("ConstraintPulley2D"), ":/Images/urho/Joint.png" },
        { Urho3D::StringHash("ConstraintRevolute2D"), ":/Images/urho/Joint.png" },
        { Urho3D::StringHash("ConstraintRope2D"), ":/Images/urho/Joint.png" },
        { Urho3D::StringHash("ConstraintWeld2D"), ":/Images/urho/Joint.png" },
        { Urho3D::StringHash("ConstraintWheel2D"), ":/Images/urho/Joint.png" },

        { Urho3D::StringHash("UIElement"), ":/Images/urho/UIElement.png" },
        { Urho3D::StringHash("BorderImage"), ":/Images/urho/UIBorderImage.png" },
        { Urho3D::StringHash("Button"), ":/Images/urho/UIButton.png" },
        { Urho3D::StringHash("CheckBox"), ":/Images/urho/UICheckbox.png" },
        { Urho3D::StringHash("Cursor"), ":/Images/urho/UICursor.png" },
        { Urho3D::StringHash("DropDownList"), ":/Images/urho/UIDropDownList.png" },
        { Urho3D::StringHash("LineEdit"), ":/Images/urho/UILineEdit.png" },
        { Urho3D::StringHash("ListView"), ":/Images/urho/UIListView.png" },
        { Urho3D::StringHash("Menu"), ":/Images/urho/UIDropDownList.png" },
        { Urho3D::StringHash("ScrollBar"), ":/Images/urho/UIScrollBar.png" },
        { Urho3D::StringHash("ScrollView"), ":/Images/urho/UIScrollView.png" },
        { Urho3D::StringHash("Slider"), ":/Images/urho/UISlider.png" },
        { Urho3D::StringHash("Sprite"), ":/Images/urho/UISprite.png" },
        { Urho3D::StringHash("Text"), ":/Images/urho/UIText.png" },
        { Urho3D::StringHash("ToolTip"), ":/Images/urho/UIText.png" },
        { Urho3D::StringHash("View3D"), ":/Images/urho/UIWindow.png" },
        { Urho3D::StringHash("Window"), ":/Images/urho/UIWindow.png" },
        { Urho3D::StringHash("UI"), ":/Images/urho/UIRoot.png" },
        { Urho3D::StringHash("UIRoot"), ":/Images/urho/UIRoot.png" },
    };

    // List of standard UI element names, should eventually move to an external file and be loaded
    std::map<const char*, const char*> UrhoUIElementNames =
    {
        { "UI Element", "UIElement" },
        { "Border Image", "BorderImage" },
        { "Sprite", "Sprite" },
        { "Text", "Text" },
        { "Tool Tip", "ToolTip" },
        { "Scroll View", "ScrollView" },
        { "Button", "Button" },
        { "Menu", "Menu" },
        { "Drop Down List", "DropDownList" },
        { "Checkbox", "CheckBox" },
        { "Cursor", "Cursor" },
        { "Line Edit", "LineEdit" },
        { "Scroll Bar", "ScrollBar" },
        { "Slider", "Slider" },
        { "Window", "Window" },
        { "View 3D", "View3D" },
        { "List View", "ListView" },
    };

    Urho3D::Serializable* UrhoObjectTree::GetSceneObject(QTreeWidgetItem* item)
    {
        if (Serializable* object = item->data(0, Qt::UserRole).value<Serializable*>())
            return object;
        return 0x0;
    }

    UrhoObjectTree::UrhoObjectTree(bool forUI) :
        isUI_(forUI)
    {
        setAccessibleName("Urho3D Scene Tree");

        //this->setSelectionMode(QAbstractItemView::ExtendedSelection);
        connect(this, &QTreeWidget::itemSelectionChanged, this, &UrhoObjectTree::ItemSelectionChanged);

        setItemDelegateForColumn(0, new UrhoTreeDelegate(this));

        setDragEnabled(true);
        setDropIndicatorShown(true);
        setDragDropMode(QAbstractItemView::DragDropMode::InternalMove);

        setColumnCount(2);
        setColumnWidth(0, 80);
        if (QTreeWidgetItem* header = headerItem())
        {
            header->setText(0, "Name");
            header->setText(1, "Type");
            setColumnWidth(0, 240);
        }
        setRootIsDecorated(false);
    }

    UrhoObjectTree::~UrhoObjectTree()
    {

    }

    void UrhoObjectTree::SetModel(Serializable* mdl)
    {
        blockSignals(true);
        roots_.clear();
        clear();
        blockSignals(false);
        if (eventHandler_)
        {
            delete eventHandler_;
            eventHandler_ = 0x0;
        }

        if (mdl == 0x0)
            return;

        eventHandler_ = new SprueEditor::UrhoEventHandler(mdl->GetContext());
        if (Scene* scene = dynamic_cast<Scene*>(mdl))
        {
            eventHandler_->Subscribe(0x0, "NodeAdded", [=](StringHash hash, VariantMap& data) {
                Scene* scene = (Scene*)data[Urho3D::NodeAdded::P_SCENE].GetPtr();
                Node* parent = (Node*)data[Urho3D::NodeAdded::P_PARENT].GetPtr();
                Node* newNode = (Node*)data[Urho3D::NodeAdded::P_NODE].GetPtr();

                if (newNode->GetName().StartsWith("SECRET_"))
                    return;

                if (QTreeWidgetItem* item = GetItem(parent))
                {
                    QTreeWidgetItem* child = new QTreeWidgetItem(item);
                    SetItemData(child, newNode);
                    Populate(child, newNode);
                }
            });

            eventHandler_->Subscribe(0x0, "NodeRemoved", [=](StringHash hash, VariantMap& data) {
                Scene* scene = (Scene*)data[Urho3D::NodeRemoved::P_SCENE].GetPtr();
                Node* parent = (Node*)data[Urho3D::NodeRemoved::P_PARENT].GetPtr();
                Node* removedNode = (Node*)data[Urho3D::NodeRemoved::P_NODE].GetPtr();

                if (QTreeWidgetItem* item = GetItem(removedNode))
                {
                    if (item->parent())
                        item->parent()->removeChild(item);
                    delete item;
                }
            });

            eventHandler_->Subscribe(0x0, "ComponentAdded", [=](StringHash hash, VariantMap& data) {
                Node* node = (Node*)data[Urho3D::ComponentAdded::P_NODE].GetPtr();
                Component* comp = (Component*)data[Urho3D::ComponentAdded::P_COMPONENT].GetPtr();
                if (QTreeWidgetItem* item = GetItem(node))
                {
                    QTreeWidgetItem* child = new QTreeWidgetItem(item);
                    SetItemData(child, comp);
                }
            });

            eventHandler_->Subscribe(0x0, "ComponentRemoved", [=](StringHash hash, VariantMap& data) {
                Node* node = (Node*)data[Urho3D::ComponentRemoved::P_NODE].GetPtr();
                Component* comp = (Component*)data[Urho3D::ComponentRemoved::P_COMPONENT].GetPtr();
                if (QTreeWidgetItem* item = GetItem(comp))
                {
                    if (item->parent())
                        item->parent()->removeChild(item);
                    delete item;
                }
            });

            eventHandler_->Subscribe(0x0, "NodeNameChanged", [=](StringHash hash, VariantMap& data) {
                Node* node = (Node*)data[Urho3D::NodeNameChanged::P_NODE].GetPtr();
                if (QTreeWidgetItem* item = GetItem(node))
                {
                    if (node->GetName().StartsWith("SECRET_"))
                        delete item;
                    else
                        SetItemData(item, node);
                }
            });

            eventHandler_->Subscribe(0x0, "NodeEnabledChanged", [=](StringHash hash, VariantMap& data) {
                Node* node = (Node*)data[Urho3D::NodeEnabledChanged::P_NODE].GetPtr();
                if (QTreeWidgetItem* item = GetItem(node))
                    SetItemData(item, node);
            });

            eventHandler_->Subscribe(0x0, "ComponentEnabledChanged", [=](StringHash hash, VariantMap& data) {
                Component* comp = (Component*)data[Urho3D::ComponentEnabledChanged::P_COMPONENT].GetPtr();
                if (QTreeWidgetItem* item = GetItem(comp))
                    SetItemData(item, comp);
            });


        }
        else if (UIElement* ui = dynamic_cast<UIElement*>(mdl))
        {
            eventHandler_->Subscribe(0x0, "ElementAdded", [=](StringHash hash, VariantMap& data) {
                UIElement* parent = (UIElement*)data[Urho3D::ElementAdded::P_PARENT].GetPtr();
                UIElement* elem = (UIElement*)data[Urho3D::ElementAdded::P_ELEMENT].GetPtr();
                if (QTreeWidgetItem* item = GetItem(parent))
                {
                    QTreeWidgetItem* child = new QTreeWidgetItem(item);
                    SetItemData(child, elem);
                    Populate(child, elem);
                }
            });

            eventHandler_->Subscribe(0x0, "ElementRemoved", [=](StringHash hash, VariantMap& data) {
                UIElement* parent = (UIElement*)data[Urho3D::ElementRemoved::P_PARENT].GetPtr();
                UIElement* elem = (UIElement*)data[Urho3D::ElementRemoved::P_ELEMENT].GetPtr();
                if (QTreeWidgetItem* item = GetItem(elem))
                {
                    if (item->parent())
                        item->parent()->removeChild(item);
                    delete item;
                }
            });
        }

        QTreeWidgetItem* item = new QTreeWidgetItem();
        SetItemData(item, mdl);
        Populate(item, mdl);
        roots_.push_back(item);
        this->addTopLevelItem(item);
        item->setExpanded(true);
    }

    Serializable* UrhoObjectTree::GetSelected() const
    {
        if (currentItem())
            return GetSceneObject(currentItem());
        return 0x0;
    }

    void UrhoObjectTree::Populate(QTreeWidgetItem* item, Object* object)
    {
        while (item->childCount())
            item->removeChild(item->child(0));

        if (Node* node = dynamic_cast<Node*>(object))
        {
            for (auto child : node->GetComponents())
            {
                QTreeWidgetItem* childItem = new QTreeWidgetItem(item);
                SetItemData(childItem, child);
                Populate(childItem, child);
            }
            for (auto child : node->GetChildren())
            {
                if (child->GetName().StartsWith("SECRET_"))
                    continue;
                QTreeWidgetItem* childItem = new QTreeWidgetItem(item);
                SetItemData(childItem, child);
                Populate(childItem, child);
            }
        }
    }

    void UrhoObjectTree::SetItemData(QTreeWidgetItem* item, Serializable* object) const
    {
        if (Node* node = dynamic_cast<Node*>(object))
        {
            QString name = node->GetName().CString();
            QString typeName = GetTypeName(node->GetType()).c_str();
            QString idStr = std::to_string(node->GetID()).c_str();
            if (name.isEmpty())
                name = QString("<unnamed %1> (%2)").arg(typeName, idStr);
            else
                name = QString("%1 (%2)").arg(name, idStr);

            QVariant var;
            var.setValue<Serializable*>(object);
            item->setData(0, Qt::UserRole, var);
            if (node->GetID() >= 0x1000000)
                item->setIcon(0, QIcon(":/Images/urho/ReplicatedNode.png"));
            else
                item->setIcon(0, QIcon(":/Images/urho/StaticNode.png"));
            item->setText(0, name);

            if (!node->IsEnabledSelf())
                item->setData(0, Qt::ForegroundRole, QColor(Qt::red));//item->setTextColor(0, Qt::red);
            else
            {
                if (node->GetID() >= 0x1000000)
                    item->setData(0, Qt::ForegroundRole, QColor(Qt::cyan));//item->setTextColor(0, Qt::cyan);
                else
                    item->setData(0, Qt::ForegroundRole, QColor(Qt::white));//item->setTextColor(0, Qt::white);
            }

            if (typeName.isEmpty())
                item->setText(1, "Model");
            else
                item->setText(1, typeName);
        }
        else if (Component* comp = dynamic_cast<Component*>(object))
        {
            std::string name = comp->GetTypeName().CString();
            QVariant var;
            var.setValue<Serializable*>(object);
            item->setData(0, Qt::UserRole, var);
            auto foundIcon = UrhoIcons.find(comp->GetType());
            if (foundIcon != UrhoIcons.end())
                item->setIcon(0, QIcon(foundIcon->second));
            item->setText(0, name.c_str());
            item->setText(1, "Component");

            if (!comp->IsEnabled())
                item->setForeground(0, QBrush(Qt::red));
            else
                item->setForeground(0, QBrush(Qt::white));
        }
    }

    void UrhoObjectTree::contextMenuEvent(QContextMenuEvent* evt)
    {
        if (QTreeWidgetItem* item = itemAt(evt->pos()))
        {
            QMenu* menu = new QMenu(this);

            Serializable* obj = GetSceneObject(item);

            // if we're not top level
            if (std::find(roots_.begin(), roots_.end(), item) == roots_.end())
            {
                QAction* delAction = new QAction("Delete");
                menu->addAction(delAction);

                if (Node* node = dynamic_cast<Node*>(obj))
                {
                    connect(delAction, &QAction::triggered, [=] {
                        if (node->GetParent())
                        {
                            node->GetParent()->RemoveChild(node);
                            //if (item->parent())
                            //    item->parent()->removeChild(item);
                        }
                    });
                }
                else if (Component* comp = dynamic_cast<Component*>(obj))
                {
                    connect(delAction, &QAction::triggered, [=] {
                        if (comp->GetNode())
                        {
                            comp->GetNode()->RemoveComponent(comp);
                            //if (item->parent())
                            //    item->parent()->removeChild(item);
                        }
                    });
                }
            }

            if (Node* node = dynamic_cast<Node*>(obj))
            {
                menu->addSeparator();
                QAction* addReplicatedChild = new QAction(QIcon(":/Images/urho/ReplicatedNode.png"), "Add Replicated Child");
                QAction* addStaticChild = new QAction(QIcon(":/Images/urho/StaticNode.png"), "Add Static Child");
                menu->addAction(addReplicatedChild);
                menu->addAction(addStaticChild);
                menu->addSeparator();

                connect(addReplicatedChild, &QAction::triggered, [=](bool) {
                    Node* created = node->CreateChild();
                    PUSH_DOCUMENT_UNDO(new SceneCommands::AddNodeCmd(node, std::shared_ptr<NodeDataSource>(new NodeDataSource(created))));
                    //Populate(item, node);
                    item->setExpanded(true);
                });

                connect(addStaticChild, &QAction::triggered, [=](bool) {
                    Node* created = node->CreateChild("", Urho3D::LOCAL);
                    PUSH_DOCUMENT_UNDO(new SceneCommands::AddNodeCmd(node, std::shared_ptr<NodeDataSource>(new NodeDataSource(created))));
                    //Populate(item, node);
                    item->setExpanded(true);
                });

                auto context = node->GetContext();
                auto categories = context->GetObjectCategories();
                for (auto category : categories)
                {
                    std::vector<QAction*> actions;
                    QMenu* subMenu = new QMenu(category.first_.CString());
                    for (auto categoryItem : category.second_)
                    {
                        auto foundIcon = UrhoIcons.find(categoryItem);
                        QAction* addComp = 0x0;

                        if (foundIcon != UrhoIcons.end())
                            addComp = new QAction(QIcon(foundIcon->second), context->GetTypeName(categoryItem).CString());
                        else
                            addComp = new QAction(context->GetTypeName(categoryItem).CString());

                        connect(addComp, &QAction::triggered, [=](bool) {
                            Component* comp = node->CreateComponent(categoryItem);
                            PUSH_DOCUMENT_UNDO(new SceneCommands::AddComponentCmd(node, std::shared_ptr<ComponentDataSource>(new ComponentDataSource(comp))));
                            //Populate(item, node);
                            item->setExpanded(true);
                        });
                        actions.push_back(addComp);
                    }

                    std::sort(actions.begin(), actions.end(), [=](QAction* lhs, QAction* rhs) { return lhs->text().compare(rhs->text(), Qt::CaseInsensitive) < 0; });
                    for (auto action : actions)
                        subMenu->addAction(action);
                    menu->addMenu(subMenu);
                }
            }

            menu->exec(mapToGlobal(evt->pos()));
        }
    }

    void UrhoObjectTree::keyReleaseEvent(QKeyEvent* evt)
    {
        if (evt->key() == Qt::Key::Key_Delete || evt->key() == Qt::Key::Key_Backspace)
        {
            for (auto selected : selectedItems())
            {
                if (std::find(roots_.begin(), roots_.end(), selected) == roots_.end())
                {
                    if (Object* object = GetSceneObject(selected))
                    {
                        if (Node* node = dynamic_cast<Node*>(object))
                        {
                            PUSH_DOCUMENT_UNDO(new SceneCommands::DeleteNodeCmd(node->GetParent(), std::shared_ptr<NodeDataSource>(new NodeDataSource(node))));
                            //node->GetParent()->RemoveChild(node);
                            //selected->parent()->removeChild(selected);
                        }
                        else if (Component* comp = dynamic_cast<Component*>(object))
                        {
                            PUSH_DOCUMENT_UNDO(new SceneCommands::DeleteComponentCmd(comp->GetNode(), std::shared_ptr<ComponentDataSource>(new ComponentDataSource(comp))));
                            //comp->GetNode()->RemoveComponent(comp);
                            //selected->parent()->removeChild(selected);
                        }
                        else if (UIElement* ui = dynamic_cast<UIElement*>(object))
                        {
                            if (ui->GetParent())
                            {
                                //TODO
                                ui->GetParent()->RemoveChild(ui);
                                //selected->parent()->removeChild(selected);
                            }
                        }
                    }
                }
            }
        }
    }

    QTreeWidgetItem* UrhoObjectTree::GetItem(const Serializable* object)
    {
        QTreeWidgetItemIterator it(this);
        while (*it)
        {
            if (GetSceneObject(*it) == object)
                return *it;
            ++it;
        }
        return 0x0;
    }

    void UrhoObjectTree::UpdateItem(Serializable* object)
    {
        if (object != 0x0)
        {
            if (QTreeWidgetItem* found = GetItem(object))
                SetItemData(found, object);
        }
    }

    std::string UrhoObjectTree::GetTypeName(const Urho3D::StringHash& hash) const
    {
        if (!isUI_)
        {
            for (auto txt : UrhoSceneObjectTypeNames)
            {
                if (Urho3D::StringHash(txt.second) == hash)
                    return txt.first;
            }
        }
        else
        {
            for (auto txt : UrhoUIElementNames)
            {
                if (Urho3D::StringHash(txt.second) == hash)
                    return txt.first;
            }
        }
        return std::string();
    }

    bool UrhoObjectTree::CanDropOn(QTreeWidgetItem* parent, QTreeWidgetItem* child)
    {
        Urho3D::Serializable* p = GetSceneObject(parent);
        Urho3D::Serializable* c = GetSceneObject(child);
        if (dynamic_cast<Node*>(p))
        {
            if (dynamic_cast<Node*>(c))
                return true;
        }
        return false;
    }

    void UrhoObjectTree::ItemDroppedOn(QTreeWidgetItem* parent, QTreeWidgetItem* child)
    {
        Urho3D::Serializable* p = GetSceneObject(parent);
        Urho3D::Serializable* c = GetSceneObject(child);

        if (Node* parentNode = dynamic_cast<Node*>(p))
        {
            if (Node* childNode = dynamic_cast<Node*>(c))
                parentNode->AddChild(childNode);
        }
    }

    void UrhoObjectTree::ItemDroppedAt(QTreeWidgetItem* parent, int index, QTreeWidgetItem* child)
    {
        Urho3D::Serializable* p = GetSceneObject(parent);
        Urho3D::Serializable* c = GetSceneObject(child);

        if (Node* parentNode = dynamic_cast<Node*>(p))
        {
            if (Node* childNode = dynamic_cast<Node*>(c))
                parentNode->AddChild(childNode, index);
        }
    }

    void UrhoObjectTree::Link(Selectron* sel)
    {
        connect(sel, &Selectron::DataChanged, [=](void* src, Selectron* sel, unsigned propertyHash)
        {
            if (src == this)
                return;

            unsigned nameHash = Urho3D::StringHash("Name").Value();
            unsigned enabledHash = Urho3D::StringHash("Is Enabled").Value();
            if ((propertyHash == nameHash || propertyHash == enabledHash))
            {
                if (auto object = sel->GetMostRecentSelected<UrhoUI::SerializableDataSource>())
                    if (object->GetObject())
                        UpdateItem(object->GetObject());
            }
        });

        connect(sel, &Selectron::SelectionChanged, [=](void* src, Selectron* sel) {
            if (src == this)
                return;

            blockSignals(true);
            clearSelection();
            for (unsigned i = 0; i < sel->GetSelectedCount(); ++i)
            {
                if (auto obj = sel->GetSelected<SerializableDataSource>(i))
                {
                    if (QTreeWidgetItem* item = GetItem(obj->GetObject()))
                        item->setSelected(true);
                }
            }
            blockSignals(false);
        });
    }

    void UrhoObjectTree::ItemSelectionChanged()
    {
        auto selection = selectedItems();
        if (selection.size() > 0)
        {
            bool first = true;
            for (auto item : selection)
            {
                if (Urho3D::Serializable* object = item->data(0, Qt::UserRole).value<Serializable*>())
                {
                    if (first)
                    {
                        GetSelectron()->SetSelected(this, std::shared_ptr<DataSource>(new SerializableDataSource(object)));
                        first = false;
                    }
                    else
                        GetSelectron()->AddSelected(this, std::shared_ptr<DataSource>(new SerializableDataSource(object)));
                }
            }
        }
        else
            GetSelectron()->SetSelected(this, 0x0);
    }
}