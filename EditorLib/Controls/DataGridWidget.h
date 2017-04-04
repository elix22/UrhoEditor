#pragma once

#include <EditorLib/Selectron.h>

#include <QWidget>
#include <QStackedLayout>
#include <QTableWidget>

#include <vector>
#include <set>
#include <string>
#include <memory>

class DocumentBase;

/// Control that displays the values of selected objects in a datagrid format. 
/// Enables relatively easy comparisons of values and editing to match/differ.
class DataGridWidget : public QWidget, public SelectronLinked
{
    Q_OBJECT
public:
    /// Construct.
    DataGridWidget();
    /// Destruct.
    virtual ~DataGridWidget();

    /// Setup signals/slots for selection changes.
    virtual void Link(Selectron* sel) override;

    /// DataSource based strategy handling for the datagrid's population and cell management.
    struct DataSourceHandler
    {
        /// Implementation should return true for DataSources that it can handle. Should perform any required conversions (ie. Euler -> Quat).
        virtual bool CanHandleDataSource(std::shared_ptr<DataSource> dataSource) const = 0;
        /// Returns true if the given column can be shown, or false if not ... such as for properties that have no viable way to edit as plain-text.
        virtual bool CanShow(std::shared_ptr<DataSource> dataSource, const std::string& column) const { return true; }
        /// Returns a list of all columns in the datasource.
        virtual std::vector<std::string> GetColumns(std::shared_ptr<DataSource> dataSource) const = 0;

        /// Returns true if the given datafield is editable in text form.
        virtual bool CanSetText(std::shared_ptr<DataSource> dataSource, const std::string& column) const { return true; }
        /// Implementation sets a value from text for a column/property. Should perform any required conversions (ie. Quat -> Euler).
        virtual void FromText(std::shared_ptr<DataSource> dataSoruce, const std::string& column, const std::string& textValue) = 0;
        /// Creates a widget to use instead of plain-text in the cell.
        virtual QWidget* CreateWidget(std::shared_ptr<DataSource> dataSource, const std::string& column) const { return 0x0; }
        /// Implementation gets the text value of a column.
        virtual QString ToText(std::shared_ptr<DataSource> dataSource, const std::string& column) const = 0;
    };

    void AddHandler(DataSourceHandler* handler) { handlers_.push_back(handler); }

private slots:
    /// Flush.
    void ActiveDocumentChanged(DocumentBase* newDoc, DocumentBase* oldDoc);
    /// Redertimine the grid layout.
    void SelectionChanged(void* source, Selectron* sel);
    /// Update changes to the data of selected objects.
    void SelectionDataChanged(void* source, Selectron* sel, unsigned hash);

    /// Update properties from text cell changes.
    void DataChanged(QTableWidgetItem* item);

protected:
    virtual void showEvent(QShowEvent*) Q_DECL_OVERRIDE;
    virtual void hideEvent(QHideEvent*) Q_DECL_OVERRIDE;

    DataSourceHandler* GetHandlerFor(std::shared_ptr<DataSource> dataSource);

private:
    /// Sets the layout stack to display the "Incompatible selections" page.
    void SetToIncompatible();
    /// Sets the layout stack to display the "Nothing selected" page.
    void SetToNoSelection();

    /// Only contains 3 pages, "nothing selected," "Incompatible objects selected", and the datatable itself
    QStackedLayout* stack_;
    /// Table-grid used for cell display.
    QTableWidget* table_;
    /// List of the columns in left-to-right order, based on the order in which their names were first encountered.
    std::vector<std::string> appearanceOrder_;
    /// List of the columns to be displayed that have passed filtering.
    std::set<std::string> filtered_;
    /// List of the datasources used for each row of the datagrid.
    std::vector< std::shared_ptr<DataSource> > dataSources_;
    /// List of datasource handlers.
    std::vector<DataSourceHandler*> handlers_;
    bool hidden_ = true;
};