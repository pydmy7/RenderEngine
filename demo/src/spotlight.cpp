#if defined __APPLE__
#  include "SamplesMacOS.h"
#else
#  include "sample.h"
#endif

#include "sample_common.h"
#include "sprk.h"

using namespace HPS;

void Sample::Spotlight(WindowKey wk, Canvas canvas, View view, Model model, SegmentKey modelKey)
{

    /* Sample: Spotlight
     * This sample will show you how to:
     * - Insert a shell
     * - Set segment level attributes
     * - Insert a spotlight
     *
     * Read more about the topics covered in this sample in our Programming Guide
     * - section 2.4:	Lights */

    view.GetSegmentKey().GetLightingAttributeControl().SetInterpolationAlgorithm(Lighting::InterpolationAlgorithm::Gouraud);
    view.GetSegmentKey().GetDrawingAttributeControl().SetPolygonHandedness(Drawing::Handedness::Right);

    /* This block code shows how to create a new root segment and add geometry to it
     * 1. Root segments are special segments, which can only be created from the Database. Since this root segment
     *		is not associated with a window, its contents will not be displayed on the screen. This segment
     *		will be used to store geometry which can later be included in segments which are under a window
     * 2. The CreateBox method inserts a box in the segment whose key is passed as the first argument
     * 3. To insert a shell, as seen in the body of the CreateBox function, a list of points and faces is needed*/
    SegmentKey includeSegment = Database::CreateRootSegment();
    CreateBox(includeSegment, Point(-1, -1, -1), Point(1, 1, 1));

    /* This block of code sets segment level attributes and includes geometry into segments
     * 1. To include a segment into another segment use the following method:
     *		destinationSegmentKey.IncludeSegment(sourceSegmentKey)
     *		including segments is a good way to add the same geometry in multiple places without
     *		needlessly creating multiple copies of the geometry in question
     * 2. A different color is applied to each segment using the MaterialMapping control*/
    SegmentKey redBox = modelKey.Subsegment();
    redBox.GetMaterialMappingControl().SetFaceColor(RGBAColor(1, 0, 0, 1));
    redBox.IncludeSegment(includeSegment);

    SegmentKey greenBox = modelKey.Subsegment();
    greenBox.GetMaterialMappingControl().SetFaceColor(RGBAColor(0, 1, 0, 1));
    greenBox.IncludeSegment(includeSegment);
    greenBox.GetModellingMatrixControl().Translate(2.5f, 0, 0);

    SegmentKey blueBox = modelKey.Subsegment();
    blueBox.GetMaterialMappingControl().SetFaceColor(RGBAColor(0, 0, 1, 1));
    blueBox.IncludeSegment(includeSegment);
    blueBox.GetModellingMatrixControl().Translate(5, 0, 0);

    view.FitWorld();
    view.GetSegmentKey().GetCameraControl().Orbit(35, 25);
    view.Update();

    /* This block of code shows how to insert a spotlight
     * 1. Make sure that light visibility is on
     * 2. Set the position and target or the spotlight, in this case they coincide with those of the camera
     * 3. Set the size of the inner and outer cone of the spotlight
     * 4. Apply the spotlight kit to the segment where the spotlight will be inserted */
    CameraKit cameraKit;
    view.GetSegmentKey().ShowCamera(cameraKit);
    Point position, target;
    cameraKit.ShowPosition(position);
    cameraKit.ShowTarget(target);

    view.GetSegmentKey().GetVisibilityControl().SetLights(true);
    SpotlightKit spotlightKit;
    spotlightKit.SetPosition(position)
        .SetTarget(target)
        .SetInnerCone(5, Spotlight::InnerConeUnits::Degrees)
        .SetOuterCone(13, Spotlight::OuterConeUnits::Degrees);

    SpotlightKey sKey = view.GetSegmentKey().InsertSpotlight(spotlightKit);

}
