#pragma once

#include <EditorLib/Controls/ISignificantControl.h>
#include <EditorLib/Interfaces.h>

#include <QTreeWidget>
#include <qevent.h>

/// Contains a categorized tree list of nodes intended for dragging and dropping elsewhere.
/// Works with an IPaletteView object in order to populate the list.
/// TODO: changes to the palette after it's been bound (ie. new "Prefab" objects)
class PaletteWidget : public QTreeWidget, public ISignificantControl
{
    Q_OBJECT
public:
    PaletteWidget();
    virtual ~PaletteWidget();

public slots:
    void ChangeView(IPaletteView* view);

protected:
    virtual void mouseMoveEvent(QMouseEvent*) Q_DECL_OVERRIDE;
};
