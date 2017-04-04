#pragma once

#include "../Controls/BasePropertyPage.h"

#include <EditorLib/ScrollAreaWidget.h>
#include <EditorLib/Selectron.h>

#include <QBoxLayout>
#include <qstackedlayout.h>

#include <set>
#include <memory>

namespace SprueEngine
{
    class IEditable;
    class TypeProperty;
    class Variant;
}

class QLabel;
class QFormLayout;

namespace SprueEditor
{
    class BaseTypeEditor;

    typedef unsigned(*TYPE_ID_OBJECT_TO_HASH)(std::shared_ptr<DataSource>);

    /// Manages a collection or property pages, displaying the correct one when a given object is selected.
    class PropertyPanel : public ScrollAreaWidget, public SelectronLinked
    {
        Q_OBJECT
    public:
        PropertyPanel(std::vector< std::pair<unsigned, BasePropertyPage*> >& pages, TYPE_ID_OBJECT_TO_HASH hashTranslator);
        virtual ~PropertyPanel();

        void SetEditable(std::shared_ptr<DataSource> editable);

        virtual void Link(Selectron* sel) override;

        QStackedLayout* GetPropertyStack() { return propertyPages_; }

    protected:
        void UpdateControls();

        virtual void showEvent(QShowEvent*) Q_DECL_OVERRIDE;
        virtual void hideEvent(QHideEvent*) Q_DECL_OVERRIDE;

    private:
        std::shared_ptr<DataSource> editing_;
        TYPE_ID_OBJECT_TO_HASH hashTranslator_;

        QStackedLayout* propertyPages_ = 0x0;
        // Table of type hashes containing list of all type editors contained in the page
        std::map<unsigned, BasePropertyPage*> typeEditors_;
        // Table of Type hashes to indices for above property pages
        std::map<unsigned, int> typeLayouts_;

    private:
        /// Primary property panel follows standard behaviour when closed. Secondary property-panels are destroyed when their dock is closed.
        bool isPrimary_ = true;
        /// Utility tracker for hidden status to prevent update emission into property pages when not visible.
        bool hidden_ = true;
    };

}