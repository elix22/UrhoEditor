#pragma once

#include <QObject>

class LanguageData;

/// Singleton object for fetching localized strings.
class Localizer : public QObject
{
    Q_OBJECT;
    Localizer();
public:
    virtual ~Localizer();

    QString Localize(const QString& input) const;

    void LoadLanguage(const QString& file);

    void StartRecording();
    void StopRecording(const QString& fileOutput) const;

    static Localizer* GetInstance();
    static QString Translate(const QString&);

signals:
    void LanguageChanged();
private:
    static Localizer* instance_;
    LanguageData* activeLanguageData_;
};