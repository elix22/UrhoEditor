#include "UrhoPropertyPage.h"

#include "../../Data/UrhoDataSources.h"

#include "AllUrhoTypeEditors.h"
#include "../../QtHelpers.h"

#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/Variant.h>
#include <Urho3D/Container/Vector.h>

#include <QLayout>

using namespace Urho3D;

namespace SprueEditor
{

UrhoPropertyPage::UrhoPropertyPage(Urho3D::Context* context, Urho3D::StringHash typeHash) :
    BasePropertyPage(),
    context_(context),
    typeHash_(typeHash)
{
    GetLayout()->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    resetIcon_ = QIcon(":/Images/fugue/reset.png");
}

UrhoPropertyPage::~UrhoPropertyPage()
{

}

void UrhoPropertyPage::SetEditing(std::shared_ptr<DataSource> editing)
{
    if (editing_ = std::dynamic_pointer_cast<UrhoUI::SerializableDataSource>(editing))
    {
        setUpdatesEnabled(false);
        blockSignals(true);
        Rebuild();
        for (auto editors : typeEditors_)
            editors.second->SetEditing(editing_);
        UpdateLabels();
        setUpdatesEnabled(true);
        blockSignals(false);
        //???for (auto handler : typeHandlers_)
        //???    handler->ObjectChanged(editing_);
    }
}

void UrhoPropertyPage::SelectionChanged(void* src, Selectron* sel)
{
    //if (auto object = sel->GetMostRecentSelected<UrhoEditor::SerializableDataSource>())
    //{
    //    if (object->GetObject()->GetType() == typeHash_)
    //    {
    //        Rebuild();
    //        SetEditing(object);
    //    }
    //}
}

void UrhoPropertyPage::SelectronDataChanged(void* src, Selectron* sel, unsigned property)
{
    if (src == this)
        return;
    // Confirm it is the same object
    if (editing_ && editing_ == sel->MostRecentSelected())
    {
        // Cast to appropriate datasource type, check is superflous unless DataSource::equals() override is wrong.
        if (auto selected = sel->GetMostRecentSelected<UrhoUI::SerializableDataSource>())
        {
            // Verify typehash, against should be superflous
            if (selected->GetObject()->GetType() == typeHash_)
            {
                // Update all editor controls
                for (auto editors : typeEditors_)
                    editors.second->SetEditing(selected);
            }
        }
    }
}

void UrhoPropertyPage::Link(Selectron* sel)
{
    //connect(sel, &Selectron::SelectionChanged, this, &UrhoPropertyPage::SelectionChanged);
    //connect(sel, &Selectron::DataChanged, this, &UrhoPropertyPage::SelectronDataChanged);
}

void UrhoPropertyPage::CreateControl(QVBoxLayout* layout, unsigned property, const Urho3D::AttributeInfo& attribute, const Urho3D::StringHash& hash)
{
#define IMPL_EDITOR(TYPE, EDITOR) case TYPE: { UrhoUI:: ## EDITOR * ptr = new UrhoUI:: ## EDITOR (property); \
    typeEditors_[hash] = ptr; layout->addLayout(ptr->GetLayout()); \
    connect(ptr, &UrhoUI::BaseTypeEditor::DataChanged, propertyChangeHandler); \
            } break;

#define IMPL_TEXT_EDITOR(TYPE) case TYPE: { UrhoUI::StringConvertibleEditor* ptr = new UrhoUI::StringConvertibleEditor(TYPE, property); \
    typeEditors_[hash] = ptr; layout->addLayout(ptr->GetLayout()); \
    connect(ptr, &UrhoUI::BaseTypeEditor::DataChanged, propertyChangeHandler); \
            } break;

    auto propertyChangeHandler = [=]() {
        UpdateLabel(attribute.name_);
        GetSelectron()->NotifyDataChanged(this, editing_, hash.Value());
    };

    bool storeControl = true;

    if (attribute.type_ == VAR_INT && attribute.enumNames_ != 0x0)
    {
        UrhoUI::EnumEditor* ptr = new UrhoUI::EnumEditor(property, attribute.enumNames_);
        if (storeControl) 
            typeEditors_[hash] = ptr; 
        layout->addLayout(ptr->GetLayout());
        
        connect(ptr, &UrhoUI::BaseTypeEditor::DataChanged, propertyChangeHandler);
        return;
    }
    else if (attribute.type_ == VAR_INT && (attribute.name_.Contains("mask", false) || attribute.name_.Contains("flag", false)))
    {
        UrhoUI::FlagsEditor* ptr = new UrhoUI::FlagsEditor(property);
        if (storeControl)
            typeEditors_[hash] = ptr;
        layout->addLayout(ptr->GetLayout());
        connect(ptr, &UrhoUI::BaseTypeEditor::DataChanged, propertyChangeHandler);
        return;
    }
    else if (attribute.type_ == VAR_INT && attribute.mode_ & AM_NODEID)
    {
        UrhoUI::NodeIDEditor* ptr = new UrhoUI::NodeIDEditor(property);
        if (storeControl)
            typeEditors_[hash] = ptr;
        layout->addLayout(ptr->GetLayout());
        connect(ptr, &UrhoUI::BaseTypeEditor::DataChanged, propertyChangeHandler);
        return;
    }
    else if (attribute.type_ == VAR_VARIANTVECTOR && attribute.mode_ & AM_NODEIDVECTOR)
    {
        UrhoUI::NodeIDListEditor* ptr = new UrhoUI::NodeIDListEditor(property);
        if (storeControl)
            typeEditors_[hash] = ptr;
        layout->addLayout(ptr->GetLayout());
        connect(ptr, &UrhoUI::BaseTypeEditor::DataChanged, propertyChangeHandler);
        return;
    }

    switch (attribute.type_)
    {
    IMPL_EDITOR(VAR_BOOL, BoolEditor);
    IMPL_EDITOR(VAR_INT, IntEditor);
    IMPL_EDITOR(VAR_FLOAT, FloatEditor);
    IMPL_EDITOR(VAR_DOUBLE, DoubleEditor);
    IMPL_EDITOR(VAR_STRING, StringEditor);
    IMPL_EDITOR(VAR_VECTOR2, Vector2Editor);
    IMPL_EDITOR(VAR_VECTOR3, Vector3Editor);
    IMPL_EDITOR(VAR_VECTOR4, Vector4Editor);
    IMPL_EDITOR(VAR_QUATERNION, QuatEditor);
    IMPL_EDITOR(VAR_INTVECTOR2, IntVector2Editor);
    IMPL_EDITOR(VAR_INTRECT, IntRectEditor);
    IMPL_EDITOR(VAR_COLOR, RGBAEditor);
    IMPL_EDITOR(VAR_STRINGVECTOR, StringListEditor);
    IMPL_TEXT_EDITOR(VAR_BUFFER);
    IMPL_EDITOR(VAR_MATRIX3, Matrix3Editor);
    IMPL_EDITOR(VAR_MATRIX3X4, Matrix3x4Editor);
    IMPL_EDITOR(VAR_MATRIX4, Matrix4x4Editor);
    IMPL_EDITOR(VAR_RESOURCEREF, ResourceRefEditor);
    IMPL_EDITOR(VAR_RESOURCEREFLIST, ResourceRefListEditor);
    IMPL_EDITOR(VAR_VARIANTVECTOR, VariantVectorEditor);
    IMPL_EDITOR(VAR_VARIANTMAP, VariantMapEditor);
    IMPL_TEXT_EDITOR(VAR_PTR);
    IMPL_TEXT_EDITOR(VAR_VOIDPTR);
    }
}

void UrhoPropertyPage::UpdateLabel(const Urho3D::StringHash& hash)
{
    if (!editing_)
        return;

    auto foundLabel = labelMap_.find(hash);
    auto foundProp = properties_.find(hash);
    if (foundLabel != labelMap_.end() && foundProp != properties_.end())
    {
        Variant defVal = editing_->GetObject()->GetAttributeDefault(foundProp->second.c_str());
        Variant curVal = editing_->GetObject()->GetAttribute(foundProp->second.c_str());

        if (defVal != curVal)
            foundLabel->second->setText(QString("<font color='#88EE88'>%1</font>").arg(foundProp->second.c_str()));
        else
            foundLabel->second->setText(foundProp->second.c_str());
    }
}

void UrhoPropertyPage::UpdateLabels()
{
    if (!editing_)
        return;
    for (auto lbl : labelMap_)
    {
        auto foundProp = properties_.find(lbl.first);
        if (foundProp != properties_.end())
        {
            Variant defVal = editing_->GetObject()->GetAttributeDefault(foundProp->second.c_str());
            Variant curVal = editing_->GetObject()->GetAttribute(foundProp->second.c_str());
            if (defVal != curVal)
                lbl.second->setText(QString("<font color='#88EE88'>%1</font>").arg(foundProp->second.c_str()));
            else
                lbl.second->setText(foundProp->second.c_str());
        }
    }
}

void UrhoPropertyPage::Build()
{
    auto attributes = context_->GetAttributes(typeHash_);
    if (attributes == 0x0)
        return;
    for (unsigned i = 0; i < attributes->Size(); ++i)
    {
        const Urho3D::AttributeInfo& attribute = attributes->At(i);
        if (attribute.mode_ & AM_NOEDIT)
            continue;

        QLabel* label = new QLabel(attribute.name_.CString());
        labelMap_[attribute.name_] = label;
        properties_[attribute.name_] = attribute.name_.CString();

        QHBoxLayout* lblLayout = new QHBoxLayout();
        lblLayout->setMargin(0);
        lblLayout->addWidget(label);
        lblLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
        QPushButton* resetBtn = new QPushButton(resetIcon_, "");//new QPushButton("Reset");
        resetBtn->setMaximumSize(20, 20);

        connect(resetBtn, &QPushButton::clicked, [=](bool) {
            ResetProperty(Urho3D::StringHash(attribute.name_));
        });
        lblLayout->addWidget(resetBtn);
        GetLayout()->addLayout(lblLayout);
        
        CreateControl(GetLayout(), i, attribute, attribute.name_);
    }
}

void UrhoPropertyPage::Rebuild()
{
    QtHelpers::ClearLayout(GetLayout());
    Build();
}

void UrhoPropertyPage::ResetProperty(const Urho3D::StringHash& hash)
{
    if (!editing_)
        return;
    auto foundProp = properties_.find(hash);
    if (foundProp != properties_.end())
    {
        Variant defVal = editing_->GetObject()->GetAttributeDefault(foundProp->second.c_str());
        editing_->GetObject()->SetAttribute(foundProp->second.c_str(), defVal);

        auto foundEditor = typeEditors_.find(hash);
        if (foundEditor != typeEditors_.end())
            foundEditor->second->PropertyUpdated(editing_);
        UpdateLabel(hash);
        GetSelectron()->NotifyDataChanged(this, editing_, hash.Value());
    }
}

}