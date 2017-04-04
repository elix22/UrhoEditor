#pragma once

#include <EditorLib/Commands/SmartUndoStack.h>

#include <QString>

#include <vector>

class CompoundCommand : public SimpleSmartCommand
{
public:
    CompoundCommand(const QString& text, std::vector<SmartCommand*> commands);
    virtual ~CompoundCommand();

    virtual void Redo() override;
    virtual void Undo()  override;
    virtual void MadeCurrent() override;
    virtual int GetID() const override;
    virtual bool CanMergeWith(const SmartCommand* rhs) override;
    virtual bool MergeWith(const SmartCommand* rhs) override;
    virtual bool ShouldBounce() override;

protected:
    std::vector<SmartCommand*> commands_;
};