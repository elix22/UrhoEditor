#pragma once

#include "UrhoBaseTypeEditor.h"

namespace UrhoUI
{

    class Matrix3Editor : public BaseTypeEditor
    {
        Q_OBJECT
    public:
        Matrix3Editor(unsigned property);

        virtual Urho3D::Variant GetValue() override;
        virtual void SetValue(const Urho3D::Variant& value) override;

        virtual void PropertyUpdated(std::shared_ptr<SerializableDataSource>) override;

        QDoubleSpinBox** spinners_;

    public slots:
        void SpinHandler(double);
    };

    class Matrix3x4Editor : public BaseTypeEditor
    {
        Q_OBJECT
    public:
        Matrix3x4Editor(unsigned property);

        virtual Urho3D::Variant GetValue() override;
        virtual void SetValue(const Urho3D::Variant& value) override;

        virtual void PropertyUpdated(std::shared_ptr<SerializableDataSource>) override;

        QDoubleSpinBox** spinners_;

    public slots:
        void SpinHandler(double);
    };

    class Matrix4x4Editor : public BaseTypeEditor
    {
        Q_OBJECT
    public:
        Matrix4x4Editor(unsigned property);

        virtual Urho3D::Variant GetValue() override;
        virtual void SetValue(const Urho3D::Variant& value) override;

        virtual void PropertyUpdated(std::shared_ptr<SerializableDataSource>) override;

        QDoubleSpinBox** spinners_;

        public slots:
        void SpinHandler(double);
    };
}