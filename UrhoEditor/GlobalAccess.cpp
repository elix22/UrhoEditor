#include "GlobalAccess.h"

#include "UrhoEditor.h"

#include <EditorLib/ApplicationCore.h>

#include <QMainWindow>
#include <QMessageBox>

void Global_SetStatusBarMessage(const QString& message, int duration)
{
    SprueEditor::UrhoEditor::GetInstance()->statusBar()->showMessage(message, duration);
}

SprueEditor::UrhoEditor* Global_GetEditor()
{
    return SprueEditor::UrhoEditor::GetInstance();
}

TaskProcessor* Global_MainTaskProcessor()
{
    return SprueEditor::UrhoEditor::GetInstance()->GetTaskProcessor();
}

TaskProcessor* Global_SecondaryTaskProcessor()
{
    return SprueEditor::UrhoEditor::GetInstance()->GetSecondaryTaskProcessor();
}

DocumentManager* Global_DocumentManager()
{
    return ApplicationCore::GetDocumentManager();
}

SprueEditor::RenderWidget* Global_RenderWidget()
{
    return SprueEditor::UrhoEditor::GetInstance()->GetRenderer();
}

SprueEditor::ViewManager* Global_ViewManager()
{
    return SprueEditor::UrhoEditor::GetInstance()->GetViewManager();
}

Selectron* Global_ObjectSectron()
{
    return SprueEditor::UrhoEditor::GetInstance()->GetObjectSelectron();
}

QMainWindow* Global_MainWindow()
{
    return SprueEditor::UrhoEditor::GetInstance();
}

void Global_ShowErrorMessage(const QString& title, const QString& message)
{
    QMessageBox::critical(0x0, title, message);
}