#include "UrhoDockBuilder.h"

#include "../../GlobalAccess.h"
#include "../../UrhoEditor.h"
#include "../../Views/ViewManager.h"
#include "../../InternalNames.h"
#include "../../Localization/LocalizedWidgets.h"

#include "Controls/UrhoObjectTree.h"

#include <EditorLib/DocumentBase.h>
#include <EditorLib/DocumentManager.h>

#include <QDockWidget>

namespace SprueEditor
{

    void UrhoDockBuilder::Build(UrhoEditor* editor)
    {
        QDockWidget* urhoSceneDock = editor->GetOrCreateDock(URHO_SCENE_TREE_PANEL, "Urho3D Scene Tree", "ALT+U");
        UrhoUI::UrhoObjectTree* sceneTree = new UrhoUI::UrhoObjectTree(false);
        sceneTree->SetSelectron(editor->GetObjectSelectron());
        sceneTree->setObjectName(URHO_SCENE_TREE_CONTROL);
        urhoSceneDock->setWidget(sceneTree);
        
        //QDockWidget* urhoUIDock = editor->GetOrCreateDock(URHO_UI_TREE_PANEL, "Urho3D UI Tree", "");
        //UrhoEditor::UrhoObjectTree* uiTree = new UrhoEditor::UrhoObjectTree(true);
        //uiTree->setObjectName(URHO_UI_TREE_CONTROL);
        //urhoUIDock->setWidget(uiTree);

        connect(editor->GetViewManager(), &ViewManager::ViewChanged, [=](ViewBase* view) {
            if (view)
                sceneTree->SetModel(view->GetScene());
            else
                sceneTree->SetModel(0x0);
        });

        DocumentManager* docMan = Global_DocumentManager();
        connect(docMan, &DocumentManager::ActiveDocumentChanged, [=](DocumentBase* newDoc, DocumentBase* oldDoc) {
            /// If we have a view then set the new scene, otherwise set to null
            if (std::shared_ptr<ViewBase> view = editor->GetViewManager()->GetActiveView())
                sceneTree->SetModel(view->GetScene());
            else
                sceneTree->SetModel(0x0);
        });
    }

}