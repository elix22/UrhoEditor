#include "UrhoPrimitiveEditors.h"

#include "../../Commands/Urho/UrhoPropertyCmd.h"

#include <Urho3D/Math/Rect.h>
#include <Urho3D/Core/Variant.h>


#define SET_RANGE(SPINNER) SPINNER->setMinimum(INT_MIN); SPINNER->setMaximum(INT_MAX); SPINNER->setMaximumWidth(80);

#define XStyle "QLabel { color: white; font-size: 8pt; font-weight: bold; background-color: #DD0000; border-top-left-radius: 5px; border-bottom-left-radius: 5px; border-bottom-right-radius: 0px; border-top-right-radius: 0px; }"
#define YStyle "QLabel { color: white; font-size: 8pt; font-weight: bold; background-color: #00AA00; border-top-left-radius: 5px; border-bottom-left-radius: 5px; border-bottom-right-radius: 0px; border-top-right-radius: 0px; }"
#define ZStyle "QLabel { color: white; font-size: 8pt; font-weight: bold; background-color: #3F3FDD; border-top-left-radius: 5px; border-bottom-left-radius: 5px; border-bottom-right-radius: 0px; border-top-right-radius: 0px; }"
#define AStyle "QLabel { color: white; font-size: 8pt; font-weight: bold; background-color: #AA007F; border-top-left-radius: 5px; border-bottom-left-radius: 5px; border-bottom-right-radius: 0px; border-top-right-radius: 0px; }"

#define CREATE_LBL(VARNAME, TEXT, STYLE, TIP) VARNAME = new QLabel(" " TEXT); \
VARNAME->setMaximumWidth(18); \
VARNAME->setMinimumWidth(18); \
VARNAME->setMaximumHeight(18); \
VARNAME->setStyleSheet(STYLE); \
VARNAME->setAlignment(Qt::AlignCenter); \
VARNAME->setToolTip(TIP);

#define CREATE_LBL_SIZED(VARNAME, TEXT, STYLE, SIZE, TIP) VARNAME = new QLabel(" " TEXT); \
VARNAME->setMaximumWidth(SIZE); \
VARNAME->setMinimumWidth(SIZE); \
VARNAME->setMaximumHeight(18); \
VARNAME->setStyleSheet(STYLE); \
VARNAME->setAlignment(Qt::AlignCenter); \
VARNAME->setToolTip(TIP);

namespace UrhoUI
{

    IntRectEditor::IntRectEditor(unsigned property) :
        BaseTypeEditor(property, false)
    {
        layout_->setSpacing(0);
        layout_->setAlignment(Qt::AlignLeft);
        CREATE_LBL(xLbl_, "L", XStyle, "");
        CREATE_LBL(yLbl_, "T", YStyle, "");
        CREATE_LBL(zLbl_, "R", ZStyle, "");
        CREATE_LBL(wLbl_, "B", AStyle, "");

        xSpin_ = new QSpinBox();
        ySpin_ = new QSpinBox();
        widthSpin_ = new  QSpinBox();
        heightSpin_ = new QSpinBox();

        SET_RANGE(xSpin_);
        SET_RANGE(ySpin_);
        SET_RANGE(widthSpin_);
        SET_RANGE(heightSpin_);

        layout_->addWidget(xLbl_);
        layout_->addWidget(xSpin_);
        layout_->addItem(new QSpacerItem(4, 1));
        layout_->addWidget(yLbl_);
        layout_->addWidget(ySpin_);
        layout_->addItem(new QSpacerItem(4, 1));
        layout_->addWidget(zLbl_);
        layout_->addWidget(widthSpin_);
        layout_->addItem(new QSpacerItem(4, 1));
        layout_->addWidget(wLbl_);
        layout_->addWidget(heightSpin_);

        PropertyUpdated(editing_);

        connect(xSpin_, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &IntRectEditor::SpinHandler);
        connect(ySpin_, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &IntRectEditor::SpinHandler);
        connect(widthSpin_, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &IntRectEditor::SpinHandler);
        connect(heightSpin_, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &IntRectEditor::SpinHandler);
    }

    Urho3D::Variant IntRectEditor::GetValue()
    {
        return Urho3D::IntRect(xSpin_->value(), ySpin_->value(), xSpin_->value() + widthSpin_->value(), ySpin_->value() + heightSpin_->value());
    }

    void IntRectEditor::SetValue(const Urho3D::Variant& value)
    {
        auto val = value.GetIntRect();
        xSpin_->blockSignals(true);
        ySpin_->blockSignals(true);
        widthSpin_->blockSignals(true);
        heightSpin_->blockSignals(true);

        xSpin_->setValue(val.left_);
        ySpin_->setValue(val.top_);
        widthSpin_->setValue(val.right_ - val.left_);
        heightSpin_->setValue(val.bottom_ - val.top_);

        xSpin_->blockSignals(false);
        ySpin_->blockSignals(false);
        widthSpin_->blockSignals(false);
        heightSpin_->blockSignals(false);
    }

    void IntRectEditor::SpinHandler(double)
    {
        Urho3D::IntRect val(xSpin_->value(), ySpin_->value(), widthSpin_->value(), heightSpin_->value());
        URHO_PROPERTY_UNDO(editing_, property_, val);
        //editing_->SetAttribute(property_, val);
        emit DataChanged();
    }

    void IntRectEditor::PropertyUpdated(std::shared_ptr<SerializableDataSource> editable)
    {
        if (editable == 0x0)
            return;
        xSpin_->blockSignals(true);
        ySpin_->blockSignals(true);
        widthSpin_->blockSignals(true);
        heightSpin_->blockSignals(true);

        Urho3D::IntRect vec = editable->GetObject()->GetAttribute(property_).GetIntRect();
        xSpin_->setValue(vec.left_);
        ySpin_->setValue(vec.top_);
        widthSpin_->setValue(vec.right_);
        heightSpin_->setValue(vec.bottom_);

        xSpin_->blockSignals(false);
        ySpin_->blockSignals(false);
        widthSpin_->blockSignals(false);
        heightSpin_->blockSignals(false);
    }

}