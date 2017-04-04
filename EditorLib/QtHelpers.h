#pragma once

#include <QString>
#include <QApplication>
#include <QClipboard>
#include <qlayout>
#include <qmimedata>
#include <qfontmetrics>
#include <qmenu>
#include <qmenubar>
#include <QAbstractButton>

#include <vector>

class QPainter;
class QFontMetrics;
class RegisteredAction;

namespace QtHelpers
{
    void ClearLayout(QLayout* layout);
    void HideLayout(QLayout* layout);
    void ShowLayout(QLayout* layout);
    void SetEnabledOnAll(QLayout* layout, bool state);

    void UpdateMenus(QMenuBar* bar);
    void UpdateMenus(QMenu* bar);

    /// Get the path to something in the AppData/Roaming directory
    QString GetAppDataPath(const QString& file);
    /// Get the path to something in the Users/USERNAME/Documents directory
    QString GetDocumentsPath(const QString& file);

    QSize MeasureTextForceWrap(const QString& text, const QFontMetrics& metrics, int width, std::vector<std::pair<int, int> >* indices);
    void PaintTextForceWrap(QPainter* painter, const QPoint& paintAt, const QString& text, QFontMetrics& metrics, int width);

    void AttachHelpFlyout(QAbstractButton* button, const QString& name);

    /// Utility for quickly adding a collection of actions in a AddActions(bar, { actA, actB, actC }); form
    void AddActions(QMenuBar* bar, const std::vector<RegisteredAction*>& actions);
    /// Utility for quickly adding a collection of actions in a AddActions(menu, { actA, actB, actC }); form
    void AddActions(QMenu* menu, const std::vector<RegisteredAction*>& actions);
}