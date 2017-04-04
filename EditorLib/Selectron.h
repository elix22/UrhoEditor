#pragma once

#include <EditorLib/DataSource.h>

#include <QObject>

#include <memory>

/// A Selectron governs the selection status of something in a common place.
/// Used instead of having controls subscribing to each other all over the place.
class Selectron : public QObject
{
    Q_OBJECT;
public:
    Selectron()
    {
    }

    virtual ~Selectron()
    {
    }

    /// Return the datasource at the given index.
    std::shared_ptr<DataSource> Selected(unsigned idx = 0) const { return selected_.size() > idx ? selected_[idx] : 0x0; }

    /// Return the most recently selected datasource. Just the last element.
    std::shared_ptr<DataSource> MostRecentSelected() const { return selected_.size() > 0 ? selected_.back() : 0x0; }

    /// Returns the number of currently selected items.
    unsigned GetSelectedCount() const { return selected_.size(); }

    /// Return the selected datasource at the given index if it is a specific type.
    template<class T>
    std::shared_ptr<T> GetSelected(unsigned idx = 0) const {
        static_assert(std::is_base_of<DataSource, T>::value, "T must be derived from DataSource to use Selectron::GetSelected<T>()");
        return std::dynamic_pointer_cast<T>(Selected(idx));
    }

    /// Return the most recent datasource if it is of a given type.
    template<class T>
    std::shared_ptr<T> GetMostRecentSelected() const {
        static_assert(std::is_base_of<DataSource, T>::value, "T must be derived from DataSource to use Selectron::GetMostRecentSelected<T>()");
        return std::dynamic_pointer_cast<T>(MostRecentSelected());
    }

    /// Sets the singly selected datasource.
    void SetSelected(void* who, std::shared_ptr<DataSource> sel);

    /// Adds a data-source for multiple selections.
    bool AddSelected(void* who, std::shared_ptr<DataSource> sel);

    /// Removes a single item from the list of selected items.
    void RemoveSelected(void* who, std::shared_ptr<DataSource> sel);

    /// Completely wipes the selection list
    void ClearSelection(void* who);

    /// Inform all subscribers that the selection has changed (send the hash of the property as well [or zero])
    void NotifyDataChanged(void* who, std::shared_ptr<DataSource> sel, unsigned hash = 0);

    /// Similar to NotifyDataChanged but does not change the selection.
    void NotifyDataChangedSoft(void* who, std::shared_ptr<DataSource> sel, unsigned hash = 0);

    /// Returns ture if all selected objects have the same type (ie SELECTOR_IEDITABLE), will return false if nothing is selected.
    bool AllSelectedSameType() const;

signals:
    /// Emitted to notify that the selection has changed
    void SelectionChanged(void* source, Selectron* selectron);
    /// Emitted when the data for the selection is changed
    void DataChanged(void* source, Selectron* selectron, unsigned propertyHash);
    /// Someone has preempted our selection, verify it's us or deselect silently
    void SelectionRejected(void* source);

private:
    std::vector< std::shared_ptr<DataSource> > selected_;
};

class SelectronLinked
{
public:
    void SetSelectron(Selectron* sel) {
        selectron_ = sel;
        Link(sel);
    }
    Selectron* GetSelectron() const { return selectron_; }

    virtual void Link(Selectron* sel) = 0;

private:
    Selectron* selectron_ = 0x0;
};