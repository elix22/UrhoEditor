#include "UrhoMatrixEditors.h"

#include "../../Commands/Urho/UrhoPropertyCmd.h"

using namespace Urho3D;

#include <Urho3D/Math/Matrix3.h>

namespace UrhoUI
{

#define IMPL_MATRIX_EDIT(LAYOUT, ADDRX, ADDRY, WIDTH) spinners_[ADDRX + ADDRY*WIDTH] = new QDoubleSpinBox(); \
    spinners_[ADDRX + ADDRY*WIDTH]->setDecimals(4); \
    spinners_[ADDRX + ADDRY*WIDTH]->setMinimum(FLT_MIN); \
    spinners_[ADDRX + ADDRY*WIDTH]->setMaximum(FLT_MAX); \
    spinners_[ADDRX + ADDRY*WIDTH]->setMaximumWidth(100); \
    LAYOUT->addWidget(spinners_[ADDRX + ADDRY*WIDTH]);

#define SET_MAT(ADDRX, ADDRY, WIDTH) spinners_[ADDRX + ADDRY * WIDTH]->blockSignals(true); \
        spinners_[ADDRX + ADDRY * WIDTH] ->setValue(val.m  ## ADDRX ## ADDRY ## _); \
        spinners_[ADDRX + ADDRY * WIDTH]->blockSignals(false);

#define GET_MAT(ADDRX, ADDRY, WIDTH) val.m  ## ADDRX ## ADDRY ## _ = spinners_[ADDRX + ADDRY * WIDTH] -> value()
#define CONNECT_MAT(ADDRX,ADDRY, WIDTH, HANDLER) connect(spinners_[ADDRX + ADDRY * WIDTH], static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, HANDLER)

    Matrix3Editor::Matrix3Editor(unsigned property) :
        BaseTypeEditor(property, true)
    {
        layout_->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        spinners_ = new QDoubleSpinBox*[3 * 3];

        for (int i = 0; i < 3; ++i)
        {
            QHBoxLayout* layout = new QHBoxLayout();
            layout->setSpacing(0);
            IMPL_MATRIX_EDIT(layout, i, 0, 3);
            IMPL_MATRIX_EDIT(layout, i, 1, 3);
            IMPL_MATRIX_EDIT(layout, i, 2, 3);
            layout_->addLayout(layout);
        }

        PropertyUpdated(editing_);
        
        CONNECT_MAT(0, 0, 3, &Matrix3Editor::SpinHandler);
        CONNECT_MAT(0, 1, 3, &Matrix3Editor::SpinHandler);
        CONNECT_MAT(0, 2, 3, &Matrix3Editor::SpinHandler);
        CONNECT_MAT(1, 0, 3, &Matrix3Editor::SpinHandler);
        CONNECT_MAT(1, 1, 3, &Matrix3Editor::SpinHandler);
        CONNECT_MAT(1, 2, 3, &Matrix3Editor::SpinHandler);
        CONNECT_MAT(2, 0, 3, &Matrix3Editor::SpinHandler);
        CONNECT_MAT(2, 1, 3, &Matrix3Editor::SpinHandler);
        CONNECT_MAT(2, 2, 3, &Matrix3Editor::SpinHandler);
    }

    Urho3D::Variant Matrix3Editor::GetValue()
    {
        Matrix3 val;
        GET_MAT(0, 0, 3);
        GET_MAT(0, 1, 3);
        GET_MAT(0, 2, 3);
        GET_MAT(1, 0, 3);
        GET_MAT(1, 1, 3);
        GET_MAT(1, 2, 3);
        GET_MAT(2, 0, 3);
        GET_MAT(2, 1, 3);
        GET_MAT(2, 2, 3);
        return val;
    }

    void Matrix3Editor::SetValue(const Urho3D::Variant& value)
    {
        Matrix3 val = value.GetMatrix3();
        SET_MAT(0, 0, 3);
        SET_MAT(0, 1, 3);
        SET_MAT(0, 2, 3);
        SET_MAT(1, 0, 3);
        SET_MAT(1, 1, 3);
        SET_MAT(1, 2, 3);
        SET_MAT(2, 0, 3);
        SET_MAT(2, 1, 3);
        SET_MAT(2, 2, 3);
    }

    void Matrix3Editor::SpinHandler(double value)
    {
        Matrix3 val;
        GET_MAT(0, 0, 3);
        GET_MAT(0, 1, 3);
        GET_MAT(0, 2, 3);
        GET_MAT(1, 0, 3);
        GET_MAT(1, 1, 3);
        GET_MAT(1, 2, 3);
        GET_MAT(2, 0, 3);
        GET_MAT(2, 1, 3);
        GET_MAT(2, 2, 3);
        URHO_PROPERTY_UNDO(editing_, property_, val);
        //editing_->SetAttribute(property_, val);

        emit DataChanged();
    }

    void Matrix3Editor::PropertyUpdated(std::shared_ptr<SerializableDataSource> editing)
    {
        if (editing == 0x0)
            return;

        Matrix3 val = editing->GetObject()->GetAttribute(property_).GetMatrix3();
        SET_MAT(0, 0, 3);
        SET_MAT(0, 1, 3);
        SET_MAT(0, 2, 3);
        SET_MAT(1, 0, 3);
        SET_MAT(1, 1, 3);
        SET_MAT(1, 2, 3);
        SET_MAT(2, 0, 3);
        SET_MAT(2, 1, 3);
        SET_MAT(2, 2, 3);
    }

    Matrix3x4Editor::Matrix3x4Editor(unsigned property) :
        BaseTypeEditor(property, true)
    {
        spinners_ = new QDoubleSpinBox*[3 * 4];

        for (int i = 0; i < 3; ++i)
        {
            QHBoxLayout* layout = new QHBoxLayout();
            layout->setSpacing(0);
            IMPL_MATRIX_EDIT(layout, i, 0, 4);
            IMPL_MATRIX_EDIT(layout, i, 1, 4);
            IMPL_MATRIX_EDIT(layout, i, 2, 4);
            IMPL_MATRIX_EDIT(layout, i, 3, 4);
            layout_->addLayout(layout);
        }

        PropertyUpdated(editing_);

        CONNECT_MAT(0, 0, 4, &Matrix3x4Editor::SpinHandler);
        CONNECT_MAT(0, 1, 4, &Matrix3x4Editor::SpinHandler);
        CONNECT_MAT(0, 2, 4, &Matrix3x4Editor::SpinHandler);
        CONNECT_MAT(0, 3, 4, &Matrix3x4Editor::SpinHandler);
        CONNECT_MAT(1, 0, 4, &Matrix3x4Editor::SpinHandler);
        CONNECT_MAT(1, 1, 4, &Matrix3x4Editor::SpinHandler);
        CONNECT_MAT(1, 2, 4, &Matrix3x4Editor::SpinHandler);
        CONNECT_MAT(1, 3, 4, &Matrix3x4Editor::SpinHandler);
        CONNECT_MAT(2, 0, 4, &Matrix3x4Editor::SpinHandler);
        CONNECT_MAT(2, 1, 4, &Matrix3x4Editor::SpinHandler);
        CONNECT_MAT(2, 2, 4, &Matrix3x4Editor::SpinHandler);
        CONNECT_MAT(2, 3, 4, &Matrix3x4Editor::SpinHandler);
    }

    Urho3D::Variant Matrix3x4Editor::GetValue()
    {
        Matrix3x4 val;
        GET_MAT(0, 0, 4);
        GET_MAT(0, 1, 4);
        GET_MAT(0, 2, 4);
        GET_MAT(0, 3, 4);
        GET_MAT(1, 0, 4);
        GET_MAT(1, 1, 4);
        GET_MAT(1, 2, 4);
        GET_MAT(1, 3, 4);
        GET_MAT(2, 0, 4);
        GET_MAT(2, 1, 4);
        GET_MAT(2, 2, 4);
        GET_MAT(2, 3, 4);
        return val;
    }

    void Matrix3x4Editor::SetValue(const Urho3D::Variant& value)
    {
        Matrix3x4 val = value.GetMatrix3x4();
        SET_MAT(0, 0, 4);
        SET_MAT(0, 1, 4);
        SET_MAT(0, 2, 4);
        SET_MAT(0, 3, 4);
        SET_MAT(1, 0, 4);
        SET_MAT(1, 1, 4);
        SET_MAT(1, 2, 4);
        SET_MAT(1, 3, 4);
        SET_MAT(2, 0, 4);
        SET_MAT(2, 1, 4);
        SET_MAT(2, 2, 4);
        SET_MAT(2, 3, 4);
    }

    void Matrix3x4Editor::SpinHandler(double)
    {
        Matrix3x4 val;
        GET_MAT(0, 0, 4);
        GET_MAT(0, 1, 4);
        GET_MAT(0, 2, 4);
        GET_MAT(0, 3, 4);
        GET_MAT(1, 0, 4);
        GET_MAT(1, 1, 4);
        GET_MAT(1, 2, 4);
        GET_MAT(1, 3, 4);
        GET_MAT(2, 0, 4);
        GET_MAT(2, 1, 4);
        GET_MAT(2, 2, 4);
        GET_MAT(2, 3, 4);
        URHO_PROPERTY_UNDO(editing_, property_, val);
        //editing_->SetAttribute(property_, val);

        emit DataChanged();
    }

    void Matrix3x4Editor::PropertyUpdated(std::shared_ptr<SerializableDataSource> editing)
    {
        if (editing == 0x0)
            return;
        Matrix3x4 val = editing->GetObject()->GetAttribute(property_).GetMatrix3x4();
        SET_MAT(0, 0, 4);
        SET_MAT(0, 1, 4);
        SET_MAT(0, 2, 4);
        SET_MAT(0, 3, 4);
        SET_MAT(1, 0, 4);
        SET_MAT(1, 1, 4);
        SET_MAT(1, 2, 4);
        SET_MAT(1, 3, 4);
        SET_MAT(2, 0, 4);
        SET_MAT(2, 1, 4);
        SET_MAT(2, 2, 4);
        SET_MAT(2, 3, 4);
    }

    Matrix4x4Editor::Matrix4x4Editor(unsigned property) :
        BaseTypeEditor(property, true)
    {
        spinners_ = new QDoubleSpinBox*[3 * 4];

        for (int i = 0; i < 4; ++i)
        {
            QHBoxLayout* layout = new QHBoxLayout();
            layout->setSpacing(0);
            IMPL_MATRIX_EDIT(layout, i, 0, 4);
            IMPL_MATRIX_EDIT(layout, i, 1, 4);
            IMPL_MATRIX_EDIT(layout, i, 2, 4);
            IMPL_MATRIX_EDIT(layout, i, 3, 4);
            layout_->addLayout(layout);
        }

        PropertyUpdated(editing_);

        CONNECT_MAT(0, 0, 4, &Matrix4x4Editor::SpinHandler);
        CONNECT_MAT(0, 1, 4, &Matrix4x4Editor::SpinHandler);
        CONNECT_MAT(0, 2, 4, &Matrix4x4Editor::SpinHandler);
        CONNECT_MAT(0, 3, 4, &Matrix4x4Editor::SpinHandler);
        CONNECT_MAT(1, 0, 4, &Matrix4x4Editor::SpinHandler);
        CONNECT_MAT(1, 1, 4, &Matrix4x4Editor::SpinHandler);
        CONNECT_MAT(1, 2, 4, &Matrix4x4Editor::SpinHandler);
        CONNECT_MAT(1, 3, 4, &Matrix4x4Editor::SpinHandler);
        CONNECT_MAT(2, 0, 4, &Matrix4x4Editor::SpinHandler);
        CONNECT_MAT(2, 1, 4, &Matrix4x4Editor::SpinHandler);
        CONNECT_MAT(2, 2, 4, &Matrix4x4Editor::SpinHandler);
        CONNECT_MAT(2, 3, 4, &Matrix4x4Editor::SpinHandler);
        CONNECT_MAT(3, 0, 4, &Matrix4x4Editor::SpinHandler);
        CONNECT_MAT(3, 1, 4, &Matrix4x4Editor::SpinHandler);
        CONNECT_MAT(3, 2, 4, &Matrix4x4Editor::SpinHandler);
        CONNECT_MAT(3, 3, 4, &Matrix4x4Editor::SpinHandler);
    }

    Urho3D::Variant Matrix4x4Editor::GetValue()
    {
        Matrix4 val;
        GET_MAT(0, 0, 4);
        GET_MAT(0, 1, 4);
        GET_MAT(0, 2, 4);
        GET_MAT(0, 3, 4);
        GET_MAT(1, 0, 4);
        GET_MAT(1, 1, 4);
        GET_MAT(1, 2, 4);
        GET_MAT(1, 3, 4);
        GET_MAT(2, 0, 4);
        GET_MAT(2, 1, 4);
        GET_MAT(2, 2, 4);
        GET_MAT(2, 3, 4);
        GET_MAT(3, 0, 4);
        GET_MAT(3, 1, 4);
        GET_MAT(3, 2, 4);
        GET_MAT(3, 3, 4);
        return val;
    }

    void Matrix4x4Editor::SetValue(const Urho3D::Variant& value)
    {
        Matrix4 val = value.GetMatrix4();
        SET_MAT(0, 0, 4);
        SET_MAT(0, 1, 4);
        SET_MAT(0, 2, 4);
        SET_MAT(0, 3, 4);
        SET_MAT(1, 0, 4);
        SET_MAT(1, 1, 4);
        SET_MAT(1, 2, 4);
        SET_MAT(1, 3, 4);
        SET_MAT(2, 0, 4);
        SET_MAT(2, 1, 4);
        SET_MAT(2, 2, 4);
        SET_MAT(2, 3, 4);
        SET_MAT(3, 0, 4);
        SET_MAT(3, 1, 4);
        SET_MAT(3, 2, 4);
        SET_MAT(3, 3, 4);
    }

    void Matrix4x4Editor::SpinHandler(double)
    {
        Matrix4 val;
        GET_MAT(0, 0, 4);
        GET_MAT(0, 1, 4);
        GET_MAT(0, 2, 4);
        GET_MAT(0, 3, 4);
        GET_MAT(1, 0, 4);
        GET_MAT(1, 1, 4);
        GET_MAT(1, 2, 4);
        GET_MAT(1, 3, 4);
        GET_MAT(2, 0, 4);
        GET_MAT(2, 1, 4);
        GET_MAT(2, 2, 4);
        GET_MAT(2, 3, 4);
        GET_MAT(3, 0, 4);
        GET_MAT(3, 1, 4);
        GET_MAT(3, 2, 4);
        GET_MAT(3, 3, 4);
        URHO_PROPERTY_UNDO(editing_, property_, val);
        //editing_->SetAttribute(property_, val);

        emit DataChanged();
    }

    void Matrix4x4Editor::PropertyUpdated(std::shared_ptr<SerializableDataSource> editing)
    {
        if (editing == 0x0)
            return;
        Matrix4 val = editing->GetObject()->GetAttribute(property_).GetMatrix4();
        SET_MAT(0, 0, 4);
        SET_MAT(0, 1, 4);
        SET_MAT(0, 2, 4);
        SET_MAT(0, 3, 4);
        SET_MAT(1, 0, 4);
        SET_MAT(1, 1, 4);
        SET_MAT(1, 2, 4);
        SET_MAT(1, 3, 4);
        SET_MAT(2, 0, 4);
        SET_MAT(2, 1, 4);
        SET_MAT(2, 2, 4);
        SET_MAT(2, 3, 4);
        SET_MAT(3, 0, 4);
        SET_MAT(3, 1, 4);
        SET_MAT(3, 2, 4);
        SET_MAT(3, 3, 4);
    }
}