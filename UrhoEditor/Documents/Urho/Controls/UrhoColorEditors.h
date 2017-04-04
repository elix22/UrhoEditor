#pragma once

#include "UrhoBaseTypeEditor.h"

class QDoubleSpinBox;
class QPushButton;

namespace UrhoUI
{

    class ColorCurvesGraph;
    class GradientEditor;

    class RGBAEditor : public BaseTypeEditor
    {
        Q_OBJECT
    public:
        RGBAEditor(unsigned property);

        virtual Urho3D::Variant GetValue() override;
        virtual void SetValue(const Urho3D::Variant& value) override;
        virtual void PropertyUpdated(std::shared_ptr<SerializableDataSource>) override;

        QDoubleSpinBox* rSpin_;
        QDoubleSpinBox* gSpin_;
        QDoubleSpinBox* bSpin_;
        QDoubleSpinBox* aSpin_;
        QPushButton* colorButton_;

        public slots:
        void SpinHandler(double);
        void ColorButtonHandler(bool);
    };

}