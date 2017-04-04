#pragma once

#include "UrhoBaseTypeEditor.h"

#include <QLineEdit>
#include <QTableWidget>

namespace UrhoUI
{
    class VariantTypeCombo;

    class BaseCollectionEditor : public BaseTypeEditor
    {
        Q_OBJECT
    public:
        BaseCollectionEditor(unsigned property);

    protected slots:
        virtual void ItemAdded(bool) { }
        virtual void ItemDeleted(bool) { }

    protected:
        QTableWidget* table_;
        QLineEdit* newItemName_;
        VariantTypeCombo* newItemType_;
    };

    class VariantVectorEditor : public BaseCollectionEditor
    {
    public:
        VariantVectorEditor(unsigned property);

        virtual Urho3D::Variant GetValue() override;
        virtual void SetValue(const Urho3D::Variant& value) override;

        virtual void PropertyUpdated(std::shared_ptr<SerializableDataSource>) override;
    };

    class VariantMapEditor : public BaseCollectionEditor
    {
    public:
        VariantMapEditor(unsigned property);

        virtual Urho3D::Variant GetValue() override;
        virtual void SetValue(const Urho3D::Variant& value) override;

        virtual void PropertyUpdated(std::shared_ptr<SerializableDataSource>) override;

    protected:
        virtual void ItemAdded(bool) override;
        virtual void ItemDeleted(bool) override;
    };
}