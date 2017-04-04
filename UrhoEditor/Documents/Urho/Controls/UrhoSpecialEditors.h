#pragma once

#include "UrhoBaseTypeEditor.h"

#include <EditorLib/Controls/FlagWidget.h>

#include <QComboBox>
#include <QListWidget>

namespace UrhoUI
{

    class EnumEditor : public BaseTypeEditor
    {
    public:
        Q_OBJECT
    public:
        EnumEditor(unsigned property, const char** enumNames);

        virtual Urho3D::Variant GetValue() override;
        virtual void SetValue(const Urho3D::Variant& value) override;

        virtual void PropertyUpdated(std::shared_ptr<SerializableDataSource>) override;

        QComboBox* combo_;

    public slots:
        void ComboHandler(int);
    };


    class FlagsEditor : public BaseTypeEditor
    {
        Q_OBJECT
    public:
        FlagsEditor(unsigned property);

        virtual Urho3D::Variant GetValue() override;
        virtual void SetValue(const Urho3D::Variant& value) override;

        virtual void PropertyUpdated(std::shared_ptr<SerializableDataSource>) override;

        std::vector<QCheckBox*> checkboxes_;

    public slots:
        void CheckBoxHandler(int state);

    private:
        FlagWidget* flags_;
    };

    class StringConvertibleEditor : public BaseTypeEditor
    {
        Q_OBJECT
    public:
        StringConvertibleEditor(Urho3D::VariantType type, unsigned);

        virtual Urho3D::Variant GetValue() override;
        virtual void SetValue(const Urho3D::Variant& value) override;

        virtual void PropertyUpdated(std::shared_ptr<SerializableDataSource>) override;

    public slots:
        void TextChanged(const QString& newText);

    private:
        QLineEdit* text_;
        Urho3D::VariantType type_;
    };

    class NodeIDTextEditor : public QLineEdit
    {
    public:
        NodeIDTextEditor();

    protected:
        virtual void dragEnterEvent(QDragEnterEvent*) Q_DECL_OVERRIDE;
        virtual void dropEvent(QDropEvent*) Q_DECL_OVERRIDE;
    };

    class NodeIDEditor : public BaseTypeEditor
    {
        Q_OBJECT
    public:
        NodeIDEditor(unsigned);

        virtual Urho3D::Variant GetValue() override;
        virtual void SetValue(const Urho3D::Variant& value) override;

        virtual void PropertyUpdated(std::shared_ptr<SerializableDataSource>) override;

    public slots:
        void TextChanged(const QString& newText);

    private:
        NodeIDTextEditor* editor_;
    };

    class NodeIDListEditor : public BaseTypeEditor
    {
        Q_OBJECT
    public:
        NodeIDListEditor(unsigned);

        virtual Urho3D::Variant GetValue() override;
        virtual void SetValue(const Urho3D::Variant& value) override;

        virtual void PropertyUpdated(std::shared_ptr<SerializableDataSource>) override;

    private:
    };

    class StringListEditor : public BaseTypeEditor
    {
        Q_OBJECT
    public:
        StringListEditor(unsigned);

        virtual Urho3D::Variant GetValue() override;
        virtual void SetValue(const Urho3D::Variant& value) override;

        virtual void PropertyUpdated(std::shared_ptr<SerializableDataSource> editing) override;

    protected slots:
        void ItemAdded(bool);
        void ItemDeleted(bool);
        void ItemChanged(QListWidgetItem*);

    private:
        void SetupItem(unsigned index, const Urho3D::String& text, QListWidgetItem*);

        QListWidget* list_;
    };

    class VariantTypeCombo : public QComboBox
    {
        Q_OBJECT
    public:
        VariantTypeCombo();

        void SetType(Urho3D::VariantType type);
        Urho3D::VariantType GetType() const;
        Urho3D::Variant GetCleanExemplar() const;

    signals:
        void VariantTypeChanged(Urho3D::VariantType newType);

    protected slots:
        void ComboIndexChanged(int newIndex);
    };
}