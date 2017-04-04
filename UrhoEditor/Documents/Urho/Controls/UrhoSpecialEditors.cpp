#include "UrhoSpecialEditors.h"

#include "../../Commands/Urho/UrhoPropertyCmd.h"

#include "../../QtHelpers.h"
#include <Urho3D/Scene/Serializable.h>
#include "UrhoObjectTree.h"

#include <qevent.h>

using namespace Urho3D;

namespace UrhoUI
{

EnumEditor::EnumEditor(unsigned property, const char** enumNames) :
    BaseTypeEditor(property, false)
{
    combo_ = new QComboBox();
    combo_->setFont(QFont("Arial", 8));
    //const char** enumName = property->GetEnumNames();
    for (int i = 0;; ++i)
    {
        if (enumNames[i] == 0x0)
            break;
        combo_->addItem(enumNames[i]);
    }

    PropertyUpdated(editing_);

    connect(combo_, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &EnumEditor::ComboHandler);

    layout_->addWidget(combo_);
}

Urho3D::Variant EnumEditor::GetValue()
{
    return Urho3D::Variant((int)combo_->currentIndex());
}

void EnumEditor::SetValue(const Urho3D::Variant& value)
{
    combo_->blockSignals(true);
    combo_->setCurrentIndex(value.GetInt());
    combo_->blockSignals(false);
}

void EnumEditor::ComboHandler(int idx)
{
    URHO_PROPERTY_UNDO(editing_, property_, idx);
    //editing_->SetAttribute(property_, idx);
    emit DataChanged();
}

void EnumEditor::PropertyUpdated(std::shared_ptr<SerializableDataSource> editing)
{
    if (editing == 0x0)
        return;

    int value = editing->GetObject()->GetAttribute(property_).GetInt();

    combo_->blockSignals(true);
    combo_->setCurrentIndex(value);
    combo_->blockSignals(false);
}

FlagsEditor::FlagsEditor(unsigned property) :
    BaseTypeEditor(property, true)
{
    layout_->setSpacing(0);
    layout_->setMargin(0);

    flags_ = new FlagWidget();
    flags_->SetBitCount(32);
    connect(flags_, &FlagWidget::BitFieldChanged, [=]() {
        URHO_PROPERTY_UNDO(editing_, property_, Variant(flags_->GetBitField()));
        emit DataChanged();
    });
    layout_->addWidget(flags_);
}

Urho3D::Variant FlagsEditor::GetValue()
{
    return flags_->GetBitField();
}

void FlagsEditor::SetValue(const Urho3D::Variant& value)
{
    flags_->blockSignals(true);
    flags_->SetBitField(value.GetUInt());
    flags_->blockSignals(false);
}

void FlagsEditor::CheckBoxHandler(int state)
{

}

void FlagsEditor::PropertyUpdated(std::shared_ptr<SerializableDataSource> editable)
{
    if (editable == 0x0)
        return;

    unsigned bits = editing_->GetObject()->GetAttribute(property_).GetInt();
    flags_->blockSignals(true);
    flags_->SetBitField(bits);
    flags_->blockSignals(false);
}

StringConvertibleEditor::StringConvertibleEditor(VariantType type, unsigned property) :
    BaseTypeEditor(property, false),
    type_(type)
{
    text_ = new QLineEdit();
    layout_->addWidget(text_);

    connect(text_, &QLineEdit::textChanged, this, &StringConvertibleEditor::TextChanged);
}

Urho3D::Variant StringConvertibleEditor::GetValue()
{
    Variant var;
    var.FromString(type_, text_->text().toStdString().c_str());
    return var;
}

void StringConvertibleEditor::SetValue(const Urho3D::Variant& value)
{
    Urho3D::String str = value.ToString();
    text_->blockSignals(true);
    text_->setText(str.CString());
    text_->blockSignals(false);
}

void StringConvertibleEditor::PropertyUpdated(std::shared_ptr<SerializableDataSource> editing)
{
    Urho3D::String str = editing->GetObject()->GetAttribute(property_).ToString();
    text_->blockSignals(true);
    text_->setText(str.CString());
    text_->blockSignals(false);
}

void StringConvertibleEditor::TextChanged(const QString& newText)
{
    if (editing_)
    {
        Variant var;
        var.FromString(type_, newText.toStdString().c_str());
        if (var.GetType() != VAR_NONE)
            URHO_PROPERTY_UNDO(editing_, property_, var);
            //editing_->SetAttribute(property_, var);
    }
}

NodeIDTextEditor::NodeIDTextEditor()
{
    setValidator(new QIntValidator());
}

void NodeIDTextEditor::dragEnterEvent(QDragEnterEvent* event)
{
    if (UrhoObjectTree* source = dynamic_cast<UrhoObjectTree*>(event->source()))
    {
        event->acceptProposedAction();
        event->accept();
        event->setDropAction(Qt::DropAction::CopyAction);
    }
}

void NodeIDTextEditor::dropEvent(QDropEvent* event)
{
    if (UrhoObjectTree* source = dynamic_cast<UrhoObjectTree*>(event->source()))
    {
        auto selected = source->selectedItems();
        Serializable* object = (Serializable*)selected[0]->data(0, Qt::UserRole).value<void*>();
        if (Node* node = dynamic_cast<Node*>(object))
            setText(QString::number(node->GetID()));
    }
}

NodeIDEditor::NodeIDEditor(unsigned property) :
    BaseTypeEditor(property, false)
{
    editor_ = new NodeIDTextEditor();
    GetLayout()->addWidget(editor_);
    connect(editor_, &QLineEdit::textChanged, this, &NodeIDEditor::TextChanged);
}

Urho3D::Variant NodeIDEditor::GetValue()
{
    bool okay = false;
    int value = editor_->text().toInt(&okay);
    if (okay)
        return Urho3D::Variant(value);
    return Urho3D::Variant((int)-1);
}

void NodeIDEditor::SetValue(const Urho3D::Variant& value)
{
    editor_->blockSignals(true);
    editor_->setText(QString::number(value.GetInt()));
    editor_->blockSignals(false);
}

void NodeIDEditor::PropertyUpdated(std::shared_ptr<SerializableDataSource> editing)
{
    if (editing)
    {
        editor_->blockSignals(true);
        editor_->setText(QString::number(editing->GetObject()->GetAttribute(property_).GetInt()));
        editor_->blockSignals(false);
    }
}


void NodeIDEditor::TextChanged(const QString& newText)
{
    if (editing_)
    {
        bool okay = false;
        int value = newText.toInt(&okay);
        if (okay)
            URHO_PROPERTY_UNDO(editing_, property_, value) //editing_->SetAttribute(property_, value);
        else
            URHO_PROPERTY_UNDO(editing_, property_, -1) //editing_->SetAttribute(property_, -1);
    }
}

NodeIDListEditor::NodeIDListEditor(unsigned property) :
    BaseTypeEditor(property, true)
{

}

Urho3D::Variant NodeIDListEditor::GetValue()
{
    //TODO
    return Urho3D::Variant();
}

void NodeIDListEditor::SetValue(const Urho3D::Variant& value)
{
    
}

void NodeIDListEditor::PropertyUpdated(std::shared_ptr<SerializableDataSource> editing)
{
    if (editing_)
    {
        SprueEditor::QtHelpers::ClearLayout(GetLayout());

        auto values = editing_->GetObject()->GetAttribute(property_).GetVariantVector();
        for (unsigned i = 0; i < values.Size(); ++i)
        {
            NodeIDTextEditor* editor = new NodeIDTextEditor();
            editor->setText(QString::number(values[i].GetInt()));
            connect(editor, &QLineEdit::textChanged, [=](const QString& txt) {
                bool okay = false;
                int value = txt.toInt(&okay);
                auto vector = editing->GetObject()->GetAttribute(property_).GetVariantVector();
                
                if (okay)
                    vector[i] = value; 
                else
                    vector[i] = -1;

                URHO_PROPERTY_UNDO(editing_, property_, vector);
                //editing_->SetAttribute(property_, vector);
                emit DataChanged();
                if (i == 0)
                    PropertyUpdated(editing_);
            });
            GetLayout()->addWidget(editor);
        }
    }
}

StringListEditor::StringListEditor(unsigned property) :
    BaseTypeEditor(property, false)
{
    list_ = new QListWidget();
    list_->setSelectionMode(QAbstractItemView::SingleSelection);
    QVBoxLayout* buttonsLayout = new QVBoxLayout();
    QPushButton* addButton = new QPushButton(QIcon(":/Images/godot/icon_add.png"), "");
    QPushButton* delButton = new QPushButton(QIcon(":/Images/godot/icon_remove.png"), "");

    buttonsLayout->setAlignment(Qt::AlignTop);
    buttonsLayout->addWidget(addButton);
    buttonsLayout->addWidget(delButton);

    layout_->addWidget(list_);
    layout_->addLayout(buttonsLayout);

    connect(addButton, &QPushButton::clicked, this, &StringListEditor::ItemAdded);
    connect(delButton, &QPushButton::clicked, this, &StringListEditor::ItemDeleted);
    connect(list_, &QListWidget::itemChanged, this, &StringListEditor::ItemChanged);
}

Urho3D::Variant StringListEditor::GetValue()
{
    // Forbidden
    return Urho3D::Variant();
}

void StringListEditor::SetValue(const Urho3D::Variant& value)
{
    //Forbidden
}

void StringListEditor::PropertyUpdated(std::shared_ptr<SerializableDataSource> editing)
{
    if (editing)
    {
        list_->blockSignals(true);
        list_->clear();
        auto stringVector = editing->GetObject()->GetAttribute(property_).GetStringVector();
        for (unsigned i = 0; i < stringVector.Size(); ++i)
        {
            QListWidgetItem* item = new QListWidgetItem();
            SetupItem(i, stringVector[i], item);
            list_->addItem(item);
        }

        list_->blockSignals(false);
    }
}

void StringListEditor::ItemAdded(bool)
{
    if (editing_)
    {
        auto stringVec = editing_->GetObject()->GetAttribute(property_).GetStringVector();
        stringVec.Push("<New String>");
        QListWidgetItem* item = new QListWidgetItem();
        SetupItem(stringVec.Size() - 1, "<New String>", item);
        list_->addItem(item);
        URHO_PROPERTY_UNDO(editing_, property_, stringVec);
        //editing_->SetAttribute(property_, stringVec);
        emit DataChanged();
    }
}

void StringListEditor::ItemDeleted(bool)
{
    if (editing_ && list_->selectedItems().size())
    {
        QListWidgetItem* item = list_->selectedItems()[0];
        int index = item->data(Qt::UserRole).value<int>();
        delete item;
        auto stringVec = editing_->GetObject()->GetAttribute(property_).GetStringVector();
        if (index < stringVec.Size())
            stringVec.Erase(index, 1);
        URHO_PROPERTY_UNDO(editing_, property_, stringVec);
        //editing_->SetAttribute(property_, stringVec);
        emit DataChanged();
    }
}

void StringListEditor::ItemChanged(QListWidgetItem* item)
{
    if (editing_)
    {
        int index = item->data(Qt::UserRole).value<int>();
        auto stringVec = editing_->GetObject()->GetAttribute(property_).GetStringVector();
        if (stringVec.Size() > index)
            stringVec[index] = Urho3D::String(item->text().toStdString().c_str());
        URHO_PROPERTY_UNDO(editing_, property_, stringVec);
        //editing_->SetAttribute(property_, stringVec);
        emit DataChanged();
    }
}

void StringListEditor::SetupItem(unsigned index, const Urho3D::String& text, QListWidgetItem* item)
{
    item->setText(text.CString());
    item->setData(Qt::UserRole, (int)index);
    item->setFlags(item->flags() | Qt::ItemIsEditable);
}

VariantTypeCombo::VariantTypeCombo() :
    QComboBox()
{
    for (int i = VariantType::VAR_NONE; i < VariantType::MAX_VAR_TYPES; ++i)
        addItem(Variant::GetTypeName((Urho3D::VariantType)i).CString());

    connect(this, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &VariantTypeCombo::ComboIndexChanged);
}

void VariantTypeCombo::SetType(Urho3D::VariantType type)
{
    setCurrentIndex((int)type);
}

Urho3D::VariantType VariantTypeCombo::GetType() const
{
    return (Urho3D::VariantType)currentIndex();
}

#define IMPL_EX(VART, RET) case VART: return RET

Urho3D::Variant VariantTypeCombo::GetCleanExemplar() const
{
    switch (GetType())
    {
    IMPL_EX(VAR_BOOL, false);
    IMPL_EX(VAR_INT, (int)0);
    IMPL_EX(VAR_FLOAT, 0.0f);
    IMPL_EX(VAR_DOUBLE, 0.0);
    IMPL_EX(VAR_COLOR, Color());
    IMPL_EX(VAR_VECTOR2, Vector2());
    IMPL_EX(VAR_VECTOR3, Vector3());
    IMPL_EX(VAR_VECTOR4, Vector4());
    IMPL_EX(VAR_QUATERNION, Quaternion());
    IMPL_EX(VAR_MATRIX3, Matrix3());
    IMPL_EX(VAR_MATRIX3X4, Matrix3x4());
    IMPL_EX(VAR_MATRIX4, Matrix4());
    IMPL_EX(VAR_INTRECT, IntRect());
    IMPL_EX(VAR_INTVECTOR2, IntVector2());
    IMPL_EX(VAR_RESOURCEREF, ResourceRef());
    IMPL_EX(VAR_RESOURCEREFLIST, ResourceRefList());
    IMPL_EX(VAR_VARIANTVECTOR, VariantVector());
    IMPL_EX(VAR_VARIANTMAP, VariantMap());
    IMPL_EX(VAR_STRING, Urho3D::String());
    IMPL_EX(VAR_STRINGVECTOR, StringVector());
    IMPL_EX(VAR_VOIDPTR, (void*)0x0);
    IMPL_EX(VAR_BUFFER, Urho3D::VectorBuffer());
    }
    return Urho3D::Variant();
}

void VariantTypeCombo::ComboIndexChanged(int newIndex)
{
    emit VariantTypeChanged((Urho3D::VariantType)newIndex);
}

}