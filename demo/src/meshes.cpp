#if defined __APPLE__
#  include "SamplesMacOS.h"
#else
#  include "sample.h"
#endif

#include "sample_common.h"
#include "sprk.h"

using namespace HPS;

void Sample::Meshes(WindowKey wk, Canvas canvas, View view, Model model, SegmentKey modelKey)
{

    /* Sample: Meshes
     * This sample will show you how to:
     * - Insert a mesh
     * - Set normals on a mesh
     * - Include a segment into other segments
     * - Set segment level attributes
     * - Change the modelling matrix
     *
     * Read more about the topics covered in this sample in our Programming Guide
     * - section 2.2:	Meshes*/

    /* This block code shows how to create a new root segment and add geometry to it
     * 1. Root segments are special segments, which can only be created from the Database. Since this root segment
     *		is not associated with a window, its contents will not be displayed on the screen. This segment
     *		will be used to store geometry which can later be included in segments which are under a window
     * 2. The CreateToroid method inserts a toroid in the segment whose key is passed as the first argument
     * 3. Please see the body of CreateToroid for details on how to insert a mesh and set its normals */
    SegmentKey includeLibrary = Database::CreateRootSegment();
    SegmentKey toroid = includeLibrary.Subsegment();
    CreateToroid(toroid);

    SegmentKey ringsKey = modelKey.Subsegment();
    ringsKey.GetVisibilityControl().SetMarkers(false).SetEdges(false);

    /* This block of code creates three segments below modelKey and includes geometry in them
     * 1. To create a child segment use the following method:
     *		SegmentKey childSegmentKey= parentSegmentKey.Subsegment("optional_child_segment_name")
     * 2. To include a segment into another segment use the following method:
     *		destinationSegmentKey.IncludeSegment(sourceSegmentKey)
     *		including segments is a good way to add the same geometry in multiple places without
     *		needlessly creating multiple copies of the geometry in question
     *	3. Segment level attributes can be set either with kits or controls. Controls interact directly
     *		with the database, while kits exist outside of it, and only change the database when they are applied
     *		on a segment. Attributes not explicitly set on a segment are inherited from parent segments.
     *		Attributes set at a shell level have precedence over those set at the segment level */
    SegmentKey redRingKey = ringsKey.Subsegment("red_ring");
    redRingKey.IncludeSegment(toroid);
    redRingKey.GetMaterialMappingControl().SetFaceColor(RGBAColor(1, 0, 0, 1));

    SegmentKey greenRingKey = ringsKey.Subsegment("green_ring");
    greenRingKey.IncludeSegment(toroid);
    greenRingKey.GetMaterialMappingControl().SetFaceColor(RGBAColor(0, 1, 0, 1));
    greenRingKey.GetModellingMatrixControl().Rotate(90, 0, 90);

    SegmentKey blueRingKey = ringsKey.Subsegment("blue_ring");
    blueRingKey.IncludeSegment(toroid);
    blueRingKey.GetMaterialMappingControl().SetFaceColor(RGBAColor(0, 0, 1, 1));
    blueRingKey.GetModellingMatrixControl().Rotate(90, 90, 0);

    view.FitWorld();
    view.GetSegmentKey().GetCameraControl().Orbit(35, 25);

}
