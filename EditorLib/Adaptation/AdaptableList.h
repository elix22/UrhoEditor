#pragma once

#include <EditorLib/editorlib_global.h>
#include <EditorLib/Adaptation/IAdaptableHost.h>

#include <QListWidget>

/// Adapter based list widget.
class EDITORLIB_EXPORT AdaptableList : public QListWidget, public IAdaptableHost
{
    Q_OBJECT;
public:
    AdaptableList(const QUuid& hostID);
    virtual ~AdaptableList();

    virtual QUuid GetHostID() const override { return hostID_; }

protected:
    QUuid hostID_;
};