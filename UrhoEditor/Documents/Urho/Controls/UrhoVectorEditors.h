#pragma once

#include "UrhoBaseTypeEditor.h"

class QLabel;
class QSpinBox;
class QDoubleSpinBox;

namespace UrhoUI
{

    class IntVector2Editor : public BaseTypeEditor
    {
        Q_OBJECT
    public:
        IntVector2Editor(unsigned property);

        virtual Urho3D::Variant GetValue() override;
        virtual void SetValue(const Urho3D::Variant& value) override;

        virtual void PropertyUpdated(std::shared_ptr<SerializableDataSource>) override;

        QLabel* xLbl_;
        QLabel* yLbl_;
        QSpinBox* xSpin_;
        QSpinBox* ySpin_;

    public slots:
        void SpinHandler(int);
    };

    class Vector2Editor : public BaseTypeEditor
    {
        Q_OBJECT
    public:
        Vector2Editor(unsigned property);

        virtual Urho3D::Variant GetValue() override;
        virtual void SetValue(const Urho3D::Variant& value) override;

        virtual void PropertyUpdated(std::shared_ptr<SerializableDataSource>) override;

        QLabel* xLbl_;
        QLabel* yLbl_;
        QDoubleSpinBox* xSpin_;
        QDoubleSpinBox* ySpin_;

    public slots:
        void SpinHandler(double);
    };

    class Vector3Editor : public BaseTypeEditor
    {
        Q_OBJECT
    public:
        Vector3Editor(unsigned property);

        virtual Urho3D::Variant GetValue() override;
        virtual void SetValue(const Urho3D::Variant& value) override;

        virtual void PropertyUpdated(std::shared_ptr<SerializableDataSource>) override;

        QLabel* xLbl_;
        QLabel* yLbl_;
        QLabel* zLbl_;
        QDoubleSpinBox* xSpin_;
        QDoubleSpinBox* ySpin_;
        QDoubleSpinBox* zSpin_;

    public slots:
        void SpinHandler(double);
    };

    class Vector4Editor : public BaseTypeEditor
    {
        Q_OBJECT
    public:
        Vector4Editor(unsigned property);

        virtual Urho3D::Variant GetValue() override;
        virtual void SetValue(const Urho3D::Variant& value) override;

        virtual void PropertyUpdated(std::shared_ptr<SerializableDataSource>) override;

        QLabel* xLbl_;
        QLabel* yLbl_;
        QLabel* zLbl_;
        QLabel* wLbl_;
        QDoubleSpinBox* xSpin_;
        QDoubleSpinBox* ySpin_;
        QDoubleSpinBox* zSpin_;
        QDoubleSpinBox* wSpin_;

        public slots:
        void SpinHandler(double);
    };

    class QuatEditor : public BaseTypeEditor
    {
        Q_OBJECT
    public:
        QuatEditor(unsigned property);

        virtual Urho3D::Variant GetValue() override;
        virtual void SetValue(const Urho3D::Variant& value) override;

        virtual void PropertyUpdated(std::shared_ptr<SerializableDataSource>) override;

        QLabel* xLbl_;
        QLabel* yLbl_;
        QLabel* zLbl_;
        QDoubleSpinBox* xSpin_;
        QDoubleSpinBox* ySpin_;
        QDoubleSpinBox* zSpin_;

    public slots:
        void SpinHandler(double);
    };

}