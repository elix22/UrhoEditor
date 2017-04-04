#pragma once

#include "../../../Controls/BasePropertyPage.h"
#include "UrhoBaseTypeEditor.h"

#include <EditorLib/DataSource.h>

#include <Urho3D/Math/StringHash.h>
#include <Urho3D/Scene/Serializable.h>

namespace SprueEditor
{

    class UrhoPropertyPage : public SprueEditor::BasePropertyPage
    {
    public:
        UrhoPropertyPage(Urho3D::Context* context, Urho3D::StringHash typeHash);
        virtual ~UrhoPropertyPage();

        virtual void SetEditing(std::shared_ptr<DataSource> editing) override;
        virtual void SelectionChanged(void* src, Selectron* sel) override;
        virtual void SelectronDataChanged(void* src, Selectron* sel, unsigned property) override;

        /// Implementation of SelectronLinked
        virtual void Link(Selectron* sel) override;

    protected:
        virtual void CreateControl(QVBoxLayout* layout, unsigned property, const Urho3D::AttributeInfo& attribute, const Urho3D::StringHash& hash);
        virtual void UpdateLabel(const Urho3D::StringHash& hash);
        void UpdateLabels();

        void Build();
        void Rebuild();

        void ResetProperty(const Urho3D::StringHash&);

    private:
        Urho3D::StringHash typeHash_;
        Urho3D::Context* context_ = 0x0;
        std::shared_ptr<UrhoUI::SerializableDataSource> editing_ = 0x0;
        std::map<Urho3D::StringHash, QLabel*> labelMap_;
        std::map<Urho3D::StringHash, UrhoUI::BaseTypeEditor* > typeEditors_;
        std::map<Urho3D::StringHash, std::string> properties_;
        QIcon resetIcon_;
    };

}