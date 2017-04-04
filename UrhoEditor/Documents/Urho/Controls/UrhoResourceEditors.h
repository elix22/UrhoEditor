#pragma once

#include "UrhoBaseTypeEditor.h"

#include <EditorLib/Controls/PathPickerWidget.h>

namespace UrhoUI
{

    class ResourceRefEditor : public BaseTypeEditor
    {
        Q_OBJECT
    public:
        ResourceRefEditor(unsigned property);

        virtual Urho3D::Variant GetValue() override;
        virtual void SetValue(const Urho3D::Variant& value) override;

        virtual void PropertyUpdated(std::shared_ptr<SerializableDataSource>) override;

    public slots:
        void PathChanged(const PathPickerWidget*, const char*);

    private:
        PathPickerWidget* browser_;
    };

    class ResourceRefListEditor : public BaseTypeEditor
    {
        Q_OBJECT
    public:
        ResourceRefListEditor(unsigned property);

        virtual Urho3D::Variant GetValue() override;
        virtual void SetValue(const Urho3D::Variant& value) override;

        virtual void PropertyUpdated(std::shared_ptr<SerializableDataSource>) override;

    public slots:
        void PathChanged(const PathPickerWidget*, const char*);

    private:
        std::vector<PathPickerWidget*> widgets_;
    };
}