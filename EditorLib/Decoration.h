#pragma once

#include <EditorLib/editorlib_global.h>

#include <unordered_map>
#include <string>

namespace EditorLib
{

    class EDITORLIB_EXPORT DecorationFactory
    {
    public:
    };

    class EDITORLIB_EXPORT DecorationManager
    {
    public:
    private:
        std::unordered_map<std::string, DecorationFactory*> factories_;
    };

}