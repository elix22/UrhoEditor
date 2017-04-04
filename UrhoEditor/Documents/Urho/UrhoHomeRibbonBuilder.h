#pragma once

#include "../../GuiBuilder/GuiBuilder.h"

namespace UrhoUI
{

    class UrhoHomeRibbonBuilder : public SprueEditor::GuiBuilder
    {
    public:
        virtual void Build(SprueEditor::UrhoEditor* editor) override;
    };

}