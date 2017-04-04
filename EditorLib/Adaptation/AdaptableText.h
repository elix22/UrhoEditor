#pragma once

#include <EditorLib/editorlib_global.h>
#include <EditorLib/Adaptation/IAdaptableHost.h>

#include <QTextBrowser>

/// Adaptation based text-browser. Usages include global or per-document logs, help documentation (continually changing source), etc.
class EDITORLIB_EXPORT AdaptableText : public QTextBrowser, public IAdaptableHost
{
    Q_OBJECT;
public:
    AdaptableText(const QUuid& hostID);
    virtual ~AdaptableText();

    virtual QUuid GetHostID() const override { return hostID_; }

protected:
    QUuid hostID_;
};