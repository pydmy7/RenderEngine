#if defined __APPLE__
#include "SamplesMacOS.h"
#else
#include "sample.h"
#endif

#include "sample_common.h"
#include "sprk.h"

using namespace HPS;

void Sample::Priority(WindowKey wk, Canvas canvas, View view, Model model,
                      SegmentKey modelKey) {
    /* Sample: Priority
     * This sample will show you how to:
     * - Control the drawing order of co-planar geometry using SetPriority()
     *
     * Read more about the topics covered in this sample in our Programming
     * Guide
     * - section 7.6.1:		Priority */

    /* This block of code sets up the scene graph:
     * Two segments are created from the model segment.
     * The circles segment will be used to demonstrate the use of priority on
     * geometry residing in different segments The shapes segment will be used
     * to demonstrate the use of priority on geometry sharing the same segment
     *
     * In order for priority to take effect
     * set the rendering algorithm to priority */
    view.Update();
    modelKey.GetVisibilityControl().SetEdges(true);
    modelKey.GetTextAttributeControl().SetAlignment(
        Text::Alignment::CenterRight);

    SegmentKey circles = modelKey.Subsegment();
    circles.GetSubwindowControl().SetSubwindow(HPS::Rectangle(-1, 0, -1, 1));
    SegmentKey shapes = modelKey.Subsegment();
    shapes.GetSubwindowControl().SetSubwindow(HPS::Rectangle(0, 1, -1, 1));

    circles.GetSubwindowControl().SetRenderingAlgorithm(
        Subwindow::RenderingAlgorithm::Priority);
    shapes.GetSubwindowControl().SetRenderingAlgorithm(
        Subwindow::RenderingAlgorithm::Priority);

    SegmentKey circle1 = circles.Subsegment();
    SegmentKey circle2 = circles.Subsegment();
    SegmentKey circle3 = circles.Subsegment();

    CircleKey c1 = circle1.InsertCircle(Point(0, 0, 0), 1.0f, Vector(0, 0, 1));
    CircleKey c2 =
        circle2.InsertCircle(Point(0.5f, 0, 0), 1.0f, Vector(0, 0, 1));
    CircleKey c3 = circle3.InsertCircle(Point(1, 0, 0), 1.0f, Vector(0, 0, 1));

    circle1.GetMaterialMappingControl().SetFaceColor(RGBAColor(1, 0, 0, 1));
    circle2.GetMaterialMappingControl().SetFaceColor(RGBAColor(0, 1, 0, 1));
    circle3.GetMaterialMappingControl().SetFaceColor(RGBAColor(0, 0, 1, 1));

    /* This block of code sets priority on the geometry from different segments
     * Since segment 'c1', representing a green circle, has the highest priority
     * it will appear on top */
    c1.SetPriority(1);
    c2.SetPriority(3);
    c3.SetPriority(2);

    circles.InsertText(Point(0, -1.8f, 0), "green circle on top");

    shapes.GetMaterialMappingControl().SetFaceColor(RGBAColor(1, 0.8f, 0.5f));
    PointArray points;
    points.push_back(Point(0, 0.5f, 0));
    points.push_back(Point(1, 0.5f, 0));
    points.push_back(Point(1, -0.5f, 0));
    points.push_back(Point(0, -0.5f, 0));
    CircleKey shape1 =
        shapes.InsertCircle(Point(0, 0, 0), 1.0f, Vector(0, 0, 1));
    PolygonKey shape2 = shapes.InsertPolygon(points);
    CircleKey shape3 =
        shapes.InsertCircle(Point(1, 0, 0), 1.0f, Vector(0, 0, 1));

    /* This block of code sets priority on geometry sharing the same segment
     * Since the polygon has the highest priority, it will be drawn on top */
    shape1.SetPriority(1);
    shape2.SetPriority(4);
    shape3.SetPriority(2);

    shapes.InsertText(Point(0, -1.8f, 0), "square on top");

    CameraKit cameraKit;
    cameraKit.SetPosition(Point(0, 0, -5))
        .SetTarget(Point(0, 0, 0))
        .SetUpVector(Vector(0, 1, 0))
        .SetField(4, 4)
        .SetProjection(Camera::Projection::Orthographic);
    view.GetSegmentKey().SetCamera(cameraKit);
}
