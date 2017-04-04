#pragma once

#include "../../GuiBuilder.h"

namespace UrhoUI
{

    class UrhoSpecialActionMenu : public SprueEditor::GuiBuilder
    {
    public:
        virtual void Build(SprueEditor::UrhoEditor* editor) override;
    };

}