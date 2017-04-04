#include "SharedDockBuilder.h"

#include "../../Data/UrhoDataSources.h"

#include "../../UrhoEditor.h"
#include "../../InternalNames.h"
#include "../../Localization/LocalizedWidgets.h"
#include "../../Views/RenderWidget.h"
#include "../../Views/ViewManager.h"

#include "../../Controls/UndoListControl.h"
#include "Documents/Urho/Controls/UrhoPropertyPage.h"

#include "../../Panels/PropertyPanel.h"
#include "../../Panels/ResourcePanel.h"

#include <EditorLib/DocumentManager.h>
#include <EditorLib/Panels/DocumentsPanel.h>
#include <EditorLib/Controls/DataGridWidget.h>
#include <EditorLib/Panels/LogPanel.h>
#include <EditorLib/Controls/SearchResultList.h>

#include <Urho3D/Scene/Component.h>

#include <QDockWidget>
#include <QUndoView>

namespace SprueEditor
{

    unsigned PropertyObjectTypeHashHandler(std::shared_ptr<DataSource> object)
    {
        if (object != 0x0)
        {
            if (auto editable = std::dynamic_pointer_cast<UrhoUI::SerializableDataSource>(object))
                return editable->GetObject()->GetType().Value();
        }
        Urho3D::StringHash hash;
        return hash;
    }

    void SharedDockBuilder::Build(UrhoEditor* editor)
    {
        QDockWidget* logPanel = editor->GetOrCreateDock(LOG_PANEL, "Log", "ALT+L");
        LogPanel* log = new LogPanel();
        log->setObjectName(LOG_CONTROL);
        logPanel->setWidget(log);
        logPanel->setVisible(false);

        QDockWidget* documentsPanel = editor->GetOrCreateDock(DOCUMENTS_PANEL, "Documents", "ALT+D");
        DocumentsPanel* documents = new DocumentsPanel();
        documents->setObjectName(DOCUMENTS_CONTROL);
        documentsPanel->setWidget(documents);
        documentsPanel->setVisible(false);

        std::vector< std::pair<unsigned, BasePropertyPage* > > pages;

        Urho3D::Context* context = editor->GetRenderer()->GetViewManager()->GetContext();
        auto factories = context->GetObjectFactories();
        for (auto factoryItem = factories.Begin(); factoryItem != factories.End(); ++factoryItem)
        {
            auto attrList = context->GetAttributes(factoryItem->first_);
            if (attrList && attrList->Size())
            {
                pages.push_back(std::make_pair(factoryItem->first_.Value(), new UrhoPropertyPage(context, factoryItem->first_)));
            }
        }

        QDockWidget* propertyPanel = editor->GetOrCreateDock(PROPERTY_PANEL, "Properties", "ALT+P");
        PropertyPanel* properties = new PropertyPanel(pages, PropertyObjectTypeHashHandler);
        propertyPanel->setWidget(properties);
        editor->GetDockingWindow()->addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, propertyPanel);
        properties->setObjectName(PROPERTY_CONTROL);
        properties->SetSelectron(editor->GetObjectSelectron());
        connect(editor->GetObjectSelectron(), &Selectron::SelectionChanged, [=](void* src, Selectron* sel) {
            if (sel->GetSelectedCount())
            {
                if (auto object = sel->GetSelected<UrhoUI::SerializableDataSource>())
                {
                    Urho3D::Serializable* selected = object->GetObject();
                    if (Urho3D::Node* node = dynamic_cast<Urho3D::Node*>(selected))
                    {
                        if (node->GetName().Empty())
                            propertyPanel->setWindowTitle(QString("Properties - %1").arg(node->GetTypeName().CString()));
                        else
                            propertyPanel->setWindowTitle(QString("Properties - %1, %2").arg(node->GetTypeName().CString(), node->GetName().CString()));
                    }
                    else if (Urho3D::Component* comp = dynamic_cast<Urho3D::Component*>(selected))
                        propertyPanel->setWindowTitle(QString("Properties - %1").arg(comp->GetTypeName().CString()));
                }
                else
                    propertyPanel->setWindowTitle("Properties");
            }
            else
                propertyPanel->setWindowTitle("Properties");
        });

        QDockWidget* resourcePanel = editor->GetOrCreateDock(RESOURCE_BROWSER_PANEL, "Resource Browser", "ALT+R");
        ResourcePanel* resources = new ResourcePanel();
        resources->setObjectName(RESOURCE_BROWSER_CONTROL);
        resourcePanel->setWidget(resources);
        editor->GetDockingWindow()->addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, resourcePanel);
        resourcePanel->setVisible(false);

        QDockWidget* dataTable = editor->GetOrCreateDock(DATAGRID_PANEL, "Data Grid", "");
        DataGridWidget* grid = new DataGridWidget();
        
    // Setup grid handlers.
        grid->AddHandler(new UrhoUI::SerializableDataGridHandler());

        grid->setObjectName(DATAGRID_CONTROL);
        dataTable->setWidget(grid);
        editor->GetDockingWindow()->addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, dataTable);
        dataTable->setVisible(false);
        grid->SetSelectron(editor->GetObjectSelectron());

        QDockWidget* historyPanel = editor->GetOrCreateDock(HISTORY_PANEL, "History", "ALT+H");
        UndoListControl* undoList = new UndoListControl();
        undoList->setObjectName(HISTORY_CONTROL);
        historyPanel->setWidget(undoList);

        editor->GetDockingWindow()->addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, historyPanel);
        historyPanel->setVisible(false);

        connect(Global_DocumentManager(), &DocumentManager::ActiveDocumentChanged, [=](DocumentBase* newDoc, DocumentBase* oldDoc) {
            if (newDoc)
                undoList->SetUndoStack(newDoc->GetUndoStack());
            else
                undoList->SetUndoStack(0x0);
        });

        QDockWidget* searchPanel = editor->GetOrCreateDock(SEARCH_RESULTS_PANEL, "Search Results", std::string());
        SearchResultList* searchList = new SearchResultList();
        searchPanel->setWidget(searchList);
        editor->GetDockingWindow()->addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, searchPanel);
        searchPanel->setVisible(false);
    }

}