#if defined __APPLE__
#  include "SamplesMacOS.h"
#else
#  include "sample.h"
#endif

#include "sample_common.h"
#include "sprk.h"

using namespace HPS;

void Sample::Segment_Attributes(WindowKey wk, Canvas canvas, View view, Model model, SegmentKey modelKey)
{

    /* Sample: Segment_Attributes
     * This sample will show you how to:
     * - Insert a shell
     * - Divide the screen into sub-window
     * - Include a segment into other segments
     * - Change segment level attributes
     * - Use transparency
     *
     * Read more about the topics covered in this sample in our Programming Guide
     * - section 1.2.3:	Attributes
     * - section 4.3:	Color
     * - section 3.3:	Sub-Windows*/

    /* This block code shows how to create a new root segment and add geometry to it
     * 1. Root segments are special segments, which can only be created from the Database. Since this root segment
     *		is not associated with a window, its contents will not be displayed on the screen. This segment
     *		will be used to store geometry which can later be included in segments which are under a window
     * 2. The CreateBox method inserts a box in the segment whose key is passed as the first argument
     * 3. To insert a shell, as seen in the body of the CreateBox function, a list of points and faces is needed*/
    SegmentKey includeLibrary = Database::CreateRootSegment();
    SegmentKey cube = includeLibrary.Subsegment("cube");
    CreateBox(cube, Point(-1, -1, -1), Point(1, 1, 1));

    SubwindowKit subwindowKit;
    VisibilityKit visibilityKit;

    /* This block of code sets face and edge color for the modelKey segment
     * These attributes will be inherited by the subsegments of modelKey, unless overwritten
     * To set colors on different types of geometry, use the MaterialMapping control*/
    visibilityKit.SetFaces(true).SetEdges(true).SetMarkers(false);
    modelKey.SetVisibility(visibilityKit);
    modelKey.GetMaterialMappingControl().SetFaceColor(RGBAColor(1, 0, 0, 1)).SetEdgeColor(RGBAColor(0, 0, 1, 1));
    modelKey.GetTextAttributeControl().SetAlignment(Text::Alignment::Center);

    /* This block of code divides the screen into sub-windows.
     * Each segment gets its own sub-window, all segments are direct children of the modelKey segment
     * 1. To set a sub-window call the SetSubwindow method with the dimensions of the sub-window,
     *		normalized between -1 and 1, with (-1, 1) pointing to the top left corner
     * 2. To include a segment into another segment use the following method:
     *		destinationSegmentKey.IncludeSegment(sourceSegmentKey)
     *		including segments is a good way to add the same geometry in multiple places without
     *		needlessly creating multiple copies of the geometry in question
     * 3. We expect the attributes of each sub-window to be the same of those of the parents
     *		segments, unless explicitly modified. There is no need to explicitly specify an attribute if
     *		it is getting inherited from a parent segment
     * 4. To add transparency to the contents of a segment, change the alpha value of the RGBAColor passed
     *		to one of the methods responsible for changing the color of geometry*/
    modelKey.GetSubwindowControl().SetBackground(Subwindow::Background::Transparent);

    SegmentKey box = modelKey.Subsegment("box");
    subwindowKit.SetSubwindow(HPS::Rectangle(-1, -0.33f, 0, 1));
    box.IncludeSegment(cube);
    box.SetSubwindow(subwindowKit);
    box.InsertText(Point(0, 1.25f, 0), "A Red box");

    SegmentKey hiddenBox = modelKey.Subsegment("hidden_box");
    subwindowKit.SetSubwindow(HPS::Rectangle(-0.33f, 0.33f, 0, 1));
    visibilityKit.SetFaces(false);
    hiddenBox.IncludeSegment(cube);
    hiddenBox.SetSubwindow(subwindowKit);
    hiddenBox.SetVisibility(visibilityKit);
    hiddenBox.InsertText(Point(0, 1.25f, 0), "Faces Hidden");

    SegmentKey transparentBox = modelKey.Subsegment("transparent_box");
    subwindowKit.SetSubwindow(HPS::Rectangle(0.33f, 1, 0, 1));
    transparentBox.IncludeSegment(cube);
    transparentBox.SetSubwindow(subwindowKit);
    transparentBox.GetMaterialMappingControl().SetFaceColor(RGBAColor(1, 0, 0, 0.5f));
    transparentBox.InsertText(Point(0, 1.25f, 0), "Transparent Box");

    SegmentKey yellowBox = modelKey.Subsegment("yellow_box");
    subwindowKit.SetSubwindow(HPS::Rectangle(-1, -0.33f, -1, 0));
    yellowBox.IncludeSegment(cube);
    yellowBox.SetSubwindow(subwindowKit);
    yellowBox.GetMaterialMappingControl().SetFaceColor(RGBAColor(1, 1, 0, 1));
    yellowBox.InsertText(Point(0, 1.25f, 0), "A Yellow Box");

    SegmentKey hiddenEdgesBox = modelKey.Subsegment("edges_hidden");
    subwindowKit.SetSubwindow(HPS::Rectangle(-0.33f, 0.33f, -1, 0));
    hiddenEdgesBox.IncludeSegment(cube);
    visibilityKit.SetEdges(false).SetFaces(true);
    hiddenEdgesBox.SetVisibility(visibilityKit);
    hiddenEdgesBox.SetSubwindow(subwindowKit);
    hiddenEdgesBox.InsertText(Point(0, 1.25f, 0), "Edges Hidden");

    SegmentKey deletedBox = modelKey.Subsegment("deleted_box");
    subwindowKit.SetSubwindow(HPS::Rectangle(0.33f, 1, -1, 0));
    IncludeKey includedBox = deletedBox.IncludeSegment(cube);
    includedBox.Delete();
    deletedBox.SetSubwindow(subwindowKit);
    deletedBox.InsertText(Point(0, 1.25f, 0), "The Box is Gone!");

    view.FitWorld();
    view.GetSegmentKey().GetCameraControl().Orbit(35, 25);

}
