#include "UrhoColorEditors.h"

#include <qstring.h>
#include <QColorDialog>

#include "../../Commands/Urho/UrhoPropertyCmd.h"

using namespace Urho3D;

namespace UrhoUI
{

#define XStyle "QLabel { color: white; font-size: 8pt; font-weight: bold; background-color: #DD0000; border-top-left-radius: 5px; border-bottom-left-radius: 5px; border-bottom-right-radius: 0px; border-top-right-radius: 0px; }"
#define YStyle "QLabel { color: white; font-size: 8pt; font-weight: bold; background-color: #00AA00; border-top-left-radius: 5px; border-bottom-left-radius: 5px; border-bottom-right-radius: 0px; border-top-right-radius: 0px; }"
#define ZStyle "QLabel { color: white; font-size: 8pt; font-weight: bold; background-color: #3F3FDD; border-top-left-radius: 5px; border-bottom-left-radius: 5px; border-bottom-right-radius: 0px; border-top-right-radius: 0px; }"
#define AStyle "QLabel { color: white; font-size: 8pt; font-weight: bold; background-color: #AA007F; border-top-left-radius: 5px; border-bottom-left-radius: 5px; border-bottom-right-radius: 0px; border-top-right-radius: 0px; }"

#define CREATE_LBL(VARNAME, TEXT, STYLE, TIP) QLabel* VARNAME = new QLabel(" " TEXT); \
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

    RGBAEditor::RGBAEditor(unsigned property) :
        BaseTypeEditor(property, true)
    {
        QHBoxLayout* lay = new QHBoxLayout();
        lay->setSpacing(0);
        layout_->setAlignment(Qt::AlignLeft);

        CREATE_LBL(xLbl_, "R", XStyle, "");
        CREATE_LBL(yLbl_, "G", YStyle, "");
        CREATE_LBL(zLbl_, "B", ZStyle, "");
        CREATE_LBL(aLbl_, "A", AStyle, "");

        rSpin_ = new QDoubleSpinBox();
        gSpin_ = new QDoubleSpinBox();
        bSpin_ = new QDoubleSpinBox();
        aSpin_ = new QDoubleSpinBox();

        lay->addWidget(xLbl_);
        lay->addWidget(rSpin_);
        lay->addItem(new QSpacerItem(4, 1));
        lay->addWidget(yLbl_);
        lay->addWidget(gSpin_);
        lay->addItem(new QSpacerItem(4, 1));
        lay->addWidget(zLbl_);
        lay->addWidget(bSpin_);
        lay->addItem(new QSpacerItem(4, 1));
        lay->addWidget(aLbl_);
        lay->addWidget(aSpin_);

        layout_->addLayout(lay);

        colorButton_ = new QPushButton();
        colorButton_->setMaximumWidth(128);

        connect(colorButton_, &QPushButton::clicked, this, &RGBAEditor::ColorButtonHandler);

        layout_->addWidget(colorButton_);

        PropertyUpdated(editing_);

        connect(rSpin_, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &RGBAEditor::SpinHandler);
        connect(gSpin_, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &RGBAEditor::SpinHandler);
        connect(bSpin_, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &RGBAEditor::SpinHandler);
        connect(aSpin_, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &RGBAEditor::SpinHandler);
    }

    Urho3D::Variant RGBAEditor::GetValue()
    {
        return Urho3D::Color(rSpin_->value(), gSpin_->value(), bSpin_->value(), aSpin_->value());
    }

    void RGBAEditor::SetValue(const Urho3D::Variant& value)
    {
        rSpin_->blockSignals(true);
        gSpin_->blockSignals(true);
        bSpin_->blockSignals(true);
        aSpin_->blockSignals(true);

        Color color = value.GetColor();
        rSpin_->setValue(color.r_);
        gSpin_->setValue(color.g_);
        bSpin_->setValue(color.b_);
        aSpin_->setValue(color.a_);

        QColor col(color.r_ * 255, color.g_ * 255, color.b_ * 255, color.a_ * 255);
        QString style = QString("background-color: %1").arg(col.name());
        colorButton_->setStyleSheet(style);

        rSpin_->blockSignals(false);
        gSpin_->blockSignals(false);
        bSpin_->blockSignals(false);
        aSpin_->blockSignals(false);
    }

    void RGBAEditor::SpinHandler(double)
    {
        Color val(rSpin_->value(), gSpin_->value(), bSpin_->value(), aSpin_->value());
        //editing_->SetAttribute(property_, val);
        URHO_PROPERTY_UNDO(editing_, property_, val);

        QColor col(val.r_ * 255, val.g_ * 255, val.b_ * 255, val.a_ * 255);

        QString style = QString("background-color: %1").arg(col.name());
        colorButton_->setStyleSheet(style);

        emit DataChanged();
    }

    void RGBAEditor::ColorButtonHandler(bool)
    {
        Color c = editing_->GetObject()->GetAttribute(property_).GetColor();
        QColor curColor(c.r_ * 255, c.g_ * 255, c.b_ * 255, c.a_ * 255);

        QColor newColor = QColorDialog::getColor(curColor);
        if (newColor.isValid())
        {
            Color val(newColor.redF(), newColor.greenF(), newColor.blueF(), newColor.alphaF());
            URHO_PROPERTY_UNDO(editing_, property_, val);
            //editing_->SetAttribute(property_, val);

            PropertyUpdated(editing_); // will also update us

            emit DataChanged();
        }
    }

    void RGBAEditor::PropertyUpdated(std::shared_ptr<SerializableDataSource> editing)
    {
        if (editing == 0x0)
            return;

        rSpin_->blockSignals(true);
        gSpin_->blockSignals(true);
        bSpin_->blockSignals(true);
        aSpin_->blockSignals(true);

        Color val = editing->GetObject()->GetAttribute(property_).GetColor();
        QColor col(val.r_ * 255, val.g_ * 255, val.b_ * 255, val.a_ * 255);
        rSpin_->setValue(val.r_);
        gSpin_->setValue(val.g_);
        bSpin_->setValue(val.b_);
        aSpin_->setValue(val.a_);

        QString style = QString("background-color: %1").arg(col.name());
        colorButton_->setStyleSheet(style);

        rSpin_->blockSignals(false);
        gSpin_->blockSignals(false);
        bSpin_->blockSignals(false);
        aSpin_->blockSignals(false);
    }
}