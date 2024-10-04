#if defined __APPLE__
#  include "SamplesMacOS.h"
#else
#  include "sample.h"
#endif

#include "sample_common.h"
#include "sprk.h"

using namespace HPS;

void Sample::Mask_Transforms(WindowKey wk, Canvas canvas, View view, Model model, SegmentKey modelKey)
{

    /* Sample: Mask_Transforms
     * This sample will show you how to:
     * - Insert a shell
     * - Mask modeling matrix rotation, scaling and translation
     * - Rotate, scale and translate a scene
     *
     * Read more about the topics covered in this sample in our Programming Guide */

    /* This block code shows how to create a new root segment and add geometry to it
     * 1. Root segments are special segments, which can only be created from the Database. Since this root segment
     *		is not associated with a window, its contents will not be displayed on the screen. This segment
     *		will be used to store geometry which can later be included in segments which are under a window
     * 2. The CreateBox method inserts a box in the segment whose key is passed as the first argument
     * 3. To insert a shell, as seen in the body of the CreateBox function, a list of points and faces is needed*/
    SegmentKey includeSegment = Database::CreateRootSegment();
    CreateBox(includeSegment, Point(-1, -1, -1), Point(1, 1, 1));

    SubwindowKit subwindowKit;
    VisibilityKit visibilityKit;

    visibilityKit.SetFaces(true).SetEdges(true);
    modelKey.SetVisibility(visibilityKit);
    modelKey.GetMaterialMappingControl().SetFaceColor(RGBAColor(0, 1, 0, 1)).SetEdgeColor(RGBAColor(0, 0, 1, 1));
    modelKey.GetTextAttributeControl().SetSize(0.02f, Text::SizeUnits::WindowRelative).SetAlignment(Text::Alignment::Center);

    /* This block of code shows how to mask the modelling matrix's rotation
     * Here we have two segments: maskRotationSegment and r_masked.
     * The r_masked segment is a child of the maskRotationSegment segment
     *
     * Each segment contains a cube, which is included from the root segment previously created
     * All geometry is set to have green faces. This attribute is overridden in r_masked where
     * face color is changes to red.
     *
     * Modelling matrix rotation is masked in r_masked by using a ModellingMatrixControl */
    SegmentKey maskRotationSegment = modelKey.Subsegment();
    subwindowKit.SetSubwindow(HPS::Rectangle(-1, -0.33f, -1, 1));
    maskRotationSegment.IncludeSegment(includeSegment);
    maskRotationSegment.SetSubwindow(subwindowKit);
    maskRotationSegment.InsertText(Point(0, 1.85f, 0), "Masking Rotation");

    SegmentKey r_masked = maskRotationSegment.Subsegment();
    r_masked.IncludeSegment(includeSegment);
    r_masked.GetModellingMatrixControl().Translate(0, -2.5f, 0);
    r_masked.GetMaterialMappingControl().SetFaceColor(RGBAColor(1, 0, 0, 1));

    /* This block of code shows how to mask the modelling matrix's rotation
     * Here we have two segments: maskTranslationSegment and t_masked.
     * The t_masked segment is a child of the maskTranslationSegment segment
     *
     * Each segment contains a cube, which is included from the root segment previously created
     * All geometry is set to have green faces. This attribute is overridden in t_masked where
     * face color is changes to red.
     *
     * Modelling matrix translation is masked in t_masked by using a ModellingMatrixControl */
    SegmentKey maskTranslationSegment = modelKey.Subsegment();
    subwindowKit.SetSubwindow(HPS::Rectangle(-0.33f, 0.33f, -1, 1));
    maskTranslationSegment.IncludeSegment(includeSegment);
    maskTranslationSegment.SetSubwindow(subwindowKit);
    maskTranslationSegment.InsertText(Point(0, 1.85f, 0), "Masking Translation");

    SegmentKey t_masked = maskTranslationSegment.Subsegment();
    t_masked.IncludeSegment(includeSegment);
    t_masked.GetModellingMatrixControl().Translate(0, -2.5f, 0);
    t_masked.GetMaterialMappingControl().SetFaceColor(RGBAColor(1, 0, 0, 1));

    /* This block of code shows how to mask the modelling matrix's rotation
     * Here we have two segments: maskScaleSegment and s_masked.
     * The s_masked segment is a child of the maskScaleSegment segment
     *
     * Each segment contains a cube, which is included from the root segment previously created
     * All geometry is set to have green faces. This attribute is overridden in s_masked where
     * face color is changes to red.
     *
     * Modelling matrix translation is masked in s_masked by using a ModellingMatrixControl */
    SegmentKey maskScalingSegment = modelKey.Subsegment();
    subwindowKit.SetSubwindow(HPS::Rectangle(0.33f, 1, -1, 1));
    maskScalingSegment.IncludeSegment(includeSegment);
    maskScalingSegment.SetSubwindow(subwindowKit);
    maskScalingSegment.InsertText(Point(0, 1.85f, 0), "Masking Scaling");

    SegmentKey s_masked = maskScalingSegment.Subsegment();
    s_masked.IncludeSegment(includeSegment);
    s_masked.GetModellingMatrixControl().Translate(0, -2.5f, 0);
    s_masked.GetMaterialMappingControl().SetFaceColor(RGBAColor(1, 0, 0, 1));

    view.FitWorld();
    view.Update();

    r_masked.GetTransformMaskControl().SetModellingMatrixRotation(true);
    t_masked.GetTransformMaskControl().SetModellingMatrixTranslation(true);
    s_masked.GetTransformMaskControl().SetModellingMatrixScale(true);

    /* This next block of code rotates the maskRotationSegment. We expect that the green cube in that segment will
     * rotate, while the red cube in its r_masked subsegment will stay still since rotation is masked.
     *
     * The same will happen for the segments masking scaling and rotation, relative to those transformations*/
    for (int i = 0; i < 360; i++) {
        maskRotationSegment.GetModellingMatrixControl().Rotate(0, 1, 0);
        maskScalingSegment.GetModellingMatrixControl().Scale(1.001f, 1.001f, 1.001f);
        maskTranslationSegment.GetModellingMatrixControl().Translate(0, 0.001f, 0);

        view.Update();
    }


}
