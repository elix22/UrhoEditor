#pragma once

#include <QListWidget>

#include <EditorLib/Commands/SmartUndoStack.h>

namespace SprueEditor
{

    class UndoListControl : public QListWidget
    {
        Q_OBJECT
    public:
        UndoListControl();
        virtual ~UndoListControl();

        void SetUndoStack(SmartUndoStack* newStack);

    signals:
        void UndoStackChanged(SmartUndoStack*);

    private slots:
        void ActionAdded(SmartCommand*);
        void ActionBounced(SmartCommand*);
        void ActionMerged(SmartCommand*);
        void IndexChanged(int index);
        void ActionUndone();
        void ActionRedone();

        void SelectionChanged();

    private:
        void FillControl();

        SmartUndoStack* stack_ = 0x0;
        QListWidgetItem* lastSelected_ = 0x0;
    };

}