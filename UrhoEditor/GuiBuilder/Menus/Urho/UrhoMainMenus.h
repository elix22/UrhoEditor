#pragma once

#include "../../GuiBuilder.h"

namespace UrhoUI
{

    class UrhoMainMenus : public SprueEditor::GuiBuilder
    {
    public:
        virtual void Build(SprueEditor::UrhoEditor* editor);
    };

}