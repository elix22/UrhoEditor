#pragma once

#include <EditorLib/editorlib_global.h>
#include <EditorLib/Adaptation/IAdaptableHost.h>

#include <QWidget>
#include <QStackedLayout>
#include <QTableWidget>

/// Adapter based grid widget, will eventually replace Controls/DataGridWidget
class EDITORLIB_EXPORT AdaptableGrid : public QWidget, public IAdaptableHost
{
    Q_OBJECT;
public:
    AdaptableGrid(const QUuid& hostID);
    virtual ~AdaptableGrid();

    virtual QUuid GetHostID() const override { return hostID_; }

protected:
    /// Only contains 3 pages, "nothing selected", "incompatible selections", and "valid"
    QStackedLayout* stacks_;

    QUuid hostID_;
};