#include "UndoListControl.h"

namespace SprueEditor
{

    UndoListControl::UndoListControl()
    {
        setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        connect(this, &QListWidget::itemSelectionChanged, this, &UndoListControl::SelectionChanged);
    }

    UndoListControl::~UndoListControl()
    {

    }

    void UndoListControl::SetUndoStack(SmartUndoStack* newStack)
    {
        if (newStack == stack_)
            return;

        if (stack_)
        {
            disconnect(stack_, &SmartUndoStack::ActionRedone, this, &UndoListControl::ActionUndone);
            disconnect(stack_, &SmartUndoStack::ActionRedone, this, &UndoListControl::ActionRedone);
            disconnect(stack_, &SmartUndoStack::ActionBounced, this, &UndoListControl::ActionBounced);
            disconnect(stack_, &SmartUndoStack::ActionMerged, this, &UndoListControl::ActionMerged);
            disconnect(stack_, &SmartUndoStack::ActionAdded, this, &UndoListControl::ActionAdded);
        }
        stack_ = newStack;
        FillControl();
        if (stack_)
        {
            connect(stack_, &SmartUndoStack::ActionUndone, this, &UndoListControl::ActionUndone);
            connect(stack_, &SmartUndoStack::ActionRedone, this, &UndoListControl::ActionRedone);
            connect(stack_, &SmartUndoStack::ActionBounced, this, &UndoListControl::ActionBounced);
            connect(stack_, &SmartUndoStack::ActionMerged, this, &UndoListControl::ActionMerged);
            connect(stack_, &SmartUndoStack::ActionAdded, this, &UndoListControl::ActionAdded);

            // Make sure we're on the correct row
            blockSignals(true);
                setCurrentRow(stack_->GetCurrent() + 1); // need +1 to account for "< empty >" item.
            blockSignals(false);
        }

        emit UndoStackChanged(stack_);
    }

    void UndoListControl::ActionAdded(SmartCommand* cmd)
    {
        blockSignals(true);
        FillControl();
        blockSignals(false);
    }

    void UndoListControl::ActionBounced(SmartCommand* cmd)
    {
        blockSignals(true);
        for (int row = 0; row < count(); row++)
        {
            QListWidgetItem* cur = item(row);
            SmartCommand* thisCmd = ((SmartCommand*)cur->data(Qt::UserRole).value<void*>());
            if (thisCmd == cmd)
                delete cur;
        }
        ActionUndone();
        blockSignals(false);
    }

    void UndoListControl::ActionMerged(SmartCommand* cmd)
    {
        blockSignals(true);
        for (int row = 0; row < count(); row++)
        {
            QListWidgetItem* cur = item(row);
            SmartCommand* thisCmd = ((SmartCommand*)cur->data(Qt::UserRole).value<void*>());
            if (thisCmd == cmd)
            {
                cur->setText(thisCmd->GetText());
                break;
            }
        }
        ActionUndone();
        blockSignals(false);
    }

    void UndoListControl::IndexChanged(int index)
    {
        blockSignals(true);

        blockSignals(false);
    }

    void UndoListControl::ActionUndone()
    {
        if (!stack_)
            return;
        blockSignals(true);
        if (QListWidgetItem* it = item(stack_->GetCurrent() + 1))
            it->setSelected(true);
        else
            item(0)->setSelected(true);
        blockSignals(false);
    }

    void UndoListControl::ActionRedone()
    {
        ActionUndone();
    }

    void UndoListControl::FillControl()
    {
        blockSignals(true);
            clear();
        blockSignals(false);

        if (stack_)
        {
            QListWidgetItem* emptyItem = new QListWidgetItem(tr("< empty >"));
            addItem(emptyItem);
            for (int i = 0; i < stack_->GetCount(); ++i)
            {
                SmartCommand* cmd = stack_->GetCommand(i);
                QListWidgetItem* item = new QListWidgetItem(cmd->GetText());
                QVariant data;
                data.setValue<void*>(cmd);
                item->setData(Qt::UserRole, data);
                addItem(item);

                if (stack_->GetCurrent() == i)
                    item->setSelected(true);
            }
        }
    }

    void UndoListControl::SelectionChanged()
    {
        auto indices = selectedIndexes();
        if (indices.size())
        {
            lastSelected_ = item(indices[0].row());
            stack_->SetCurrent(indices[0].row()-1);
        }
        else
            lastSelected_->setSelected(true);
    }
}