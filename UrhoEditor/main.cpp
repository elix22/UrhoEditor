#include "UrhoEditor.h"

#include "SprueApplication.h"

#include <EditorLib/Settings/Settings.h>
#include <EditorLib/Settings/SettingsPage.h>
#include <EditorLib/Settings/SettingsValue.h>

#include <QApplication>
#include <QDir>
#include <QDesktopWidget>
#include <QLibrary>
#include <QFile>
#include <QStyle>

// Application Builders

// Dock builders
#include "Documents/Urho/UrhoHomeRibbonBuilder.h"

// Documents
#include "Documents/Urho/UrhoSceneDocument.h"

// Menu Builders
#include "GuiBuilder/Menus/CoreMenuBuilder.h"
#include "GuiBuilder/Menus/EditMenuBuilder.h"
#include "GuiBuilder/Menus/LayoutMenuBuilder.h"
#include "GuiBuilder/Menus/ViewportMenuBuilder.h"
#include "GuiBuilder/Menus/ViewMenuBuilder.h"

// Dock Builders
#include "GuiBuilder/Docks/SharedDockBuilder.h"
#include "Documents/Urho/UrhoDockBuilder.h"

// Ribbon Builders
#include "GuiBuilder/Ribbons/CoreGUIRibbonBuilder.h"

// Application
#include "Applications/UrhoEditorApplication.h"

using namespace SprueEditor;

QDataStream &operator<<(QDataStream &out, Urho3D::Serializable*& rhs) {
    out.writeRawData(reinterpret_cast<char*>(&rhs), sizeof(rhs));
    return out;
}

QDataStream & operator >> (QDataStream &in, Urho3D::Serializable*& rhs) {
    in.readRawData(reinterpret_cast<char*>(rhs), sizeof(rhs));
    return in;
}

#define DECLARE_URHO_TYPE(TYPENAME) QDataStream &operator<<(QDataStream& out, const Urho3D::TYPENAME& rhs) { out.writeRawData((char*)(&rhs), sizeof(rhs)); return out; } \
    QDataStream & operator >> (QDataStream &in, Urho3D::TYPENAME& rhs) { in.readRawData(reinterpret_cast<char*>(&rhs), sizeof(rhs)); return in; }

DECLARE_URHO_TYPE(IntVector2);
DECLARE_URHO_TYPE(Vector2);
DECLARE_URHO_TYPE(Vector3);
DECLARE_URHO_TYPE(Quaternion);
DECLARE_URHO_TYPE(Matrix3);
DECLARE_URHO_TYPE(Matrix3x4);
DECLARE_URHO_TYPE(Matrix4);

#ifdef WIN32
#include <Windows.h>
typedef BOOL(*SetProcessDpiAwarenessT)(int value);
#endif

int main(int argc, char *argv[])
{
#ifdef _WIN32
    //{
    //    QLibrary user32("user32.dll", NULL);
    //    SetProcessDpiAwarenessT SetProcessDpiAwarenessD = (SetProcessDpiAwarenessT)user32.resolve("SetProcessDpiAwarenessInternal");
    //    if (SetProcessDpiAwarenessD)
    //        SetProcessDpiAwarenessD(1); //PROCESS_SYSTEM_DPI_AWARE
    //}
#endif

    qRegisterMetaTypeStreamOperators<Urho3D::Serializable*>("Urho3D::Serializable*");

#define DECLARE_URHO_STREAM(TYPENAME) qRegisterMetaTypeStreamOperators<Urho3D:: TYPENAME>("Urho3D::" #TYPENAME);

    DECLARE_URHO_STREAM(IntVector2);
    DECLARE_URHO_STREAM(Vector2);
    DECLARE_URHO_STREAM(Vector3);
    DECLARE_URHO_STREAM(Quaternion);
    //DECLARE_URHO_STREAM(Matrix3);
    //DECLARE_URHO_STREAM(Matrix3x4);
    //DECLARE_URHO_STREAM(Matrix4);

    Settings* settings = new Settings();

#ifdef SPRUE_TEXGEN
    QCoreApplication::setApplicationName("TexGraph");
#elif defined(SPRUE_SCULPT)
    QCoreApplication::setApplicationName("Sculpt");
#elif defined(URHO_EDITOR)
    QCoreApplication::setApplicationName("Urho3D Editor");
#else
    QCoreApplication::setApplicationName("UrhoEditor");
#endif

    QCoreApplication::setLibraryPaths(QStringList{ "plugins/platforms" });
    UrhoUI::Urho3D_ConstructSettings(settings);


    settings->RestoreSettings();

    if (settings->GetPage("General")->Get("Enable High-DPI scaling")->value_.toBool())
        QApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
    else
        QApplication::setAttribute(Qt::AA_EnableHighDpiScaling, false);

    SprueApplication a(settings, argc, argv);

    /// Developer processes
    //TextureNodeIconBuilder();

    //@ApplicationUnique
    std::vector<GuiBuilder*> menuBuilders = {
        new CoreMenuBuilder(),
        new EditMenuBuilder(),
        new LayoutMenuBuilder(),
        new ViewportMenuBuilder(),
        new ViewMenuBuilder(),

        new SharedDockBuilder(),
        new UrhoDockBuilder(),

        new UrhoUI::UrhoHomeRibbonBuilder(),
        new CoreGUIRibbonBuilder(),
    };

    std::vector<DocumentHandler*> documentHandlers = {
        new UrhoUI::UrhoSceneDocumentHandler()
    };

    QFile file(":/qdarkstyle/style.qss");
    if (file.open(QFile::ReadOnly | QFile::Text))
        a.setStyleSheet(QLatin1String(file.readAll()));

    UrhoEditor w(settings, menuBuilders, documentHandlers);
    w.CreateWidgets();

    //w.show();
    w.RestoreLayout("PREVIOUS_LAYOUT.layout");

    w.show();

    //w.showMaximized();
    return a.exec();
}