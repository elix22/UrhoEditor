#include "UrhoBaseTypeEditor.h"

#include <QBoxLayout>

#include "../../GlobalAccess.h"
#include "../../QtHelpers.h"
#include "../../Commands/Urho/UrhoPropertyCmd.h"

namespace UrhoUI
{

    BaseTypeEditor::BaseTypeEditor(unsigned property, bool vertical) :
        editing_(0x0)
    {
        property_ = property;
        if (vertical)
            layout_ = new QVBoxLayout();
        else
            layout_ = new QHBoxLayout();
    }

    void BaseTypeEditor::SetEditing(std::shared_ptr<SerializableDataSource> editing)
    {
        editing_ = editing;
        if (editing_ && editing_->GetObject()->GetAttributes() && editing_->GetObject()->GetAttributes()->Size() < property_)
            editing_ = 0x0;
        else if (editing_ && !editing_->GetObject()->GetAttributes())
            editing_ = 0x0;

        if (!editing_)
            SetEnabled(false);
        else
            SetEnabled(true);
        PropertyUpdated(editing_);
    }

    void BaseTypeEditor::SetEnabled(bool state)
    {
        SprueEditor::QtHelpers::SetEnabledOnAll(layout_, state);
    }

    IntEditor::IntEditor(unsigned property) :
        BaseTypeEditor(property, false)
    {
        spinner_ = new QSpinBox();
        layout_->addWidget(spinner_);
        spinner_->setMinimum(INT_MIN);
        spinner_->setMaximum(INT_MAX);

        PropertyUpdated(editing_);

        connect(spinner_, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &IntEditor::SpinHandler);
    }

    Urho3D::Variant IntEditor::GetValue()
    {
        return Urho3D::Variant((int)spinner_->value());
    }

    void IntEditor::SetValue(const Urho3D::Variant& value)
    {
        spinner_->blockSignals(true);
        spinner_->setValue(value.GetInt());
        spinner_->blockSignals(false);
    }

    void IntEditor::SpinHandler(int value)
    {
        if (editing_)
            URHO_PROPERTY_UNDO(editing_, property_, value);
        //editing_->SetAttribute(property_, Urho3D::Variant(value));
    //property_->Set(editing_, value);
        emit DataChanged();
    }

    void IntEditor::PropertyUpdated(std::shared_ptr<SerializableDataSource> editing)
    {
        if (editing == 0x0)
            return;
        spinner_->blockSignals(true);

        spinner_->setValue(editing->GetObject()->GetAttribute(property_).GetInt());

        spinner_->blockSignals(false);
    }

    BoolEditor::BoolEditor(unsigned property) :
        BaseTypeEditor(property, false)
    {
        layout_->addWidget(checkbox_ = new QCheckBox());

        PropertyUpdated(editing_);

        connect(checkbox_, &QCheckBox::stateChanged, this, &BoolEditor::CheckChanged);
    }

    Urho3D::Variant BoolEditor::GetValue()
    {
        return Urho3D::Variant(checkbox_->isChecked());
    }

    void BoolEditor::SetValue(const Urho3D::Variant& value)
    {
        checkbox_->blockSignals(true);
        checkbox_->setChecked(value.GetBool());
        checkbox_->blockSignals(false);
    }

    void BoolEditor::CheckChanged(int state)
    {
        if (!editing_)
            return;
        URHO_PROPERTY_UNDO(editing_, property_, checkbox_->isChecked());
        //editing_->SetAttribute(property_, checkbox_->isChecked());
        emit DataChanged();
    }

    void BoolEditor::PropertyUpdated(std::shared_ptr<SerializableDataSource> editable)
    {
        if (editable == 0x0)
            return;

        checkbox_->blockSignals(true);

        checkbox_->setChecked(editable->GetObject()->GetAttribute(property_).GetBool());

        checkbox_->blockSignals(false);
    }

    UIntEditor::UIntEditor(unsigned property) :
        BaseTypeEditor(property, false)
    {
        layout_->addWidget(spinner_ = new QSpinBox());
        spinner_->setMinimum(0);
        spinner_->setMaximum(INT_MAX);

        PropertyUpdated(editing_);

        connect(spinner_, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &UIntEditor::SpinChanged);
    }

    Urho3D::Variant UIntEditor::GetValue()
    {
        return Urho3D::Variant((unsigned)spinner_->value());
    }

    void UIntEditor::SetValue(const Urho3D::Variant& value)
    {
        spinner_->blockSignals(true);
        spinner_->setValue(value.GetUInt());
        spinner_->blockSignals(false);
    }
    
    void UIntEditor::SpinChanged(int value)
    {
        URHO_PROPERTY_UNDO(editing_, property_, (unsigned)value);
        //editing_->SetAttribute(property_, Urho3D::Variant((unsigned)value));
        emit DataChanged();
    }

    void UIntEditor::PropertyUpdated(std::shared_ptr<SerializableDataSource> editing)
    {
        if (editing == 0x0)
            return;

        spinner_->blockSignals(true);

        spinner_->setValue(editing->GetObject()->GetAttribute(property_).GetUInt());

        spinner_->blockSignals(false);
    }

    FloatEditor::FloatEditor(unsigned property) :
        BaseTypeEditor(property, false)
    {
        layout_->addWidget(spinner_ = new QDoubleSpinBox());

        PropertyUpdated(editing_);
        spinner_->setDecimals(4);
        connect(spinner_, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &FloatEditor::SpinHandler);
    }

    Urho3D::Variant FloatEditor::GetValue()
    {
        return Urho3D::Variant((float)spinner_->value());
    }

    void FloatEditor::SetValue(const Urho3D::Variant& value)
    {
        spinner_->blockSignals(true);
        spinner_->setValue(value.GetFloat());
        spinner_->blockSignals(false);
    }

    void FloatEditor::SpinHandler(double value)
    {
        URHO_PROPERTY_UNDO(editing_, property_, (float)value);
        //editing_->SetAttribute(property_, Urho3D::Variant((float)value));
        emit DataChanged();
    }

    void FloatEditor::PropertyUpdated(std::shared_ptr<SerializableDataSource> editing)
    {
        if (editing == 0x0)
            return;

        spinner_->blockSignals(true);

        spinner_->setValue(editing->GetObject()->GetAttribute(property_).GetFloat());

        spinner_->blockSignals(false);
    }

    DoubleEditor::DoubleEditor(unsigned property) :
        BaseTypeEditor(property, false)
    {
        layout_->addWidget(spinner_ = new QDoubleSpinBox());

        PropertyUpdated(editing_);

        connect(spinner_, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &DoubleEditor::SpinHandler);
    }

    Urho3D::Variant DoubleEditor::GetValue()
    {
        return Urho3D::Variant(spinner_->value());
    }

    void DoubleEditor::SetValue(const Urho3D::Variant& value)
    {
        spinner_->blockSignals(true);
        spinner_->setValue(value.GetDouble());
        spinner_->blockSignals(false);
    }

    void DoubleEditor::SpinHandler(double value)
    {
        URHO_PROPERTY_UNDO(editing_, property_, value);
        //editing_->SetAttribute(property_, Urho3D::Variant((double)value));
        emit DataChanged();
    }

    void DoubleEditor::PropertyUpdated(std::shared_ptr<SerializableDataSource> editing)
    {
        if (editing == 0x0)
            return;

        spinner_->blockSignals(true);

        spinner_->setValue(editing->GetObject()->GetAttribute(property_).GetDouble());

        spinner_->blockSignals(false);
    }

    StringEditor::StringEditor(unsigned property) :
        BaseTypeEditor(property, false)
    {
        editor_ = new QLineEdit();
        auto actionList = editor_->actions();
        for (auto act : actionList)
            editor_->removeAction(act);
        layout_->addWidget(editor_);

        PropertyUpdated(editing_);

        connect(editor_, &QLineEdit::textChanged, this, &StringEditor::TextChanged);
    }

    Urho3D::Variant StringEditor::GetValue()
    {
        //TODO? better way to get cstr?
        return Urho3D::Variant(editor_->text().toStdString().c_str());
    }

    void StringEditor::SetValue(const Urho3D::Variant& value)
    {
        editor_->blockSignals(true);
        editor_->setText(value.GetString().CString());
        editor_->blockSignals(false);
    }

    void StringEditor::TextChanged(const QString& txt)
    {
        URHO_PROPERTY_UNDO(editing_, property_, Urho3D::Variant(txt.toStdString().c_str()));
        emit DataChanged();
    }

    void StringEditor::PropertyUpdated(std::shared_ptr<SerializableDataSource> editing)
    {
        if (editing == 0x0)
            return;

        editor_->blockSignals(true);

        editor_->setText(editing->GetObject()->GetAttribute(property_).GetString().CString());

        editor_->blockSignals(false);
    }
}