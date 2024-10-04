#if defined __APPLE__
#  include "SamplesMacOS.h"
#else
#  include "sample.h"
#endif

#include "sample_common.h"
#include "sprk.h"

using namespace HPS;

void Sample::Sub_Shell_Attributes(WindowKey wk, Canvas canvas, View view, Model model, SegmentKey modelKey)
{

    /* Sample: Shell_Attributes
     * This sample will show you how to:
     * - Insert a shell
     * - Divide the screen into sub-window
     * - Change shell level attributes
     *
     * Read more about the topics covered in this sample in our Programming Guide
     * - section 2.1:	Shells */

    SubwindowKit subwindowKit;
    VisibilityKit visibilityKit;

    /* This block of code sets visibility and color attributes at the segment level
     * Shell level attributes take precedence over segment level attributes */
    visibilityKit.SetFaces(true).SetEdges(true).SetVertices(true);
    modelKey.SetVisibility(visibilityKit);
    modelKey.GetMaterialMappingControl()
        .SetFaceColor(RGBAColor(1, 0, 0, 1))
        .SetEdgeColor(RGBAColor(0, 0, 1, 1))
        .SetVertexColor(RGBAColor(0, 1, 0, 1));
    modelKey.GetTextAttributeControl().SetAlignment(Text::Alignment::Center);

    /* This block of code divides the screen into sub-windows.
     * Each segment gets its own sub-window, all segments are direct children of the modelKey segment
     * 1. To set a sub-window call the SetSubwindow method with the dimensions of the sub-window,
     *		normalized between -1 and 1, with (-1, 1) pointing to the top left corner */
    modelKey.GetSubwindowControl().SetBackground(Subwindow::Background::Transparent);

    // Vertex Visibility
    SegmentKey box = modelKey.Subsegment("box");
    subwindowKit.SetSubwindow(HPS::Rectangle(-1, 0, 0, 1));
    ShellKey sk1 = CreateBox(box, Point(-1, -1, -1), Point(1, 1, 1));
    sk1.SetVertexVisibilitiesByRange(0, 8, false);
    box.SetSubwindow(subwindowKit);
    box.InsertText(Point(0, 2.0f, 0), "Hidden Markers");

    // Hidden Faces
    SegmentKey hiddenBox = modelKey.Subsegment("hidden_box");
    subwindowKit.SetSubwindow(HPS::Rectangle(0, 1, 0, 1));
    ShellKey sk2 = CreateBox(hiddenBox, Point(-1, -1, -1), Point(1, 1, 1));
    sk2.SetFaceVisibilitiesByRange(0, 6, false);
    hiddenBox.SetSubwindow(subwindowKit);
    hiddenBox.InsertText(Point(0, 2.0f, 0), "Hidden Faces");

    // Change of Color
    SegmentKey yellowBox = modelKey.Subsegment("yellow_box");
    subwindowKit.SetSubwindow(HPS::Rectangle(-1, 0, -1, 0));
    ShellKey sk3 = CreateBox(yellowBox, Point(-1, -1, -1), Point(1, 1, 1));
    sk3.SetFaceRGBColorsByRange(0, 6, RGBColor(1, 1, 0));
    yellowBox.SetSubwindow(subwindowKit);
    yellowBox.InsertText(Point(0, 2.0f, 0), "A Yellow Box");

    // Deleted Box
    SegmentKey deletedBox = modelKey.Subsegment("deleted_box");
    subwindowKit.SetSubwindow(HPS::Rectangle(0, 1, -1, 0));
    ShellKey sk4 = CreateBox(deletedBox, Point(-1, -1, -1), Point(1, 1, 1));
    sk4.Delete();
    deletedBox.SetSubwindow(subwindowKit);
    deletedBox.InsertText(Point(0, 2.0f, 0), "The Box is Gone!");

    view.FitWorld();
    view.GetSegmentKey().GetCameraControl().Orbit(35, 25);

}
