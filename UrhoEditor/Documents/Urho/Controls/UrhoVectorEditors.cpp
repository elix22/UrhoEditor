#include "UrhoVectorEditors.h"

#include "../../Commands/Urho/UrhoPropertyCmd.h"

#include <qspinbox.h>

#include <Urho3D/Scene/Node.h>

using namespace Urho3D;

namespace UrhoUI
{

#define SET_RANGE(SPINNER) SPINNER->setMinimum(INT_MIN); SPINNER->setMaximum(INT_MAX); 

#define SET_RANGE_DBL(SPINNER) SPINNER->setMinimum(INT_MIN); SPINNER->setMaximum(INT_MAX); SPINNER->setDecimals(4);
    //SPINNER->setMaximumWidth(80);

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

    IntVector2Editor::IntVector2Editor(unsigned property) :
        BaseTypeEditor(property, false)
    {
        layout_->setSpacing(0);
        layout_->setAlignment(Qt::AlignLeft);
        CREATE_LBL(xLbl_, "X", XStyle, "");
        CREATE_LBL(yLbl_, "Y", YStyle, "");

        xSpin_ = new QSpinBox();
        ySpin_ = new QSpinBox();

        SET_RANGE(xSpin_);
        SET_RANGE(ySpin_);

        layout_->addWidget(xLbl_);
        layout_->addWidget(xSpin_);
        layout_->addItem(new QSpacerItem(4, 1));
        layout_->addWidget(yLbl_);
        layout_->addWidget(ySpin_);

        //PropertyUpdated(editable);

        connect(xSpin_, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &IntVector2Editor::SpinHandler);
        connect(ySpin_, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &IntVector2Editor::SpinHandler);
    }

    Urho3D::Variant IntVector2Editor::GetValue()
    {
        return Urho3D::IntVector2(xSpin_->value(), ySpin_->value());
    }

    void IntVector2Editor::SetValue(const Urho3D::Variant& value)
    {
        xSpin_->blockSignals(true);
        ySpin_->blockSignals(true);
        
        auto val = value.GetIntVector2();
        xSpin_->setValue(val.x_);
        ySpin_->setValue(val.y_);

        xSpin_->blockSignals(false);
        ySpin_->blockSignals(false);
    }

    void IntVector2Editor::SpinHandler(int value)
    {
        IntVector2 val(xSpin_->value(), ySpin_->value());
        URHO_PROPERTY_UNDO(editing_, property_, val);
        //editing_->SetAttribute(property_, val);
        emit DataChanged();
    }

    void IntVector2Editor::PropertyUpdated(std::shared_ptr<SerializableDataSource> editable)
    {
        if (editable == 0x0)
            return;

        xSpin_->blockSignals(true);
        ySpin_->blockSignals(true);

        IntVector2 vec = editable->GetObject()->GetAttribute(property_).GetIntVector2();
        xSpin_->setValue(vec.x_);
        ySpin_->setValue(vec.y_);

        xSpin_->blockSignals(false);
        ySpin_->blockSignals(false);
    }

    Vector2Editor::Vector2Editor(unsigned property) :
        BaseTypeEditor(property, false)
    {
        layout_->setSpacing(0);
        layout_->setAlignment(Qt::AlignLeft);
        CREATE_LBL(xLbl_, "X", XStyle, "");
        CREATE_LBL(yLbl_, "Y", YStyle, "");

        xSpin_ = new QDoubleSpinBox();
        ySpin_ = new QDoubleSpinBox();

        SET_RANGE_DBL(xSpin_);
        SET_RANGE_DBL(ySpin_);

        layout_->addWidget(xLbl_);
        layout_->addWidget(xSpin_);
        layout_->addItem(new QSpacerItem(4, 1));
        layout_->addWidget(yLbl_);
        layout_->addWidget(ySpin_);

        PropertyUpdated(editing_);

        connect(xSpin_, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &Vector2Editor::SpinHandler);
        connect(ySpin_, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &Vector2Editor::SpinHandler);
    }

    Urho3D::Variant Vector2Editor::GetValue()
    {
        return Urho3D::Vector2(xSpin_->value(), ySpin_->value());
    }

    void Vector2Editor::SetValue(const Urho3D::Variant& value)
    {
        xSpin_->blockSignals(true);
        ySpin_->blockSignals(true);

        auto val = value.GetIntVector2();
        xSpin_->setValue(val.x_);
        ySpin_->setValue(val.y_);

        xSpin_->blockSignals(false);
        ySpin_->blockSignals(false);
    }

    void Vector2Editor::SpinHandler(double)
    {
        Vector2 val(xSpin_->value(), ySpin_->value());
        URHO_PROPERTY_UNDO(editing_, property_, val);
        //editing_->SetAttribute(property_, val);
        emit DataChanged();
    }

    void Vector2Editor::PropertyUpdated(std::shared_ptr<SerializableDataSource> editable)
    {
        if (editable == 0x0)
            return;
        xSpin_->blockSignals(true);
        ySpin_->blockSignals(true);

        Vector2 vec = editable->GetObject()->GetAttribute(property_).GetVector2();
        xSpin_->setValue(vec.x_);
        ySpin_->setValue(vec.y_);

        xSpin_->blockSignals(false);
        ySpin_->blockSignals(false);
    }

    Vector3Editor::Vector3Editor(unsigned property) :
        BaseTypeEditor(property, false)
    {
        layout_->setSpacing(0);
        layout_->setAlignment(Qt::AlignLeft);
        CREATE_LBL(xLbl_, "X", XStyle, "");
        CREATE_LBL(yLbl_, "Y", YStyle, "");
        CREATE_LBL(zLbl_, "Z", ZStyle, "");

        xSpin_ = new QDoubleSpinBox();
        ySpin_ = new QDoubleSpinBox();
        zSpin_ = new QDoubleSpinBox();

        SET_RANGE_DBL(xSpin_);
        SET_RANGE_DBL(ySpin_);
        SET_RANGE_DBL(zSpin_);

        layout_->addWidget(xLbl_);
        layout_->addWidget(xSpin_);
        layout_->addItem(new QSpacerItem(4, 1));
        layout_->addWidget(yLbl_);
        layout_->addWidget(ySpin_);
        layout_->addItem(new QSpacerItem(4, 1));
        layout_->addWidget(zLbl_);
        layout_->addWidget(zSpin_);

        PropertyUpdated(editing_);

        connect(xSpin_, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &Vector3Editor::SpinHandler);
        connect(ySpin_, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &Vector3Editor::SpinHandler);
        connect(zSpin_, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &Vector3Editor::SpinHandler);
    }

    Urho3D::Variant Vector3Editor::GetValue()
    {
        return Urho3D::Vector3(xSpin_->value(), ySpin_->value(), zSpin_->value());
    }

    void Vector3Editor::SetValue(const Urho3D::Variant& value)
    {
        xSpin_->blockSignals(true);
        ySpin_->blockSignals(true);
        zSpin_->blockSignals(true);

        auto val = value.GetVector3();
        xSpin_->setValue(val.x_);
        ySpin_->setValue(val.y_);
        zSpin_->setValue(val.z_);

        xSpin_->blockSignals(false);
        ySpin_->blockSignals(false);
        zSpin_->blockSignals(false);
    }

    void Vector3Editor::SpinHandler(double)
    {
        Vector3 val(xSpin_->value(), ySpin_->value(), zSpin_->value());
        URHO_PROPERTY_UNDO(editing_, property_, val);
        //editing_->SetAttribute(property_, val);
        if (Urho3D::Node* nd = dynamic_cast<Urho3D::Node*>(editing_->GetObject().Get()))
            nd->MarkDirty();
        emit DataChanged();
    }

    void Vector3Editor::PropertyUpdated(std::shared_ptr<SerializableDataSource> editable)
    {
        if (editable == 0x0)
            return;
        xSpin_->blockSignals(true);
        ySpin_->blockSignals(true);
        zSpin_->blockSignals(true);

        Vector3 vec = editable->GetObject()->GetAttribute(property_).GetVector3();
        xSpin_->setValue(vec.x_);
        ySpin_->setValue(vec.y_);
        zSpin_->setValue(vec.z_);

        xSpin_->blockSignals(false);
        ySpin_->blockSignals(false);
        zSpin_->blockSignals(false);
    }

    Vector4Editor::Vector4Editor(unsigned property) :
        BaseTypeEditor(property, false)
    {
        layout_->setSpacing(0);
        layout_->setAlignment(Qt::AlignLeft);
        CREATE_LBL(xLbl_, "X", XStyle, "");
        CREATE_LBL(yLbl_, "Y", YStyle, "");
        CREATE_LBL(zLbl_, "Z", ZStyle, "");
        CREATE_LBL(wLbl_, "W", AStyle, "");

        xSpin_ = new QDoubleSpinBox();
        ySpin_ = new QDoubleSpinBox();
        zSpin_ = new QDoubleSpinBox();
        wSpin_ = new QDoubleSpinBox();

        SET_RANGE_DBL(xSpin_);
        SET_RANGE_DBL(ySpin_);
        SET_RANGE_DBL(zSpin_);
        SET_RANGE_DBL(wSpin_);

        layout_->addWidget(xLbl_);
        layout_->addWidget(xSpin_);
        layout_->addItem(new QSpacerItem(4, 1));
        layout_->addWidget(yLbl_);
        layout_->addWidget(ySpin_);
        layout_->addItem(new QSpacerItem(4, 1));
        layout_->addWidget(zLbl_);
        layout_->addWidget(zSpin_);
        layout_->addItem(new QSpacerItem(4, 1));
        layout_->addWidget(wLbl_);
        layout_->addWidget(wSpin_);

        PropertyUpdated(editing_);

        connect(xSpin_, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &Vector4Editor::SpinHandler);
        connect(ySpin_, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &Vector4Editor::SpinHandler);
        connect(zSpin_, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &Vector4Editor::SpinHandler);
        connect(wSpin_, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &Vector4Editor::SpinHandler);
    }

    Urho3D::Variant Vector4Editor::GetValue()
    {
        return Urho3D::Vector4(xSpin_->value(), ySpin_->value(), zSpin_->value(), wSpin_->value());
    }

    void Vector4Editor::SetValue(const Urho3D::Variant& value)
    {
        xSpin_->blockSignals(true);
        ySpin_->blockSignals(true);
        zSpin_->blockSignals(true);
        wSpin_->blockSignals(true);

        auto val = value.GetVector4();
        xSpin_->setValue(val.x_);
        ySpin_->setValue(val.y_);
        zSpin_->setValue(val.z_);
        wSpin_->setValue(val.w_);

        xSpin_->blockSignals(false);
        ySpin_->blockSignals(false);
        zSpin_->blockSignals(false);
        wSpin_->blockSignals(false);
    }

    void Vector4Editor::SpinHandler(double)
    {
        Vector4 val(xSpin_->value(), ySpin_->value(), zSpin_->value(), wSpin_->value());
        URHO_PROPERTY_UNDO(editing_, property_, val);
        //editing_->SetAttribute(property_, val);
        emit DataChanged();
    }

    void Vector4Editor::PropertyUpdated(std::shared_ptr<SerializableDataSource> editable)
    {
        if (editable == 0x0)
            return;
        xSpin_->blockSignals(true);
        ySpin_->blockSignals(true);
        zSpin_->blockSignals(true);
        wSpin_->blockSignals(true);

        Vector4 vec = editable->GetObject()->GetAttribute(property_).GetVector4();
        xSpin_->setValue(vec.x_);
        ySpin_->setValue(vec.y_);
        zSpin_->setValue(vec.z_);
        wSpin_->setValue(vec.w_);

        xSpin_->blockSignals(false);
        ySpin_->blockSignals(false);
        zSpin_->blockSignals(false);
        wSpin_->blockSignals(false);
    }

    QuatEditor::QuatEditor(unsigned property) :
        BaseTypeEditor(property, false)
    {
        layout_->setSpacing(0);
        layout_->setAlignment(Qt::AlignLeft);
        CREATE_LBL(xLbl_, "X", XStyle, "");
        CREATE_LBL(yLbl_, "Y", YStyle, "");
        CREATE_LBL(zLbl_, "Z", ZStyle, "");

        xSpin_ = new QDoubleSpinBox();
        ySpin_ = new QDoubleSpinBox();
        zSpin_ = new QDoubleSpinBox();

        SET_RANGE_DBL(xSpin_);
        SET_RANGE_DBL(ySpin_);
        SET_RANGE_DBL(zSpin_);

        layout_->addWidget(xLbl_);
        layout_->addWidget(xSpin_);
        layout_->addItem(new QSpacerItem(4, 1));
        layout_->addWidget(yLbl_);
        layout_->addWidget(ySpin_);
        layout_->addItem(new QSpacerItem(4, 1));
        layout_->addWidget(zLbl_);
        layout_->addWidget(zSpin_);

        PropertyUpdated(editing_);

        connect(xSpin_, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &QuatEditor::SpinHandler);
        connect(ySpin_, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &QuatEditor::SpinHandler);
        connect(zSpin_, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &QuatEditor::SpinHandler);
    }

    void QuatEditor::SpinHandler(double)
    {
        Quaternion quat;
        quat.FromEulerAngles(xSpin_->value(), ySpin_->value(), zSpin_->value());
        URHO_PROPERTY_UNDO(editing_, property_, quat);
        //editing_->SetAttribute(property_, quat);
        emit DataChanged();
    }

    Urho3D::Variant QuatEditor::GetValue()
    {
        Quaternion quat;
        quat.FromEulerAngles(xSpin_->value(), ySpin_->value(), zSpin_->value());
        return quat;
    }

    void QuatEditor::SetValue(const Urho3D::Variant& value)
    {
        xSpin_->blockSignals(true);
        ySpin_->blockSignals(true);
        zSpin_->blockSignals(true);

        Quaternion quat = value.GetQuaternion();
        Vector3 vec = quat.EulerAngles();
        xSpin_->setValue(vec.x_);
        ySpin_->setValue(vec.y_);
        zSpin_->setValue(vec.z_);

        xSpin_->blockSignals(false);
        ySpin_->blockSignals(false);
        zSpin_->blockSignals(false);
    }

    void QuatEditor::PropertyUpdated(std::shared_ptr<SerializableDataSource> editable)
    {
        if (editable == 0x0)
            return;
        SetValue(editable->GetObject()->GetAttribute(property_));
    }

}