#include "Selectron.h"


void Selectron::SetSelected(void* who, std::shared_ptr<DataSource> sel)
{
    if (!selected_.empty())
        selected_.clear();
    if (sel) // don't actually add a selection if we'd be adding 0x0/null
        AddSelected(who, sel);
    else
        emit SelectionChanged(who, this);
}

bool Selectron::AddSelected(void* who, std::shared_ptr<DataSource> sel)
{
    for (unsigned i = 0; i < selected_.size(); ++i)
    {
        if (selected_[i]->Equals(sel.get()))
            return false;
    }
    selected_.push_back(sel);
    emit SelectionChanged(who, this);
    return true;
}

void Selectron::RemoveSelected(void* who, std::shared_ptr<DataSource> sel)
{
    auto foundData = std::find(selected_.begin(), selected_.end(), sel);
    if (foundData != selected_.end())
        selected_.erase(foundData);
    emit SelectionChanged(who, this);
}

void Selectron::ClearSelection(void* who)
{
    selected_.clear();
    emit SelectionChanged(who, this);
}

void Selectron::NotifyDataChanged(void* who, std::shared_ptr<DataSource> sel, unsigned hash)
{
    if (std::find(selected_.begin(), selected_.end(), sel) == selected_.end())
        SetSelected(who, sel);
    emit DataChanged(who, this, hash);
}

void Selectron::NotifyDataChangedSoft(void* who, std::shared_ptr<DataSource> sel, unsigned hash)
{
    emit DataChanged(who, this, hash);
}

bool Selectron::AllSelectedSameType() const
{
    if (selected_.size() > 1)
    {
        unsigned firstType = selected_.front()->GetID();
        for (auto it = selected_.begin() + 1; it != selected_.end(); ++it)
            if (firstType != it->get()->GetID())
                return false;
        return true;
    }
    else if (!selected_.empty())
        return true;
    return false;
}