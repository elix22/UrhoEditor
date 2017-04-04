#pragma once

#include "../../GuiBuilder.h"

#include <QActionGroup>

namespace UrhoUI
{

    class UrhoSelectionModeMenu : public SprueEditor::GuiBuilder
    {
    public:
        virtual void Build(SprueEditor::UrhoEditor* editor);

    private:
        QActionGroup* group_;
    };

}