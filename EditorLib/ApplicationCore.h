#pragma once

#include <QMainWindow>
#include <EditorLib/DocumentManager.h>

class Ribbon;

class ApplicationCore
{
public:
    ApplicationCore(QMainWindow* mainWindow) {
        instance_ = this;
        mainWindow_ = mainWindow;
        documentManager_ = new DocumentManager();
    }

    static ApplicationCore* GetInstance() { return instance_; }
    static QMainWindow* GetMainWindow() { return GetInstance()->mainWindow_; }
    static DocumentManager* GetDocumentManager() { return GetInstance()->documentManager_; }
    static Ribbon* GetRibbon() { return GetInstance()->ribbon_; }
    static void SetRibbon(Ribbon* rib) { GetInstance()->ribbon_ = rib; }
    static void SetStatusBarMessage(const QString& msg);

private:
    static ApplicationCore* instance_;
    QMainWindow* mainWindow_ = 0x0;
    DocumentManager* documentManager_ = 0x0;
    Ribbon* ribbon_ = 0x0;
};