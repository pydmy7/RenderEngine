#if defined __APPLE__
#  include "SamplesMacOS.h"
#else
#  include "sample.h"
#endif

#include "sample_common.h"
#include "sprk.h"

using namespace HPS;

void Sample::Hidden_Line_Advanced(WindowKey wk, Canvas canvas, View view, Model model, SegmentKey modelKey)
{

    /* Sample: Hidden_Line_Advanced
     * This sample will show you how to:
     * - Load an HSF file through Stream and check for Stream IO exceptions
     * - Change world handedness
     * - Change culling settings
     * - Change the rendering algorithm to FastHiddenLine
     * - Add simple shadows to a scene
     * - Add transparency to a scene
     *
     * Read more about the topics covered in this sample in our Programming Guide
     * - section 7.6:	Hidden Surface Removal
     * - section 7.7:	Hidden Line Removal
     * - section 7.2:	Shadows
     * - section 4.5:	Transparency
     * - section 6.3.4:	Culling Optimizations */

    /* The next block of code shows how to change world handedness and how to change culling settings
     * 1. World Handedness is a drawing attribute. From a SegmentKey object, obtain its DrawingAttributeControl
     *    and use it to set the world handedness
     * 2. In order to change culling options, obtain a CullingControl object
     * 3. Using a CullingControl object, change the extent, back-face, frustum and vector culling as desired*/
    view.GetSegmentKey().GetDrawingAttributeControl().SetWorldHandedness(HPS::Drawing::Handedness::Right);
    view.GetSegmentKey().GetCullingControl().SetExtent(false, 0);

    /* This next block of code shows you how to load an HSF file
     * 1. Create an ImportOptionsKit and set the segment in which you want the file to be loaded
     * 2. Use the Stream::File::Import method to load the desired file, with the options you specified
     * 3. Since this operation happens on a separate thread, wait for it to complete before proceeding
     * 4. Use a try-catch block to capture any Stream IO exceptions */
    Stream::ImportNotifier notifier;
    try {
        Stream::ImportOptionsKit importOptionsKit;
        importOptionsKit.SetSegment(modelKey);
        UTF8 filename = INPUT_FOLDER + "/burj.hsf";

        notifier = Stream::File::Import(static_cast<char const*>(filename), importOptionsKit);
        notifier.Wait();
    }
    catch (IOException e) {
        // handle Stream IO errors here
    }

    CameraKit cameraKit;
    view.FitWorld();
    cameraKit.SetPosition(Point(49474.261f, -186011.625f, 35299.847f))
        .SetTarget(Point(-4773.888f, -220777.984f, 6612.253f))
        .SetUpVector(Vector(-0.3424f, -0.2194f, 0.9135f))
        .SetField(19948.931f, 19948.931f)
        .SetProjection(Camera::Projection::Orthographic);
    view.GetSegmentKey().SetCamera(cameraKit);

    /* The next block of code shows how to add Simple Shadows to a scene
     * 1. Obtain the VisualEffectsControl from a SegmentKey object
     * 2. Turn on simple shadows by calling SetSimpleShadow(true)
     * 3. Additional shadows attribute that can be set include the position of the plane on which
     *    shadows will be rendered, the direction of the light used for shadow generation,
     *    and the color of the shadows
     * 4. Enable shadow visibility through a VisibilityControl or a VisibilityKit */
    view.GetSegmentKey()
        .GetVisualEffectsControl()
        .SetSimpleShadow(true)
        .SetSimpleShadowPlane(Plane(0, 0, -1, -131.528f))
        .SetSimpleShadowLightDirection(Vector(0.66667f, -0.333335f, 0.66667f));
    view.GetSegmentKey().GetVisibilityControl().SetShadows(true);

    /* The next block of code shows how to add transparency to a scene
     * 1. Obtain the TransparencyControl object from a SegmentKey object
     * 2. Choose the transparency method: Blended, ScreenDoor or Off
     * 4. Choose the algorithm used for transparency calculations. In this case Depth Peeling is selected
     * 5. Some transparency algorithm have special options - for example when choosing Depth Peeling, the
     *    number of depth peeling layers can be selected. In this example 2 layers are used
     * 6. Add an alpha component to the material color of the scene elements which will be displayed as transparent.
     *    In this example, faces will be 80% transparent while edges will be completely opaque. */
    view.GetSegmentKey()
        .GetTransparencyControl()
        .SetAlgorithm(Transparency::Algorithm::DepthPeeling)
        .SetDepthPeelingLayers(2)
        .SetMethod(Transparency::Method::Blended);
    modelKey.GetMaterialMappingControl().SetFaceColor(RGBAColor(1, 1, 1, 0.2f)).SetEdgeColor(RGBAColor(0, 0, 1, 1));

    /* The next block of code shows how to use the FastHiddenLine rendering algorithm
     * 1. Obtain the SubwindowControl object, and change the rendering algorithm with a call to SetRenderingAlgorithm
     * 2. To modify hidden line attributes, obtain the HiddenLineAttributeControl object
     * 3. In this example faces are rendered so that shadow calculation can take place even while in hidden line*/
    modelKey.GetVisibilityControl().SetFaces(true).SetEdges(true).SetLines(true);
    modelKey.GetSubwindowControl().SetRenderingAlgorithm(Subwindow::RenderingAlgorithm::FastHiddenLine);
    modelKey.GetHiddenLineAttributeControl().SetRenderFaces(true).SetVisibility(true);


}
