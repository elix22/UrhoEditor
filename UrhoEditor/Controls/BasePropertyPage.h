#pragma once

#include <EditorLib/DataSource.h>
#include <EditorLib/ScrollAreaWidget.h>
#include <EditorLib/Selectron.h>

namespace SprueEditor
{

    /// Baseclass for property pages that appear in the property panel
    /// Concrete implementations exist for Urho3D 
    class BasePropertyPage : public ScrollAreaWidget, public SelectronLinked
    {
    public:
        BasePropertyPage() : 
            ScrollAreaWidget()
        { 
        }
        virtual ~BasePropertyPage() { }

        virtual void SetEditing(std::shared_ptr<DataSource> editing) = 0;
        virtual void SelectionChanged(void* src, Selectron* sel) = 0;
        virtual void SelectronDataChanged(void* src, Selectron* sel, unsigned property) = 0;
    };

}