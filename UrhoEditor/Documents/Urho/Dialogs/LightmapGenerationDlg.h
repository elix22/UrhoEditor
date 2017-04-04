#pragma once

#include <QDialog>

namespace UrhoUI
{

    /// Configures and executes the generation of lightmaps for an Urho3D scene.
    class LightmapGenerationDlg : public QDialog
    {
        Q_OBJECT
    public:
        /// Construct.
        LightmapGenerationDlg();
        /// Destruct.
        virtual ~LightmapGenerationDlg();
    };

}