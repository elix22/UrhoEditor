#pragma once

#include <QAction>
#include <QIcon>
#include <QString>

#include <vector>

class DocumentBase;

/// Preferable to using QAction. All existing instances can be found in a single "AutoList" 
/// which is primarily used for populating the action customization GUI and quick action search
class RegisteredAction : public QAction
{
public:
    /// Construct. Action class is used for grouping commands
    RegisteredAction(const QString& actionClass, const QString& text, QObject* owner = 0x0);
    /// Construct. Action class is used for grouping commands
    RegisteredAction(const QString& actionClass, const QIcon& icon, const QString& text, QObject* owner = 0x0);
    virtual ~RegisteredAction();

    QString GetActionClass() const { return actionClass_; }

    virtual void SetShortcut(QKeySequence sequence) { QAction::setShortcut(sequence); }

    virtual QKeySequence GetShortCut() { return QAction::shortcut(); }

    virtual void Trigger() { QAction::trigger(); }

    static std::vector< RegisteredAction* >& GetList() { return actions_; }
    static RegisteredAction* GetAction(const QString& originalName);

    static void Save();
    static void Restore();

    virtual bool IsDocumentRelevant(DocumentBase* document) { return true; }
    virtual QString GetDocumentType() const { return QString(); }

    QString GetOriginalName() const { return originalName_; }

    virtual bool IsAvailable() const { return isEnabled(); }

private:
    QString originalName_;
    QString actionClass_;
    static std::vector< RegisteredAction* > actions_;
};

class RegisteredWrapperAction : public RegisteredAction
{
    Q_OBJECT
public:
    RegisteredWrapperAction(const QString& actionClass, QAction* wrapping);
    virtual ~RegisteredWrapperAction() { }

    virtual void SetShortcut(QKeySequence sequence) override {
        if (action_)
            action_->setShortcut(sequence);
    }

    QAction* GetAction() const { return action_; }

    virtual QKeySequence GetShortCut() override { return action_ ? action_->shortcut() : 0x0; }

    virtual void Trigger();

private:
    QAction* action_;
};