#include "SprueApplication.h"

#include <EditorLib/Settings/Settings.h>

#include <qstandardpaths.h>

namespace SprueEditor
{

    SprueApplication::SprueApplication(Settings* settings, int argc, char** argv) :
        QApplication(argc, argv),
        settings_(settings)
    {
#ifdef SPRUE_TEXGEN
        QCoreApplication::setApplicationName("TexGraph");
        QCoreApplication::setApplicationVersion("0.1");
#elif defined(SPRUE_SCULPT)
        QCoreApplication::setApplicationName("Sculpt");
        QCoreApplication::setApplicationVersion("0.1");
#elif defined(URHO_EDITOR)
        QCoreApplication::setApplicationName("Urho3D Editor");
        QCoreApplication::setApplicationVersion("0.1");
#else
        QCoreApplication::setApplicationName("UrhoEditor");
        QCoreApplication::setApplicationVersion("0.1");
#endif
        

        QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
#ifndef SPRUE_HTML_LOG
        path += "/log.html";
#else
        path += "/log.txt";
#endif
        logFile_ = new LogFile(path, "Log");

    }
    
    SprueApplication::~SprueApplication()
    {
        settings_->SaveSettings();
    }

}