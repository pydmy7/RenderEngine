#if defined __APPLE__
#  include "SamplesMacOS.h"
#else
#  include "sample.h"
#endif

#include "sample_common.h"
#include "sprk.h"

using namespace HPS;

void Sample::Shell_Attributes(WindowKey wk, Canvas canvas, View view, Model model, SegmentKey modelKey)
{

    /* Sample: Shell_Attributes
     * This sample will show you how to:
     * - Insert a shell
     * - Create a Material Palette
     * - Assign a material at the geometry level
     *
     * Read more about the topics covered in this sample in our Programming Guide
     * - section 2.1:	Shells
     * - section 4.2:	Materials and Material Palettes */

    /* In this block of code some attributes are set at the segment level */
    modelKey.GetVisibilityControl().SetEdges(true);
    modelKey.GetMaterialMappingControl()
        .SetFaceColor(RGBAColor(1, 0, 0, 1))
        .SetEdgeColor(RGBAColor(0, 0, 1, 1))
        .SetVertexColor(RGBAColor(0, 1, 0, 1));

    /* Next, three shells are inserted in the same segment
     * At this point all three shells share the segment attributes set above */
    SegmentKey box = modelKey.Subsegment("boxes");
    ShellKey box1 = CreateBox(box, Point(-1, -1, -1), Point(1, 1, 1));
    ShellKey box2 = CreateBox(box, Point(2, 2, 2), Point(4, 4, 4));
    ShellKey box3 = CreateBox(box, Point(5, 5, 5), Point(7, 7, 7));

    /* This next block of code shows how to define a Material Palette
     * 1. Material Palettes need to be defined in a portfolio, so the first step is to
     *    obtain a PortfolioKey object.
     * 2. Material Palettes, when set on a segment, inherit down the scene graph like
     *    attributes. Therefore it is safe to set the material palette on the modelKey
     *    segment: the box segment will inherit this material palette
     * 3. Three MaterialKit objects are created. In this example each material kit describes
     *    a simple diffuse color, but many different attributes can be set on a single kit
     * 4. A MaterialPalette is defined in a portfolio by supplying a name for the material palette
     *    and an array of MaterialKit objects
     * 5. The material palette is set on the segment.
     *    At this point the materials defined in the myPalette palette can be used on this segment
     * */
    MaterialKitArray materialKit;
    PortfolioKey portfolio;
    modelKey.GetPortfolioControl().ShowTop(portfolio);
    materialKit.push_back(MaterialKit().SetDiffuse(RGBAColor(0, 0, 1, 1)));
    materialKit.push_back(MaterialKit().SetDiffuse(RGBAColor(1, 1, 0, 1)));
    materialKit.push_back(MaterialKit().SetDiffuse(RGBAColor(0, 1, 0, 1)));
    MaterialPaletteDefinition mpd = portfolio.DefineMaterialPalette("myPalette", materialKit);
    modelKey.SetMaterialPalette("myPalette");

    /* This next block of code shows how to set some of the materials defined in the myPalette
     * palette to the shells inserted above.
     * These geometry-level attributes will have precedence over the segment-level attributes set above
     * 1. Create a MaterialMappingKit object, and use one of the ByIndex methods to set attributes on
     *    the shell
     * 2. Since one of the shell does not have a geometry-level attribute set on it, it will use the
     *    segment-level attributes set above instead.
     */
    MaterialMappingKit materialMappingKit;
    materialMappingKit.SetFaceMaterialByIndex(0);
    box2.SetMaterialMapping(materialMappingKit);
    materialMappingKit.SetFaceMaterialByIndex(2);
    box3.SetMaterialMapping(materialMappingKit);

    view.FitWorld();
    view.GetSegmentKey().GetCameraControl().Orbit(35, 25);

}
