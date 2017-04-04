#include "PaletteWidget.h"

#include <qdrag.h>
#include <qmimedata.h>

#include <string>
#include <map>

PaletteWidget::PaletteWidget()
{
    headerItem()->setHidden(true);
}

PaletteWidget::~PaletteWidget()
{

}

void PaletteWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (!(event->buttons() & Qt::LeftButton))
        return;
    // if the left button is down
    if (event->buttons() & Qt::LeftButton)
    {
        if (QTreeWidgetItem* item = dynamic_cast<QTreeWidgetItem*>(itemAt(event->pos())))
        {
            QDrag* drag = new QDrag(this);
            QMimeData* mimedata = new QMimeData();
            drag->setMimeData(mimedata);
            mimedata->setObjectName(item->data(1, Qt::UserRole).toString());
            mimedata->setText(item->data(0, Qt::UserRole).toString());
            Qt::DropAction dropAction = drag->exec(Qt::DropAction::CopyAction);
        }
    }
}

void PaletteWidget::ChangeView(IPaletteView* view)
{
    clear();
    if (view)
    {
        auto groups = view->GetPlatteItems();
        for (auto grp : groups)
        {
            QTreeWidgetItem* grpItem = new QTreeWidgetItem();
            grpItem->setText(0, grp.name_);
            addTopLevelItem(grpItem);

            for (auto& item : grp.items_)
            {
                QTreeWidgetItem* node = new QTreeWidgetItem();
                node->setText(0, item.text_);
                node->setToolTip(0, item.tooltip_);
                node->setIcon(0, item.icon_);
                node->setData(0, Qt::UserRole, item.simpleMimeData_);
                node->setData(1, Qt::UserRole, item.mimeTypeID_);
                grpItem->addChild(node);
            }
        }
    }
}