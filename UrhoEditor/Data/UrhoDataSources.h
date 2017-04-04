#pragma once

#include <EditorLib/DataSource.h>
#include <EditorLib/Controls/DataGridWidget.h>

#include <Urho3D/Scene/Component.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Scene/Serializable.h>
#include <Urho3D/UI/UIElement.h>

namespace UrhoUI
{

    class SerializableDataSource : public DataSource
    {
    public:
        SerializableDataSource(Urho3D::Serializable* serial);

        SerializableDataSource(Urho3D::Node* node) { 
            serial_ = node;
        }

        SerializableDataSource(Urho3D::Component* comp) { 
            serial_ = comp;
        }

        virtual int GetID() const override { return 3; }
        virtual void Destroy() override;

        Urho3D::SharedPtr<Urho3D::Serializable> GetObject() { return serial_; }

        virtual QString GetName() const override;
        virtual QString GetTypeName() const override;

    protected:
        virtual bool equals(const DataSource* rhs) const override;

        Urho3D::SharedPtr<Urho3D::Serializable> serial_;
    };

    /// Handler for usage in the shared data-grid editor
    struct SerializableDataGridHandler : public DataGridWidget::DataSourceHandler
    {
        virtual bool CanHandleDataSource(std::shared_ptr<DataSource> dataSource) const override;
        virtual std::vector<std::string> GetColumns(std::shared_ptr<DataSource> dataSource) const override;
        virtual void FromText(std::shared_ptr<DataSource> dataSoruce, const std::string& column, const std::string& textValue) override;
        virtual QString ToText(std::shared_ptr<DataSource> dataSource, const std::string& column) const override;
    };


    class NodeDataSource : public SerializableDataSource
    {
    public:
        NodeDataSource(Urho3D::Node* node) : SerializableDataSource(node)
        {
        }

        virtual int GetID() const override { return 4; }
        
        Urho3D::Node* GetNode() const { return (Urho3D::Node*)serial_.Get(); }

        virtual QString GetName() const override;
    };

    class ComponentDataSource : public SerializableDataSource
    {
    public:
        ComponentDataSource(Urho3D::Component* component) : SerializableDataSource(component)
        {
        }

        virtual int GetID() const override { return 5; }

        Urho3D::Component* GetComponent() const { return (Urho3D::Component*)serial_.Get(); }

        virtual QString GetName() const override;
    };

    class UIDataSource : public SerializableDataSource
    {
    public:
        UIDataSource(Urho3D::UIElement* component) : SerializableDataSource(component)
        {
        }

        virtual int GetID() const override { return 6; }

        Urho3D::UIElement* GetElement() const { return (Urho3D::UIElement*)serial_.Get(); }

        virtual QString GetName() const override;
    };
}