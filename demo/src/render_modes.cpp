#if defined __APPLE__
#include "SamplesMacOS.h"
#else
#include "sample.h"
#endif

#include "sample_common.h"
#include "sprk.h"

using namespace HPS;

void Sample::Render_Modes(WindowKey wk, Canvas canvas, View view, Model model,
                          SegmentKey modelKey) {
    /* Sample: Render_Modes
     * This sample will show you how to:
     * - Load an HSF file through Stream and check for Stream IO exceptions
     * - Divide the screen into sub-window
     * - Include a segment into other segments
     * - Render with flat shading, wireframe, hidden line, gouraud and phong
     * algorithms
     * - Use color index interpolation
     *
     * Read more about the topics covered in this sample in our Programming
     * Guide
     * - section 7.5:	Lighting Interpolation*/

    /* This next block of code shows you how to load an HSF file
     * 1. Create an ImportOptionsKit and set the segment in which you want the
     *file to be loaded
     * 2. Use the Stream::File::Import method to load the desired file, with the
     *options you specified
     * 3. Since this operation happens on a separate thread, wait for it to
     *complete before proceeding
     * 4. Use a try-catch block to capture any Stream IO exceptions
     * 5. Root segments are special segments, which can only be created from the
     *Database. Since this root segment is not associated with a window, its
     *contents will not be displayed on the screen. This segment will be used to
     *store geometry which can later be included in segments which are under a
     *window */
    SegmentKey includeLibrary = Database::CreateRootSegment();
    SegmentKey urn = includeLibrary.Subsegment();

    // Loading an HSF file into a specific segment
    Stream::ImportNotifier notifier;
    SegmentKey phongKey = modelKey.Subsegment();
    try {
        Stream::ImportOptionsKit importOptionsKit;
        importOptionsKit.SetSegment(urn);
        UTF8 filename = INPUT_FOLDER + "/urn.hsf";

        notifier = Stream::File::Import(static_cast<char const*>(filename),
                                        importOptionsKit);
        notifier.Wait();

        // Find the geometry mesh
        SearchResults searchResults;
        size_t found = urn.Find(Search::Type::Geometry,
                                Search::Space::Subsegments, searchResults);
        if (found != 1)
            throw IOException("The geometry is missing", IOResult::Failure);

        Key foundKey = searchResults.GetIterator().GetItem();
        if (foundKey.Type() == Type::MeshKey) {
            MeshKey urnMeshKey = (MeshKey)foundKey;

            PointArray points;
            urnMeshKey.ShowPoints(points);

            // Setup vertex RGB colors which will be used for face color
            // interpolation
            size_t numberOfPoints = points.size();
            float quarterPoints = (float)numberOfPoints / 4.0f;
            RGBColorArray colorArray(numberOfPoints);
            for (size_t i = 0; i < numberOfPoints; ++i) {
                if (i < quarterPoints)
                    colorArray[i] = RGBColor(0.24f, 0.48f, 0.76f);
                else if (i > quarterPoints && i < quarterPoints * 2)
                    colorArray[i] = RGBColor(0.67f, 0.07f, 0.64f);
                else if (i > quarterPoints * 2 && i < quarterPoints * 3)
                    colorArray[i] = RGBColor(1, 0.5f, 0);
                else
                    colorArray[i] = RGBColor(0.87f, 0.65f, 0);
            }

            urnMeshKey.SetVertexRGBColorsByRange(0, colorArray,
                                                 Mesh::Component::Faces);
        }
    } catch (IOException const&) {
        /* catch Stream I/O Errors here */
    }

    urn.GetModellingMatrixControl().Rotate(-25, 0, 0);

    TextAttributeKit textAttributeKit;
    VisibilityKit visibilityKit;
    CameraKit cameraKit;

    textAttributeKit.SetAlignment(Text::Alignment::TopCenter);
    textAttributeKit.SetSize(0.03f, Text::SizeUnits::WindowRelative);

    modelKey.SetTextAttribute(textAttributeKit);
    modelKey.GetMaterialMappingControl().SetFaceColor(RGBAColor(1, 0, 0, 1));
    modelKey.GetColorInterpolationControl().SetFaceColor(false);

    SubwindowKit subwindowKit;

    /* This block of code divides the screen into subsegments, includes geometry
     *in the scene and sets different rendering options on each segment
     * 1. To set a sub-window call the SetSubwindow method with the dimensions
     *of the sub-window, normalized between -1 and 1, with (-1, 1) pointing to
     *the top left corner
     * 2. To include a segment into another segment use the following method:
     *		destinationSegmentKey.IncludeSegment(sourceSegmentKey)
     *		including segments is a good way to add the same geometry in
     *multiple places without needlessly creating multiple copies of the
     *geometry in question
     * 3. Change the lighting interpolation algorithm used for a segment by
     *using the LightingAttributeControl::SetInterpolationAlgorithm() method
     * 4. Segment attributes are changed as needed through the use of kits and
     *controls
     *
     * NOTE: Lights do not shine through sub-windows, therefore it is necessary
     *to insert a new light per sub-window to see the results of the lighting
     *algorithms*/
    SegmentKey wireframeKey = modelKey.Subsegment();
    subwindowKit.SetSubwindow(HPS::Rectangle(-1, -0.33f, 0, 1));
    wireframeKey.GetVisibilityControl()
        .SetFaces(false)
        .SetEdges(true)
        .SetLights(false);
    wireframeKey.IncludeSegment(urn);
    wireframeKey.SetSubwindow(subwindowKit);
    wireframeKey.SetVisibility(visibilityKit);
    wireframeKey.InsertText(Point(0, 1.40f, 0), "Wireframe");

    // Index Interpolated rendering
    SegmentKey indexInterpolatedKey = modelKey.Subsegment();
    indexInterpolatedKey.InsertDistantLight(Vector(1, 1, 0));
    subwindowKit.SetSubwindow(HPS::Rectangle(-0.33f, 0.33f, 0, 1));
    indexInterpolatedKey.IncludeSegment(urn);
    indexInterpolatedKey.SetSubwindow(subwindowKit);
    indexInterpolatedKey.GetColorInterpolationControl().SetFaceColor(true);
    indexInterpolatedKey.GetVisibilityControl().SetLights(true);
    indexInterpolatedKey.InsertText(Point(0, 1.40f, 0),
                                    "Color Index Interpolated");

    // Gouraud Shaded
    SegmentKey gouraudKey = modelKey.Subsegment();
    gouraudKey.InsertDistantLight(Vector(1, 1, 0));
    gouraudKey.GetLightingAttributeControl().SetInterpolationAlgorithm(
        Lighting::InterpolationAlgorithm::Gouraud);
    subwindowKit.SetSubwindow(HPS::Rectangle(0.33f, 1, 0, 1));
    gouraudKey.IncludeSegment(urn);
    gouraudKey.SetSubwindow(subwindowKit);
    gouraudKey.GetVisibilityControl().SetLights(true);
    gouraudKey.InsertText(Point(0, 1.40f, 0), "Gouraud");

    // Hidden Surface and Lines
    SegmentKey hiddenSurfaceKey = modelKey.Subsegment();
    subwindowKit.SetSubwindow(HPS::Rectangle(-1, -0.33f, -1, 0));
    visibilityKit.UnsetEverything().SetFaces(true).SetEdges(true).SetLights(
        false);
    hiddenSurfaceKey.IncludeSegment(urn);
    hiddenSurfaceKey.SetSubwindow(subwindowKit);
    hiddenSurfaceKey.SetVisibility(visibilityKit);
    hiddenSurfaceKey.GetSubwindowControl().SetRenderingAlgorithm(
        Subwindow::RenderingAlgorithm::HiddenLine);
    hiddenSurfaceKey.GetHiddenLineAttributeControl()
        .SetVisibility(false)
        .SetLinePattern("-  -  -")
        .SetDimFactor(0.5f)
        .SetFaceDisplacement(0.5f);
    hiddenSurfaceKey.InsertText(Point(0, 1.40f, 0), "True Hidden Line");

    // Flat Shaded
    SegmentKey flatKey = modelKey.Subsegment();
    flatKey.InsertDistantLight(Vector(1, 1, 0));
    flatKey.GetLightingAttributeControl().SetInterpolationAlgorithm(
        Lighting::InterpolationAlgorithm::Flat);
    subwindowKit.SetSubwindow(HPS::Rectangle(-0.33f, 0.33f, -1, 0));
    flatKey.IncludeSegment(urn);
    flatKey.SetSubwindow(subwindowKit);
    flatKey.InsertText(Point(0, 1.40f, 0), "Flat Shaded");

    // Phong Shaded
    phongKey.InsertDistantLight(Vector(1, 1, 0));
    phongKey.GetLightingAttributeControl().SetInterpolationAlgorithm(
        Lighting::InterpolationAlgorithm::Phong);
    phongKey.GetVisibilityControl().SetLights(true);
    subwindowKit.SetSubwindow(HPS::Rectangle(0.33f, 1, -1, 0));
    phongKey.IncludeSegment(urn);
    phongKey.SetSubwindow(subwindowKit);
    phongKey.InsertText(Point(0, 1.40f, 0), "Phong");

    view.FitWorld().Update();
}
