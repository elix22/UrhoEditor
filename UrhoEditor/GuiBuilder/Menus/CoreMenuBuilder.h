#include "../GuiBuilder.h"

namespace SprueEditor
{

    class CoreMenuBuilder : public GuiBuilder
    {
    public:
        virtual void Build(UrhoEditor* editor) override;
    };

}