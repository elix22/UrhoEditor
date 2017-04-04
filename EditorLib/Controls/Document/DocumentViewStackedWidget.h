#pragma once

#include <QStackedWidget>
#include <map>

class DocumentBase;

/// QStackedWidget that uses a predicate object to select the appropriate widget to display for
/// the current document.
/// Purpose:
/// Use this widget to display the appropriate specific control where the same DockingPanel is
/// semantically appropriate (ie. "Scene Tree")
class DocumentViewStackedWidget : public QStackedWidget
{
    Q_OBJECT
public:
    /// Interface for a class used to confirm the validity of a widget for a given document.
    /// Results are prioritized by the returned specificity.
    struct IDocTypeChecker
    {
        virtual bool CheckDocument(DocumentBase* doc) = 0;
        virtual int Specificity() const { return 0; }
    };

    /// Simple DocTypeChecker that works solely against a specific type.
    /// Current view-index is not a concern
    template<class T>
    struct TypedDocTypeChecker : public IDocTypeChecker
    {
        virtual bool CheckDocument(DocumentBase* doc) { return dynamic_cast<T*>(doc) != 0x0; }
        virtual int Specificity() const override { return 1; }
    };

    /// Type checker that requires a specific type and current view-index.
    /// Has higher specificity than the type only checker.
    template<class T>
    struct TypedViewIndexDocTypeChecker : public IDocTypeChecker {
        TypedViewIndexDocTypeChecker(int idx) : viewIndex_(idx) {

        }

        virtual bool CheckDocument(DocumentBase* doc) { return dynamic_cast<T*>(doc) != 0x0 && doc->GetActiveViewIndex() == viewIndex_; }
        virtual int Specificity() const override { return 2; }

        int viewIndex_;
    };

    /// Construct.
    DocumentViewStackedWidget(QWidget* parent = 0x0);
    /// Destruct.
    virtual ~DocumentViewStackedWidget();

    /// Add a widget with a specific type checker.
    void AddWidget(IDocTypeChecker* checker, QWidget* widget);

    /// Helper function for adding a checked widget based solely on document type.
    template<typename T>
    void AddWidget(QWidget* widget) { AddWidget(new TypedDocTypeChecker<T>(), widget); }

    /// Helper function for adding a checked widget based on both document type and current view index.
    template<typename T, int IDX>
    void AddWidget(QWidget* widget) { AddWidget(new TypedViewIndexDocTypeChecker<T>(IDX), widget); }

    private slots:
    /// Respond to view changes when a new document is focused.
    void OnDocumentChanged(DocumentBase* newDoc, DocumentBase* oldDoc);
    /// Respond to changes of the view in a document.
    void OnDocumentViewChanged(DocumentBase* doc);

private:
    /// Maps the index of the active view to the index in the stacked widget.
    std::map<int, int> viewIndexToWidget_;
    /// Verifies if we actually care about a document.
    std::map<IDocTypeChecker*, int> checkerTable_;
};