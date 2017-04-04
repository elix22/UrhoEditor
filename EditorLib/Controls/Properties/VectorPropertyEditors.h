#pragma once

#include <EditorLib/Controls/Properties/BasePropertyEditor.h>

class QLabel;
class QSpinBox;
class QDoubleSpinBox;

/// Property editor that works with QPoint (integral)
class IntVector2PropertyEditor : public BasePropertyEditor
{
    Q_OBJECT;
public:
    IntVector2PropertyEditor();

    virtual void SetValue(const QVariant& value) override;
    virtual QVariant GetValue() const override;

    QLabel* xLbl_;
    QLabel* yLbl_;
    QSpinBox* xSpin_;
    QSpinBox* ySpin_;

public slots:
    void SpinHandler(int);
};

/// Property editor that works with QVector2D
class Vector2PropertyEditor : public BasePropertyEditor
{
    Q_OBJECT;
public:
    Vector2PropertyEditor();

    virtual void SetValue(const QVariant& value) override;
    virtual QVariant GetValue() const override;

    QLabel* xLbl_;
    QLabel* yLbl_;
    QDoubleSpinBox* xSpin_;
    QDoubleSpinBox* ySpin_;

public slots:
    void SpinHandler(double);
};

/// Property editor that works with QVector3D
class Vector3PropertyEditor : public BasePropertyEditor
{
    Q_OBJECT;
public:
    Vector3PropertyEditor();

    virtual void PropertyUpdated(std::shared_ptr<DataSource>);

    virtual void SetValue(const QVariant& value) override;
    virtual QVariant GetValue() const override;

    QLabel* xLbl_;
    QLabel* yLbl_;
    QLabel* zLbl_;
    QDoubleSpinBox* xSpin_;
    QDoubleSpinBox* ySpin_;
    QDoubleSpinBox* zSpin_;

public slots:
    void SpinHandler(double);
};

/// Property editor that works with QQuaternion, displays values as pitch, yaw, roll
class QuatPropertyEditor : public BasePropertyEditor
{
    Q_OBJECT;
public:
    QuatPropertyEditor();

    virtual void PropertyUpdated(std::shared_ptr<DataSource>);

    virtual void SetValue(const QVariant& value) override;
    virtual QVariant GetValue() const override;

    QLabel* xLbl_;
    QLabel* yLbl_;
    QLabel* zLbl_;
    QDoubleSpinBox* xSpin_;
    QDoubleSpinBox* ySpin_;
    QDoubleSpinBox* zSpin_;

public slots:
    void SpinHandler(double);
};