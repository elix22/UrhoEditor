#pragma once

#include "UrhoBaseTypeEditor.h"

namespace UrhoUI
{

    class IntRectEditor : public BaseTypeEditor
    {
        Q_OBJECT
    public:
        IntRectEditor(unsigned property);

        virtual Urho3D::Variant GetValue() override;
        virtual void SetValue(const Urho3D::Variant& value) override;

        virtual void PropertyUpdated(std::shared_ptr<SerializableDataSource>) override;

        QLabel* xLbl_;
        QLabel* yLbl_;
        QLabel* zLbl_;
        QLabel* wLbl_;
        QSpinBox* xSpin_;
        QSpinBox* ySpin_;
        QSpinBox* widthSpin_;
        QSpinBox* heightSpin_;

    public slots:
        void SpinHandler(double);
    };

}