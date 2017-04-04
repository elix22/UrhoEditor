#include "UrhoResourceEditors.h"

#include "../../Commands/Urho/UrhoPropertyCmd.h"

#include "../../QtHelpers.h"

#include <map>

namespace UrhoUI
{

    static std::map<Urho3D::StringHash, std::string> UrhoResourceToFileType = {
        std::make_pair("LuaScript",     "Lua Script files (*.lua)"),
        std::make_pair("Material",      "Materials (*.xml)"),
        std::make_pair("Model",         "Models (*.mdl)"),
        std::make_pair("ParticleEffect","Particle effects (*.xml)"),
        std::make_pair("ScriptFile",    "Angelscript files (*.as)"),
        std::make_pair("Sound",         "Sound files (*.wav)"),
        std::make_pair("Texture",       "Textures (*.xml, *.png, *.jpg, *.jpeg, *.tga)"),
        std::make_pair("Texture2D",     "2D Textures (*.xml, *.png, *.jpg, *.jpeg, *.tga, *.dds)"),
        std::make_pair("Texture3D",     "3D Textures (*.xml, *.dds)"),
        std::make_pair("TextureCube",   "Textures (*.xml, *.dds)"),
        std::make_pair("TextureArray",  "Texture Arrays (*.xml, *.dds)"),
        std::make_pair("XMLFile",       "XML files(*.xml)"),
    };

    ResourceRefEditor::ResourceRefEditor(unsigned property) :
        BaseTypeEditor(property, false)
    {
        browser_ = new PathPickerWidget(false);
        layout_->addWidget(browser_);

        connect(browser_, &PathPickerWidget::PathChanged, this, &ResourceRefEditor::PathChanged);
    }

    Urho3D::Variant ResourceRefEditor::GetValue()
    {
        return Urho3D::Variant();
    }

    void ResourceRefEditor::SetValue(const Urho3D::Variant& value)
    {
        Urho3D::ResourceRef ref = value.GetResourceRef();
        browser_->SetPath(ref.name_.CString());

        Urho3D::ResourceRef list = editing_->GetObject()->GetAttribute(property_).GetResourceRef();
        const auto foundType = UrhoResourceToFileType.find(list.type_);
        if (foundType != UrhoResourceToFileType.end())
            browser_->SetFilter(foundType->second);
    }

    void ResourceRefEditor::PropertyUpdated(std::shared_ptr<SerializableDataSource> editing)
    {
        if (editing)
        {
            Urho3D::ResourceRef ref = editing->GetObject()->GetAttribute(property_).GetResourceRef();
            browser_->SetPath(ref.name_.CString());

            Urho3D::ResourceRef list = editing_->GetObject()->GetAttribute(property_).GetResourceRef();
            const auto foundType = UrhoResourceToFileType.find(list.type_);
            if (foundType != UrhoResourceToFileType.end())
                browser_->SetFilter(foundType->second);
        }
    }

    void ResourceRefEditor::PathChanged(const PathPickerWidget* widget, const char* newPath)
    {
        if (editing_)
        {
            Urho3D::ResourceRef ref = editing_->GetObject()->GetAttribute(property_).GetResourceRef();
            ref.name_ = newPath;
            URHO_PROPERTY_UNDO(editing_, property_, ref);
            //editing_->SetAttribute(property_, ref);
            PropertyUpdated(editing_);
        }
    }

    ResourceRefListEditor::ResourceRefListEditor(unsigned property) :
        BaseTypeEditor(property, true)
    {

    }

    Urho3D::Variant ResourceRefListEditor::GetValue()
    {
        return Urho3D::Variant();
    }

    void ResourceRefListEditor::SetValue(const Urho3D::Variant& value)
    {
        Urho3D::ResourceRefList list = value.GetResourceRefList();
        const auto foundType = UrhoResourceToFileType.find(list.type_);
        for (unsigned i = 0; i < list.names_.Size(); ++i)
        {
            PathPickerWidget* browser = new PathPickerWidget(false);
            browser->SetPath(list.names_[i].CString());
            browser->setObjectName(QString::number(i));
            layout_->addWidget(browser);
            if (foundType != UrhoResourceToFileType.end())
                browser->SetFilter(foundType->second);
        }
    }

    void ResourceRefListEditor::PropertyUpdated(std::shared_ptr<SerializableDataSource> editing)
    {
        SprueEditor::QtHelpers::ClearLayout(layout_);
        if (editing_)
        {
            Urho3D::ResourceRefList list = editing_->GetObject()->GetAttribute(property_).GetResourceRefList();
            const auto foundType = UrhoResourceToFileType.find(list.type_);
            for (unsigned i = 0; i < list.names_.Size(); ++i)
            {
                PathPickerWidget* browser = new PathPickerWidget(false);
                browser->SetPath(list.names_[i].CString());
                browser->setObjectName(QString::number(i));
                layout_->addWidget(browser);
                if (foundType != UrhoResourceToFileType.end())
                    browser->SetFilter(foundType->second);
            }
        }
    }

    void ResourceRefListEditor::PathChanged(const PathPickerWidget* widget, const char* newPath)
    {
        unsigned index = widget->objectName().toUInt();

        Urho3D::ResourceRefList list = editing_->GetObject()->GetAttribute(property_).GetResourceRefList();
        list.names_[index] = newPath;
        URHO_PROPERTY_UNDO(editing_, property_, list);
        //editing_->SetAttribute(property_, list);
        PropertyUpdated(editing_);
    }
}