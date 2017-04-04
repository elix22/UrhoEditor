#pragma once

#include "../../GlobalAccess.h"
#include "../../Data/UrhoDataSources.h"

#include <QWidget>
#include <QLabel>
#include <QBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>

#include <Urho3D/Scene/Serializable.h>
#include <Urho3D/Core/Attribute.h>

#include <memory>

namespace Urho3D
{
    class Serializable;
}

class QCheckBox;
class QLineEdit;
class QSpinBox;
class QDoubleSpinBox;

namespace UrhoUI
{
    /// Base type for data controller objects.
    class BaseTypeEditor : public QObject
    {
        Q_OBJECT
    public:
        BaseTypeEditor(unsigned property, bool vertical);

        QLayout* GetLayout() { return layout_; }

        virtual void SetEditing(std::shared_ptr<SerializableDataSource> editing);

        virtual void SetEnabled(bool state);

        void SetPropertyIndex(unsigned property) { property_ = property; }

        std::shared_ptr<SerializableDataSource> GetEditing() { return editing_; }

        virtual Urho3D::Variant GetValue() = 0;
        virtual void SetValue(const Urho3D::Variant& value) = 0;

    public slots:
        virtual void PropertyUpdated(std::shared_ptr<SerializableDataSource> editable) = 0;

    signals:
        void DataChanged();

    protected:
        QBoxLayout* layout_;
        std::shared_ptr<SerializableDataSource> editing_;
        unsigned property_;
    };

    class IntEditor : public BaseTypeEditor
    {
        Q_OBJECT
    public:
        IntEditor(unsigned property);

        virtual void PropertyUpdated(std::shared_ptr<SerializableDataSource>) override;

        QSpinBox* spinner_;

        virtual Urho3D::Variant GetValue() override;
        virtual void SetValue(const Urho3D::Variant& value) override;

    public slots:
        void SpinHandler(int value);
    };

    class BoolEditor : public BaseTypeEditor
    {
        Q_OBJECT
    public:
        BoolEditor(unsigned property);

        virtual void PropertyUpdated(std::shared_ptr<SerializableDataSource>) override;
        virtual Urho3D::Variant GetValue() override;
        virtual void SetValue(const Urho3D::Variant& value) override;

        QCheckBox* checkbox_;
    public slots:
        void CheckChanged(int state);
    };

    class UIntEditor : public BaseTypeEditor
    {
        Q_OBJECT
    public:
        UIntEditor(unsigned property);

        virtual Urho3D::Variant GetValue() override;
        virtual void SetValue(const Urho3D::Variant& value) override;
        virtual void PropertyUpdated(std::shared_ptr<SerializableDataSource>) override;

        QSpinBox* spinner_;

    public slots:
        void SpinChanged(int state);
    };

    class FloatEditor : public BaseTypeEditor
    {
        Q_OBJECT
    public:
        FloatEditor(unsigned property);

        virtual Urho3D::Variant GetValue() override;
        virtual void SetValue(const Urho3D::Variant& value) override;
        virtual void PropertyUpdated(std::shared_ptr<SerializableDataSource>) override;

        QDoubleSpinBox* spinner_;

    public slots:
        void SpinHandler(double value);
    };

    class DoubleEditor : public BaseTypeEditor
    {
        Q_OBJECT
    public:
        DoubleEditor(unsigned property);

        virtual Urho3D::Variant GetValue() override;
        virtual void SetValue(const Urho3D::Variant& value) override;
        virtual void PropertyUpdated(std::shared_ptr<SerializableDataSource>) override;

        QDoubleSpinBox* spinner_;

        public slots:
        void SpinHandler(double value);
    };

    class StringEditor : public BaseTypeEditor
    {
        Q_OBJECT
    public:
        StringEditor(unsigned property);

        virtual Urho3D::Variant GetValue() override;
        virtual void SetValue(const Urho3D::Variant& value) override;
        virtual void PropertyUpdated(std::shared_ptr<SerializableDataSource>) override;

        QLineEdit* editor_;

    public slots:
        void TextChanged(const QString& txt);
    };
}