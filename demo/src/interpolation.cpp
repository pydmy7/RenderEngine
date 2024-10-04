#if defined __APPLE__
#include "SamplesMacOS.h"
#else
#include "sample.h"
#endif

#include "sample_common.h"
#include "sprk.h"

using namespace HPS;

void Sample::Interpolation(WindowKey wk, Canvas canvas, View view, Model model,
                           SegmentKey modelKey) {
    /* Sample: Interpolation
     * This sample will show you how to:
     * - Create a material palette
     * - Set colors on vertices using an RGBColor
     * - Set colors on vertices using a material palette
     * - Turning Color Interpolation ON and OFF
     *
     * Read more about the topics covered in this sample in our Programming
     * Guide
     * - section 4.3:	Color
     * - section 4.2:	Materials and Material Palettes */

    modelKey.GetMaterialMappingControl().SetFaceColor(RGBAColor(0, 0, 1, 1));

    int const numberOfSides = 36;
    int const numberOfVertices = 2 * (numberOfSides + 1);

    HPS::FloatArray radii;
    radii.push_back(0.5f);
    radii.push_back(1);
    radii.push_back(0.5f);
    radii.push_back(0.8f);

    HPS::FloatArray yValue;
    yValue.push_back(0.5f);
    yValue.push_back(0);

    int vertex;
    HPS::PointArray meshPoints(numberOfVertices);
    float angle;

    for (vertex = 0; vertex < numberOfVertices; ++vertex) {
        angle = (float)(vertex / 2.0f) / (float)numberOfSides *
                (2 * (float)HPS::PI);
        meshPoints[vertex].x = radii[vertex % 4] * cos(angle);
        meshPoints[vertex].y = yValue[vertex % 2];
        meshPoints[vertex].z = radii[vertex % 4] * sin(angle);
    }

    HPS::SegmentKey geometry = modelKey.Subsegment();
    geometry.GetVisibilityControl().SetEverything(false);
    MeshKey mesh = geometry.InsertMesh(numberOfSides + 1, 2, meshPoints);
    for (vertex = 0; vertex < numberOfVertices; ++vertex) {
        mesh.SetVertexIndexColorsByRange(vertex, 1, (float)(vertex % 2) * 3);
    }

    TextKit textKit;
    textKit.SetSize(0.02f, Text::SizeUnits::WindowRelative);
    textKit.SetPosition(Point(0, -0.2f, 0));
    textKit.SetAlignment(Text::Alignment::Center);

    /* This next block of code shows how to define a Material Palette
     * 1. Material Palettes need to be defined in a portfolio, so the first step
     * is to obtain a PortfolioKey object.
     * 2. Material Palettes, when set on a segment, inherit down the scene graph
     * like attributes. Therefore it is safe to set the material palette on the
     * modelKey segment: the box segment will inherit this material palette
     * 3. Three MaterialKit objects are created. In this example each material
     * kit describes a simple diffuse color, but many different attributes can
     * be set on a single kit
     * 4. A MaterialPalette is defined in a portfolio by supplying a name for
     * the material palette and an array of MaterialKit objects
     * 5. The material palette is set on the segment.
     *    At this point the materials defined in the myPalette palette can be
     * used on this segment
     * */
    MaterialKitArray materialKit;
    PortfolioKey portfolio;
    modelKey.GetPortfolioControl().ShowTop(portfolio);
    materialKit.push_back(MaterialKit().SetDiffuse(RGBAColor(1, 0, 0, 1)));
    materialKit.push_back(MaterialKit().SetDiffuse(RGBAColor(0, 1, 0, 1)));
    materialKit.push_back(MaterialKit().SetDiffuse(RGBAColor(0, 0, 1, 1)));
    materialKit.push_back(MaterialKit().SetDiffuse(RGBAColor(1, 1, 0, 1)));
    MaterialPaletteDefinition mpd =
        portfolio.DefineMaterialPalette("myPalette", materialKit);
    modelKey.SetMaterialPalette("myPalette");

    /* This block of code shows how to color a shell using color interpolation
     * 1. Insert the shell
     * 2. Set a color on its vertices, it is possible to set the same color on
     * many vertices at once
     * 3. Turn color interpolation ON for faces on using a
     * ColorInterpolationControl */
    SegmentKey s1 = modelKey.Subsegment();
    s1.GetSubwindowControl().SetSubwindow(HPS::Rectangle(-1, 0, 0, 1),
                                          Subwindow::Type::Lightweight);
    s1.ReferenceGeometry(mesh);
    s1.GetColorInterpolationControl().SetFaceColor(true).SetFaceIndex(false);
    textKit.SetText("Color \nInterpolation");
    s1.InsertText(textKit);

    /* This block of code shows how to color a shell without using color
     * interpolation
     * 1. Insert the shell
     * 2. Set a color on its vertices, it is possible to set the same color on
     * many vertices at once
     * 3. Turn color interpolation OFF for faces on using a
     * ColorInterpolationControl Since color interpolation is off, the face
     * color declared above will be used instead of the vertices colors for
     * faces */
    SegmentKey s2 = modelKey.Subsegment();
    s2.GetSubwindowControl().SetSubwindow(HPS::Rectangle(0, 1, 0, 1),
                                          Subwindow::Type::Lightweight);
    s2.ReferenceGeometry(mesh);
    s2.GetColorInterpolationControl().SetFaceColor(false).SetFaceIndex(false);
    textKit.SetText("No Color \nInterpolation");
    s2.InsertText(textKit);

    /* This block of code shows how to color a shell by index
     * 1. Insert the shell
     * 2. Set a color on its vertices by index. The indices used refer to the
     * MaterialKits in the MaterialPalette which is currently active on this
     * segment. The MaterialPalette has been defined and set as active above.
     * 3. Turn color interpolation ON for faces on using a
     * ColorInterpolationControl */
    SegmentKey s3 = modelKey.Subsegment();
    s3.GetSubwindowControl().SetSubwindow(HPS::Rectangle(-1, 0, -1, 0),
                                          Subwindow::Type::Lightweight);
    s3.ReferenceGeometry(mesh);
    s3.GetColorInterpolationControl().SetFaceColor(true).SetFaceIndex(true);
    textKit.SetText("Color Interpolation & \nColor Index Interpolation");
    s3.InsertText(textKit);

    /* This block of code shows how to color a shell by index, without using
     * color interpolation
     * 1. Insert the shell
     * 2. Set a color on its vertices by index. The indices used refer to the
     * MaterialKits in the MaterialPalette which is currently active on this
     * segment. The MaterialPalette has been defined and set as active above.
     * 3. Turn color interpolation OFF for faces on using a
     * ColorInterpolationControl */
    SegmentKey s4 = modelKey.Subsegment();
    s4.GetSubwindowControl().SetSubwindow(HPS::Rectangle(0, 1, -1, 0),
                                          Subwindow::Type::Lightweight);
    s4.ReferenceGeometry(mesh);
    s4.GetColorInterpolationControl().SetFaceColor(false).SetFaceIndex(true);
    textKit.SetText("No Color Interpolation & \nColor Index Interpolation");
    s4.InsertText(textKit);

    view.FitWorld();
}
