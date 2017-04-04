#pragma once

#include <EditorLib/Settings/Settings.h>

#include <QDialog>

class ConfigDlg : public QDialog
{
    Q_OBJECT
protected:
    ConfigDlg(Settings*);

public:
    virtual ~ConfigDlg();

    static void Show(Settings* settings);

    static QWidget* GenerateControls(SettingsPage* page, SettingValue* value);

private:
    static ConfigDlg* instance_;
};