#include "UrhoCollectionEditor.h"

#include "UrhoSpecialEditors.h"
#include "../../UrhoHelpers.h"
#include "../../Commands/Urho/UrhoPropertyCmd.h"

#include <Urho3D/Scene/Node.h>

#include <QHeaderView>

using namespace Urho3D;

namespace UrhoUI
{

    BaseCollectionEditor::BaseCollectionEditor(unsigned property) :
        BaseTypeEditor(property, false)
    {
        table_ = new QTableWidget();
        table_->setMinimumHeight(160);
        table_->setSelectionMode(QAbstractItemView::SingleSelection);

        QVBoxLayout* buttonsLayout = new QVBoxLayout();
        QPushButton* addButton = new QPushButton(QIcon(":/Images/godot/icon_add.png"), "");
        QPushButton* delButton = new QPushButton(QIcon(":/Images/godot/icon_remove.png"), "");

        QHBoxLayout* newControlsLayout = new QHBoxLayout();
        newItemType_ = new VariantTypeCombo();
        newItemName_ = new QLineEdit();
        newItemName_->setPlaceholderText(tr("variable name"));
        newControlsLayout->addWidget(newItemType_);
        newControlsLayout->addWidget(newItemName_);

        QVBoxLayout* listLayout = new QVBoxLayout();
        listLayout->addLayout(newControlsLayout);
        listLayout->addWidget(table_);

        buttonsLayout->setAlignment(Qt::AlignTop);
        buttonsLayout->addWidget(addButton);
        buttonsLayout->addWidget(delButton);

        connect(addButton, &QPushButton::clicked, this, &BaseCollectionEditor::ItemAdded);
        connect(delButton, &QPushButton::clicked, this, &BaseCollectionEditor::ItemDeleted);

        layout_->addLayout(listLayout);
        layout_->addLayout(buttonsLayout);
    }

    VariantVectorEditor::VariantVectorEditor(unsigned property) :
        BaseCollectionEditor(property)
    {
        table_->setColumnCount(2);
        table_->verticalHeader()->setVisible(false);
        QStringList headers;
        headers.push_back("Type");
        headers.push_back("Value");
        table_->setHorizontalHeaderLabels(headers);
    }

    Urho3D::Variant VariantVectorEditor::GetValue()
    {
        return Urho3D::Variant();
    }

    void VariantVectorEditor::SetValue(const Urho3D::Variant& value)
    {
        table_->blockSignals(true);
        while (table_->rowCount())
            table_->removeRow(0);

        Urho3D::VariantVector data = value.GetVariantVector();

        unsigned row = 0;
        for (auto item = data.Begin(); item != data.End(); ++item, ++row)
        {
            table_->setItem(row, 0, new QTableWidgetItem(item->GetTypeName(item->GetType()).CString()));
            table_->setItem(row, 1, new QTableWidgetItem(item->ToString().CString()));
        }

        table_->blockSignals(false);
    }

    void VariantVectorEditor::PropertyUpdated(std::shared_ptr<SerializableDataSource> editing)
    {
        if (editing_)
        {
            table_->blockSignals(true);
            while (table_->rowCount())
                table_->removeRow(0);

            Urho3D::VariantVector data = editing->GetObject()->GetAttribute(property_).GetVariantVector();

            unsigned row = 0;
            for (auto item = data.Begin(); item != data.End(); ++item, ++row)
            {
                table_->setItem(row, 0, new QTableWidgetItem(item->GetTypeName(item->GetType()).CString()));
                table_->setItem(row, 1, new QTableWidgetItem(item->ToString().CString()));
            }

            table_->blockSignals(false);
        }
    }

    VariantMapEditor::VariantMapEditor(unsigned property) :
        BaseCollectionEditor(property)
    {
        table_->setColumnCount(3);
        table_->verticalHeader()->setVisible(false);
        table_->setHorizontalHeaderItem(0, new QTableWidgetItem("Name"));
        table_->setHorizontalHeaderItem(1, new QTableWidgetItem("Type"));
        table_->setHorizontalHeaderItem(2, new QTableWidgetItem("Value"));

        connect(table_, &QTableWidget::itemChanged, [=](QTableWidgetItem* changed) {
            if (editing_)
            {
                Urho3D::VariantMap data = editing_->GetObject()->GetAttribute(property_).GetVariantMap();
                unsigned row = 0;
                for (auto item = data.Begin(); item != data.End(); ++item, ++row)
                {
                    if (row == changed->row())
                    {
                        if (changed->column() == 2)
                        {
                            // always check for a new type
                            Urho3D::VariantType type = Urho3D::Variant::GetTypeFromName(table_->item(row, 1)->text().toStdString().c_str());
                            Urho3D::Variant var;
                            var.FromString(type, changed->text().toStdString().c_str());
                            data[item->first_] = var;

                            table_->blockSignals(true);
                            // Update our table text
                            table_->item(row, 2)->setText(var.ToString().CString());
                            table_->blockSignals(false);
                            
                            URHO_PROPERTY_UNDO(editing_, property_, data);
                            //editing_->SetAttribute(property_, data);
                        }
                        return;
                    }
                }
            }
        });
    }

    Urho3D::Variant VariantMapEditor::GetValue()
    {
        return Urho3D::Variant();
    }

    void VariantMapEditor::SetValue(const Urho3D::Variant& value)
    {
        //Forbidden
    }

    void VariantMapEditor::PropertyUpdated(std::shared_ptr<SerializableDataSource> editing)
    {
        if (editing_)
        {
            table_->blockSignals(true);
            while (table_->rowCount())
                table_->removeRow(0);

            Urho3D::VariantMap data = editing->GetObject()->GetAttribute(property_).GetVariantMap();

            unsigned row = 0;
            for (auto item = data.Begin(); item != data.End(); ++item, ++row)
            {
                table_->insertRow(table_->rowCount());

                
                QTableWidgetItem* nameItem = new QTableWidgetItem(SprueEditor::RecoverVariableName(dynamic_cast<Urho3D::Node*>(editing->GetObject().Get()), item->first_).c_str());
                nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
                QTableWidgetItem* typeItem = new QTableWidgetItem(Urho3D::Variant::GetTypeName(item->second_.GetType()).CString());
                typeItem->setFlags(typeItem->flags() & ~Qt::ItemIsEditable);
                QTableWidgetItem* valueItem = new QTableWidgetItem(item->second_.ToString().CString());

                //VariantTypeCombo* combo = new VariantTypeCombo();
                //combo->SetType(item->second_.GetType());
                //connect(combo, &VariantTypeCombo::VariantTypeChanged, [=](Urho3D::VariantType newType) {
                //    emit table_->itemChanged(valueItem);
                //});

                table_->setItem(row, 0, nameItem);
                //table_->setCellWidget(row, 1, combo);
                table_->setItem(row, 1, typeItem);
                table_->setItem(row, 2, valueItem);
            }

            table_->blockSignals(false);
        }
    }

    void VariantMapEditor::ItemAdded(bool)
    {
        if (editing_)
        {
            Urho3D::VariantType type = newItemType_->GetType();
            if (type != Urho3D::VAR_NONE && !newItemName_->text().isEmpty())
            {
                auto varMap = editing_->GetObject()->GetAttribute(property_).GetVariantMap();
                Urho3D::Variant var = newItemType_->GetCleanExemplar();
                if (var.GetType() != Urho3D::VAR_NONE)
                {
                    std::string varName = newItemName_->text().toStdString().c_str();
                    varMap[varName.c_str()] = var;
                    SprueEditor::StoreVariableName(dynamic_cast<Urho3D::Node*>(editing_->GetObject().Get()), varName);
                    URHO_PROPERTY_UNDO(editing_, property_, varMap);
                    //editing_->SetAttribute(property_, varMap);
                    emit DataChanged();
                    PropertyUpdated(editing_);
                }
            }
        }
    }

    void VariantMapEditor::ItemDeleted(bool)
    {
        if (editing_ && table_->selectedItems().size())
        {
            QTableWidgetItem* item = table_->selectedItems()[0];
            auto varMap = editing_->GetObject()->GetAttribute(property_).GetVariantMap();
            unsigned i = 0;
            for (auto it = varMap.Begin(); it != varMap.End(); ++it, ++i)
            {
                if (i == item->row())
                {
                    varMap.Erase(it->first_);
                    break;
                }
            }
            table_->removeRow(item->row());

            URHO_PROPERTY_UNDO(editing_, property_, varMap);
            //editing_->SetAttribute(property_, varMap);
            emit DataChanged();
        }
    }
}