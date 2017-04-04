#pragma once

#include <Urho3D/Core/Context.h>
#include <Urho3D/Math/Color.h>
#include <Urho3D/Math/Matrix3.h>
#include <Urho3D/Math/Matrix3x4.h>
#include <Urho3D/Core/Object.h>
#include <Urho3D/Math/Vector2.h>
#include <Urho3D/Math/Vector3.h>
#include <Urho3D/Math/Quaternion.h>

#include <algorithm>
#include <functional>
#include <map>
#include <memory>

namespace Urho3D
{
    class CustomGeometry;
    class DebugRenderer;
    class Node;
    class Texture2D;
}

namespace SprueEditor
{
    /// Recovers the name of a variable from the given node's scene
    std::string RecoverVariableName(Urho3D::Node*, const Urho3D::StringHash& hash);
    /// Stores a variable name for the given node in its scene.
    void StoreVariableName(Urho3D::Node*, const std::string& name);
    /// Adds a variable to the given node and stores the name in the node's scene.
    void AddUrhoVariable(Urho3D::Node*, const std::string& name, Urho3D::Variant var);

    void DrawPie(Urho3D::DebugRenderer*, float angle, const Urho3D::Vector3& center, const Urho3D::Vector3& normal, float radius, const Urho3D::Color& color, int steps, bool depthTest);

    /// Helper class for dealing with Urho3D event subscribtions to lambdas
    class UrhoEventHandler : public Urho3D::Object
    {
        URHO3D_OBJECT(Object, UrhoEventHandler);
    public:
        UrhoEventHandler(Urho3D::Context* context) : Urho3D::Object(context) { }

        typedef std::function<void(Urho3D::StringHash, Urho3D::VariantMap&)> EVENT_SUBSCRIPTION;

        void Subscribe(Urho3D::Object* from, Urho3D::StringHash eventID, EVENT_SUBSCRIPTION func);
        void Unsubscribe(Urho3D::Object* from, Urho3D::StringHash eventID);

        std::map<Urho3D::StringHash, EVENT_SUBSCRIPTION> subscribers_;

    protected:
        void BaseEventHandler(Urho3D::StringHash eventID, Urho3D::VariantMap& data);
    };

}