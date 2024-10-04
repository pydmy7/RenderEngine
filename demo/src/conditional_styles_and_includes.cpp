#if defined __APPLE__
#include "SamplesMacOS.h"
#else
#include "sample.h"
#endif

#include "sample_common.h"
#include "sprk.h"

using namespace HPS;

void Sample::Conditional_Styles_And_Includes(WindowKey wk, Canvas canvas,
                                             View view, Model model,
                                             SegmentKey modelKey) {
    /* Sample: Conditional_Styles_And_Includes
     * This sample will show you how to:
     * - Set styles only when certain conditions are met
     * - Include geometry only when certain conditions are met
     * - Create named styles
     * - Build complex conditions using logical operators
     *
     * Read more about the topics covered in this sample in our Programming
     * Guide
     * - section 4.1.3:		Using Conditional Styles
     * - section 4.1.2:		Defining and Using Styles */

    /* This next block of code shows you how to load an HSF file
     * 1. Create an ImportOptionsKit and set the segment in which you want the
     * file to be loaded
     * 2. Use the Stream::File::Import method to load the desired file, with the
     * options you specified
     * 3. Since this operation happens on a separate thread, wait for it to
     * complete before proceeding
     * 4. Use a try-catch block to capture any Stream IO exceptions */
    SegmentKey model1 = Database::CreateRootSegment();
    SegmentKey model2 = Database::CreateRootSegment();
    Stream::ImportNotifier notifier;
    try {
        Stream::ImportOptionsKit importOptionsKit;
        importOptionsKit.SetSegment(model1);
        UTF8 filename = INPUT_FOLDER + "/bnc.hsf";

        notifier = Stream::File::Import(static_cast<char const*>(filename),
                                        importOptionsKit);
        notifier.Wait();

        filename = INPUT_FOLDER + "/urn.hsf";
        importOptionsKit.SetSegment(model2);
        notifier = Stream::File::Import(static_cast<char const*>(filename),
                                        importOptionsKit);
        notifier.Wait();
    } catch (IOException e) {
        // handle Stream IO errors here
    }

    /* The next block of code sets up the scene:
     * 1. Four segments are created from the modelKey segment
     * 2. Each of the four sub-segments is given its own subwindow
     * 3. Since light does not transmit through windows, each subwindow needs a
     * light source
     * 4. The second model is scaled down so that its size is comparable to the
     * first model */
    model2.GetModellingMatrixControl()
        .Scale(0.01f, 0.01f, 0.01f)
        .Translate(0, 0.03f, 0);
    SegmentKey topLeft, topRight, bottomLeft, bottomRight;
    topLeft = modelKey.Subsegment();
    topLeft.GetSubwindowControl().SetSubwindow(HPS::Rectangle(-1, 0, 0, 1));
    topLeft.InsertDistantLight(Vector(1, 1, 0));
    topRight = modelKey.Subsegment();
    topRight.GetSubwindowControl().SetSubwindow(HPS::Rectangle(0, 1, 0, 1));
    topRight.InsertDistantLight(Vector(1, 1, 0));
    bottomLeft = modelKey.Subsegment();
    bottomLeft.GetSubwindowControl().SetSubwindow(HPS::Rectangle(-1, 0, -1, 0));
    bottomLeft.InsertDistantLight(Vector(1, 1, 0));
    bottomRight = modelKey.Subsegment();
    bottomRight.GetSubwindowControl().SetSubwindow(HPS::Rectangle(0, 1, -1, 0));
    bottomRight.InsertDistantLight(Vector(1, 1, 0));

    /* This block of code defines four named styles
     * 1. For each style create a segment which contains the attribute for the
     *style
     * 2. Obtain the key for a portfolio
     * 3. Define named styles by giving them a name and the segment key that
     *contains the attributes for each style */
    SegmentKey style1, style2, style3, style4;
    style1 = modelKey.Subsegment();
    style1.GetVisibilityControl().SetFaces(false).SetEdges(true).SetLights(
        false);
    style2 = modelKey.Subsegment();
    style2.GetLightingAttributeControl().SetInterpolationAlgorithm(
        Lighting::InterpolationAlgorithm::Phong);
    style3 = modelKey.Subsegment();
    style3.GetVisibilityControl().SetEdges(true);
    style3.GetMaterialMappingControl().SetEdgeColor(RGBAColor(1, 0, 0, 1));
    style4 = modelKey.Subsegment();
    style4.GetVisibilityControl().SetEdges(true);
    style4.GetMaterialMappingControl().SetEdgeColor(RGBAColor(0, 0, 1, 1));

    PortfolioKey pKey;
    modelKey.GetPortfolioControl().ShowTop(pKey);
    pKey.DefineNamedStyle("wireframe", style1);
    pKey.DefineNamedStyle("phong", style2);
    pKey.DefineNamedStyle("redEdges", style3);
    pKey.DefineNamedStyle("blueEdges", style4);

    /* This block of code shows how to style a segment conditionally
     * The segment model1 will only use the 'wireframe' style is the condition
     * 'bottom' evaluates to TRUE
     *
     * If more than one condition evaluates to true, than multiple styles are
     * used */
    UTF8 t("top"), b("bottom"), l("left"), r("right");
    model1.GetStyleControl().PushNamed("wireframe", ConditionalExpression(b));
    model1.GetStyleControl().PushNamed("phong", ConditionalExpression(t));
    model1.GetStyleControl().PushNamed("redEdges", ConditionalExpression(r));
    model1.GetStyleControl().PushNamed("blueEdges", ConditionalExpression(l));
    model1.IncludeSegment(
        model2, AND(ConditionalExpression(b), ConditionalExpression(r)));

    /* The next block of code sets multiple conditions on each of the four
     * subwindows and includes the geometry which has the conditional styles set
     * on it */
    UTF8Array c1, c2, c3, c4;
    c1.push_back(t);
    c1.push_back(l);
    c2.push_back(t);
    c2.push_back(r);
    c3.push_back(b);
    c3.push_back(l);
    c4.push_back(b);
    c4.push_back(r);

    topLeft.IncludeSegment(model1);
    topLeft.SetConditions(c1);
    topRight.IncludeSegment(model1);
    topRight.SetConditions(c2);
    bottomLeft.IncludeSegment(model1);
    bottomLeft.SetConditions(c3);
    bottomRight.IncludeSegment(model1);
    bottomRight.SetConditions(c4);

    view.Update();
    CameraKit cameraKit;
    cameraKit.SetPosition(Point(-0.04f, 0.11f, 0.05f))
        .SetTarget(Point(0, 0, 0.01f))
        .SetUpVector(Vector(0, 0, 1))
        .SetField(0.075f, 0.076f)
        .SetProjection(Camera::Projection::Perspective);
    view.GetSegmentKey().SetCamera(cameraKit);
}
