#pragma once

#include <QApplication>
#include <QClipboard>
#include <qlayout.h>
#include <qmimedata.h>
#include <qfontmetrics.h>
#include <qmenu.h>
#include <qmenubar.h>
#include <QAbstractButton>

#include <vector>

class QPainter;
class QFontMetrics;

namespace SprueEditor
{
    namespace QtHelpers
    {
        /// Removes and destroys everything contained by a layout.
        void ClearLayout(QLayout* layout);
        /// Hides everything contained by a layout.
        void HideLayout(QLayout* layout);
        /// Shows everything contained by a layout.
        void ShowLayout(QLayout* layout);
        /// Sets the enable state of all contents of the given layout.
        void SetEnabledOnAll(QLayout* layout, bool state);

        void UpdateMenus(QMenuBar* bar);
        void UpdateMenus(QMenu* bar);

        /// Get the path to something in the AppData/Roaming directory
        std::string GetAppDataPath(const std::string& file);
        /// Get the path to something in the Users/USERNAME/Documents directory
        std::string GetDocumentsPath(const std::string& file);

        QSize MeasureTextForceWrap(const QString& text, const QFontMetrics& metrics, int width, std::vector<std::pair<int, int> >* indices);
        void PaintTextForceWrap(QPainter* painter, const QPoint& paintAt, const QString& text, QFontMetrics& metrics, int width);

        void AttachHelpFlyout(QAbstractButton* button, const QString& name);
    }
}