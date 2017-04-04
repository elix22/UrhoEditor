#include "DocumentSearchResult.h"

#include "../GlobalAccess.h"
#include "../UrhoEditor.h"

#include <EditorLib/DocumentManager.h>

namespace SprueEditor
{

    DocumentSearchResult::DocumentSearchResult(std::shared_ptr<DataSource> dataSource, std::shared_ptr<DocumentBase> document)
    {
        dataSource_ = dataSource;
        document_ = document;
    }

    void DocumentSearchResult::GoTo()
    {
        if (document_ && dataSource_)
        {
            if (Global_DocumentManager()->GetActiveDocument() != document_.get())
                Global_DocumentManager()->SetActiveDocument(document_.get());
            UrhoEditor::GetInstance()->GetObjectSelectron()->SetSelected(0x0, dataSource_);
        }
    }

}