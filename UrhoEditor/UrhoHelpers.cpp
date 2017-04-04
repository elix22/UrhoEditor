#include "UrhoHelpers.h"

#include <Urho3D/Core/Context.h>
#include <Urho3D/Graphics/CustomGeometry.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Resource/Image.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Graphics/Texture2D.h>
#include <Urho3D/Math/Vector4.h>

#include <QString>

using namespace Urho3D;

namespace SprueEditor
{

    std::string RecoverVariableName(Urho3D::Node* node, const Urho3D::StringHash& hash)
    {
        if (Urho3D::Scene* scene = node->GetScene())
            return scene->GetVarName(hash).CString();
        return std::string();
    }
    
    void StoreVariableName(Urho3D::Node* node, const std::string& name)
    {
        if (Urho3D::Scene* scene = node->GetScene())
        {
            // Only add the variable name if it doesn't already exist
            if (scene->GetVarName(name.c_str()).Empty())
            {
                Urho3D::String names = scene->GetVarNamesAttr();
                scene->SetVarNamesAttr(QString("%1;%2;").arg(names.CString(), name.c_str()).toStdString().c_str());
            }
        }
    }

    void AddUrhoVariable(Urho3D::Node* node, const std::string& name, Urho3D::Variant var)
    {
        auto map = node->GetVars();
        map[name.c_str()] = var;
        StoreVariableName(node, name);
    }

    void DrawPie(Urho3D::DebugRenderer* debugRenderer, float angle, const Urho3D::Vector3& center, const Urho3D::Vector3& normal, float radius, const Urho3D::Color& color, int steps, bool depthTest)
    {
        Quaternion orientation;
        orientation.FromRotationTo(Vector3::UP, normal.Normalized());

        Vector3 p = orientation * Vector3(radius, 0, 0) + center;
        unsigned uintColor = color.ToUInt();
        Vector3 right = p.CrossProduct(normal);

        Vector3 last;
        for (int i = 0; i <= steps; i++)
        {
            Vector3 vt;
            int degrees = ((float)i) / (((float)steps) * 360.0f);
            vt = right * cos(((angle) / steps)*i);
            vt += p * sin(((angle) / steps)*i);
            vt += center;
            if (i > 0)
                debugRenderer->AddTriangle(center, last, vt, color);
            last = vt;
        }
    }

    void UrhoEventHandler::Subscribe(Urho3D::Object* from, Urho3D::StringHash eventID, EVENT_SUBSCRIPTION func)
    {
        if (from)
            SubscribeToEvent(from, eventID, URHO3D_HANDLER(UrhoEventHandler, BaseEventHandler));
        else
            SubscribeToEvent(eventID, URHO3D_HANDLER(UrhoEventHandler, BaseEventHandler));
        _ASSERT(subscribers_.find(eventID) == subscribers_.end());
        subscribers_[eventID] = func;
    }

    void UrhoEventHandler::Unsubscribe(Urho3D::Object* from, Urho3D::StringHash eventID)
    {
        if (from)
            UnsubscribeFromEvent(from, eventID);
        else
            UnsubscribeFromEvent(eventID);
    }

    void UrhoEventHandler::BaseEventHandler(Urho3D::StringHash eventID, Urho3D::VariantMap& data)
    {
        auto found = subscribers_.find(eventID);
        if (found != subscribers_.end())
            found->second(eventID, data);
    }
}