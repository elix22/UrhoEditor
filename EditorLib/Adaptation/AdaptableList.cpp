#include "AdaptableList.h"

AdaptableList::AdaptableList(const QUuid& hostID) :
    QListWidget(0x0),
    hostID_(hostID)
{

}

AdaptableList::~AdaptableList()
{

}