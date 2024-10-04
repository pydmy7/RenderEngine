#if defined __APPLE__
#include "SamplesMacOS.h"
#else
#include "sample.h"
#endif

#include "sample_common.h"
#include "sprk.h"
#include "sprk_ops.h"

using namespace HPS;

void Sample::Highlighting(WindowKey wk, Canvas canvas, View view, Model model,
                          SegmentKey modelKey) {
    /* Sample: Highlight styles
     * This sample will show you how to:
     * - Load an HSF file
     * - Load an image and use it to create a texture
     * - Create an opaque style
     * - Create a transparent style
     * - Create a textured style
     *
     * Read more about the topics covered in this sample in our Programming
     * Guide
     * - section 6.4:	Highlighting
     * - section 4.3:    Defining and using styles
     */

    // Setup four sub windows
    modelKey.GetSubwindowControl().SetBackground(
        Subwindow::Background::Transparent);

    SegmentKey one = modelKey.Subsegment();
    one.GetSubwindowControl().SetSubwindow(HPS::Rectangle(-1, 0, 0, 1));
    one.InsertDistantLight(Vector(0.5f, 0.5f, 0));
    SegmentKey two = modelKey.Subsegment();
    two.GetSubwindowControl().SetSubwindow(HPS::Rectangle(0, 1, 0, 1));
    two.InsertDistantLight(Vector(0.5f, 0.5f, 0));
    SegmentKey three = modelKey.Subsegment();
    three.GetSubwindowControl().SetSubwindow(HPS::Rectangle(-1, 0, -1, 0));
    three.InsertDistantLight(Vector(0.5f, 0.5f, 0));
    SegmentKey four = modelKey.Subsegment();
    four.GetSubwindowControl().SetSubwindow(HPS::Rectangle(0, 1, -1, 0));
    four.InsertDistantLight(Vector(0.5f, 0.5f, 0));

    ImageKit imageKit;
    SegmentKey data_segment = Database::CreateRootSegment();

    try {
        /* This next block of code shows you how to load an HSF file
         * 1. Create an ImportOptionsKit and set the segment in which you want
         * the file to be loaded
         * 2. Use the Stream::File::Import method to load the desired file with
         * the options you specified
         * 3. Since this operation happens on a separate thread, wait for it to
         * complete before proceeding
         * 4. Use a try-catch block to capture any Stream IO exceptions */
        Stream::ImportOptionsKit importOptionsKit;
        importOptionsKit.SetSegment(data_segment);
        UTF8 filename = INPUT_FOLDER + "/bnc.hsf";

        Stream::ImportNotifier notifier = Stream::File::Import(
            static_cast<char const*>(filename), importOptionsKit);
        notifier.Wait();

        /* This block of code shows you how to load an image from a file
         * 1. Create an ImportOptionsKit and set the image format
         * 2. Use the Image::File::Import method to load the desired file with
         * the options you specified
         * 3. Use a try-catch block to capture any IO exceptions */
        filename = INPUT_FOLDER + "/pattern.jpg";
        Image::ImportOptionsKit iok;
        iok.SetFormat(Image::Format::Jpeg);
        imageKit = Image::File::Import(filename, iok);
    } catch (IOException e) {
        // handle IO errors here
        e.what();
    }

    // Include the loaded model into the four sub windows
    IncludeKey one_include = one.IncludeSegment(data_segment);
    IncludeKey two_include = two.IncludeSegment(data_segment);
    IncludeKey three_include = three.IncludeSegment(data_segment);
    IncludeKey four_include = four.IncludeSegment(data_segment);

    // create a portfolio to store styles and the texture
    PortfolioKey myPortfolio = Database::CreatePortfolio();

    // create an ImageDefinition from the image loaded earlier. an image
    // definition is required to create a TextureDefinition
    ImageDefinition imageDefinition =
        myPortfolio.DefineImage("my_image", imageKit);
    TextureOptionsKit textureOptionsKit;

    // see the table below about parameterization sources
    textureOptionsKit.SetParameterizationSource(
        Material::Texture::Parameterization::UV);

    // makes the image into a texture
    myPortfolio.DefineTexture("my_texture", imageDefinition, textureOptionsKit);

    modelKey.GetModellingMatrixControl()
        .Rotate(-90, 30, 0)
        .Scale(0.6f, 0.6f, 0.6f);

    // create the opaque red style
    SegmentKey redStyleSegment = Database::CreateRootSegment();
    NamedStyleDefinition red_style =
        myPortfolio.DefineNamedStyle("red_style", redStyleSegment);
    redStyleSegment.GetMaterialMappingControl().SetFaceColor(
        RGBAColor(1.0f, 0, 0));

    // create the transparent style. it is a 1/2 transparent red style
    SegmentKey transparentStyleSegment = Database::CreateRootSegment();
    NamedStyleDefinition transparent_style = myPortfolio.DefineNamedStyle(
        "transparent_style", transparentStyleSegment);
    transparentStyleSegment.GetMaterialMappingControl().SetFaceColor(
        RGBAColor(1.0f, 0, 0, 0.5f));

    // create the textured style based on the image we loaded earlier
    SegmentKey texturedStyleKey = Database::CreateRootSegment();
    NamedStyleDefinition textured_style =
        myPortfolio.DefineNamedStyle("textured_style", texturedStyleKey);
    textured_style.GetSource().GetMaterialMappingControl().SetFaceTexture(
        "my_texture");

    HighlightOptionsKit hok1("red_style");
    HighlightOptionsKit hok2("transparent_style");
    HighlightOptionsKit hok3("textured_style");

    // make the portfolio active
    modelKey.GetPortfolioControl().Push(myPortfolio);

    view.FitWorld();

    // Setup a legend for the first subwindow
    SegmentKey one_legend = one.Subsegment();
    CameraKit const legend_camera = CameraKit::GetDefault();
    one_legend.SetCamera(legend_camera);
    TextKit textKit;
    textKit.SetText("Unstyled model")
        .SetPosition(Point(0, 0, -1.5f))
        .SetAlignment(HPS::Text::Alignment::Center);
    one_legend.InsertText(textKit);
    one_legend.GetDrawingAttributeControl().SetDepthRange(
        0, 0.1f);  // bring to front

    // Setup a legend for the second subwindow
    SegmentKey two_legend = two.Subsegment();
    two_legend.SetCamera(legend_camera);
    textKit.SetText("Styled with opaque red style");
    two_legend.InsertText(textKit);
    two_legend.GetDrawingAttributeControl().SetDepthRange(
        0, 0.1f);  // bring to front

    // Obtain a key path to the second subwindow, and highlight it using the red
    // style setup earlier
    SprocketPath sprocket_path(canvas, canvas.GetAttachedLayout(), view, model);
    KeyPath key_path = sprocket_path.GetKeyPath()
                           .PushFront(two_include)
                           .PushFront(data_segment);

    hok1.SetOverlay(HPS::Drawing::Overlay::WithZValues);
    canvas.GetWindowKey().GetHighlightControl().Highlight(key_path, hok1);

    // Setup a legend for the third subwindow
    SegmentKey three_legend = three.Subsegment();
    three_legend.SetCamera(legend_camera);
    textKit.SetText("Styled with transparent red style");
    three_legend.InsertText(textKit);
    three_legend.GetDrawingAttributeControl().SetDepthRange(
        0, 0.1f);  // bring to front

    key_path = sprocket_path.GetKeyPath()
                   .PushFront(three_include)
                   .PushFront(data_segment);

    // Overlay "InPlace" is necessary for transparent styles
    hok2.SetOverlay(HPS::Drawing::Overlay::InPlace);
    canvas.GetWindowKey().GetHighlightControl().Highlight(key_path, hok2);

    // Setup a legend for the fourth subwindow
    SegmentKey four_legend = four.Subsegment();
    four_legend.SetCamera(legend_camera);
    textKit.SetText("Styled with textured style");
    four_legend.InsertText(textKit);
    four_legend.GetDrawingAttributeControl().SetDepthRange(
        0, 0.1f);  // bring to front
    key_path = sprocket_path.GetKeyPath()
                   .PushFront(four_include)
                   .PushFront(data_segment);

    hok3.SetOverlay(HPS::Drawing::Overlay::WithZValues);
    canvas.GetWindowKey().GetHighlightControl().Highlight(key_path, hok3);

    canvas.Update();
}
