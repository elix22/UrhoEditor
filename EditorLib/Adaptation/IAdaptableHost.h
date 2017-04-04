#pragma once

#include <EditorLib/editorlib_global.h>

#include <QUuid>

class EDITORLIB_EXPORT IAdaptableHost
{
public:
    /// Unique identifier for this adaptable host, given to adaptables so they can return correct adaptee.
    virtual QUuid GetHostID() const = 0;
};