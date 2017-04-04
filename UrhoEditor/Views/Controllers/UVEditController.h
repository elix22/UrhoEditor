#pragma once

#include "../ViewController.h"

#include <vector>

namespace SprueEditor
{

    /// ViewController that handles editing UV coordinates in UVMapView in order to make minor tweaks and layout changes.
    /// Supports selecting and moving vertices, edges, and faces.
    /// Capabilities: expand selection, box selection, exclusion areas
    /// TODO: Add support for seams
    /// TODO: implement definitions.
    class UVEditController : public ViewController
    {
    public:
        UVEditController(ViewManager* manager, ViewBase* view, InputRecorder* input);
        virtual ~UVEditController();

        virtual bool CheckActivation(const ViewController* currentlyActive) override;
        virtual bool CheckTermination() override;
        virtual void Update() override;
        virtual void Prepare() override;
        virtual void Terminate() override;

    protected:
        /// Selects the face that best fits the given coordinates.
        void PickFace(float x, float y);
        /// Selects the edge that best fits the given coordinates.
        void PickEdge(float x, float y);
        /// Selects the vertex that best fits the given coordinates.
        void PickVertex(float x, float y);
        void ExpandSelectionVertices();
        void ExpandSelectionFaces();

    private:
        std::vector<unsigned> selectedVertexIndices_;
    };

}
    