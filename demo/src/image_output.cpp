#if defined __APPLE__
#include "SamplesMacOS.h"
#else
#include "sample.h"
#endif

#include "sample_common.h"
#include "sprk.h"

#include <fstream>

using namespace HPS;

void Sample::Image_Output(WindowKey wk, Canvas canvas, View view, Model model,
                          SegmentKey modelKey) {
    /* Sample: Image_Output
     * This sample will show you how to:
     * - Load an HSF file through Stream and check for Stream IO exceptions
     * - Create an Offscreen Window
     * - Render to an offscreen window
     * - Save a snapshot of the screen to a file
     *
     * Read more about the topics covered in this sample in our Programming
     * Guide
     * - section 6.2:	Offscreen Rendering */

    Stream::ImportNotifier streamNotifier;
    SegmentKey geometry = modelKey.Subsegment();

    /* This next block of code shows you how to load an HSF file
     * 1. Create an ImportOptionsKit and set the segment in which you want the
     * file to be loaded
     * 2. Use the Stream::File::Import method to load the desired file, with the
     * options you specified
     * 3. Since this operation happens on a separate thread, wait for it to
     * complete before proceeding
     * 4. Use a try-catch block to capture any Stream IO exceptions */
    try {
        Stream::ImportOptionsKit importOptionsKit;
        importOptionsKit.SetSegment(geometry);
        UTF8 filename = INPUT_FOLDER + "/conrod.hsf";

        streamNotifier = Stream::File::Import(
            static_cast<char const*>(filename), importOptionsKit);
        streamNotifier.Wait();
    } catch (IOException e) {
        /* catch Stream I/O Errors here */
    }

    geometry.GetModellingMatrixControl().Rotate(-90, 0, 0);

    /* This block of code shows how to create an off-screen window and how to
     *render to it
     * 1. Create an OffScreenWindowOptionsKit object, and set all the options
     *related to the off-screen window such as the driver to use, or whether it
     *will use anti-aliasing
     * 2. To have an off-screen window with the same dimensions of the current
     *on-screen window the size in pixels of the on-screen window can be
     *obtained from the WindowInfoControl
     * 3. Crate an OffscreenWindow from the Database. Pass the width, height and
     *options to the constructor
     * 4. Set a camera, visibility and any other attribute as necessary on the
     *off-screen window. In order to obtain an exact snapshot of the on-screen
     *window, these attributes should be the same between on- and off-screen
     *windows
     * 5. Include a segment containing geometry into the off-screen window. Only
     *geometry which is either in the included segment or under it in the scene
     *graph will be rendered to the off-screen window
     * 6. Update the off-screen window to start rendering to it. Since this
     *operation occurs on a separate thread, it is possible to wait for it to
     *complete by using the UpdateNotifier::Wait() method */
    CameraKit cameraKit;
    VisibilityKit visibilityKit;
    view.FitWorld();
    view.GetSegmentKey().ShowCamera(cameraKit);
    view.GetSegmentKey().ShowVisibility(visibilityKit);

    OffScreenWindowOptionsKit oswok;
    oswok.SetDriver(Window::Driver::OpenGL2).SetAntiAliasCapable(true);

    unsigned int width, height;
    canvas.GetWindowKey().GetWindowInfoControl().ShowPhysicalPixels(width,
                                                                    height);
    OffScreenWindowKey offscreenWindowKey =
        Database::CreateOffScreenWindow(width, height, oswok);
    offscreenWindowKey.SetCamera(cameraKit);
    offscreenWindowKey.SetVisibility(visibilityKit);
    offscreenWindowKey.IncludeSegment(modelKey);
    UpdateNotifier updateNotifier = offscreenWindowKey.UpdateWithNotifier();
    updateNotifier.Wait();

    /* This block of code shows how to output the image rendered to the
     * off-screen window to a file
     * 1. Obtain an imageKit from the off-screen window's WindowOptionControl
     * 2. Obtain the image data as an array of bytes using the
     * ImageKit::ShowData() method
     * 3. Output the data to a file in binary format */
    ImageKit imageKit;
    ByteArray imageData;
    std::ofstream file;
    bool exportSuccessful = true;
    UTF8 file_out = OUTPUT_FOLDER + "/image_output.jpeg";

    HPS::SegmentKey results_segment = modelKey.Subsegment();

    HPS::TextKit textKit;
    textKit.SetSize(0.02f, Text::SizeUnits::SubscreenRelative);
    PointArray pointArray(3);
    pointArray[0] = HPS::Point(-0.95f, -0.65f, 10);
    pointArray[1] = HPS::Point(0.95f, -0.65f, 10);
    pointArray[2] = HPS::Point(0, -1, 10);
    textKit.SetRegion(pointArray, Text::RegionAlignment::Center,
                      Text::RegionFitting::Left, false, false, true);
    textKit.SetBackground(true, "rounded box");
    textKit.SetAlignment(HPS::Text::Alignment::BottomLeft);
    results_segment.GetMaterialMappingControl().SetFaceColor(
        RGBAColor(1, 1, 1));
    results_segment.GetMaterialMappingControl().SetEdgeColor(
        RGBAColor(0, 0, 0));
    results_segment.GetVisibilityControl().SetEdges(true);

    offscreenWindowKey.GetWindowOptionsControl().ShowImage(Image::Format::Jpeg,
                                                           imageKit);
    imageKit.ShowData(imageData);

    file.open(static_cast<char const*>(file_out),
              std::ios::out | std::ios::binary);
    if (file.fail())
        exportSuccessful = false;
    else {
        size_t size = imageData.size();
        for (size_t i = 0; i < size; i++) {
            file << imageData.at(i);
        }
    }

    if (file.fail()) exportSuccessful = false;

    file.close();
    if (file.fail()) exportSuccessful = false;

    if (!exportSuccessful) {
        UTF8 error_message;
        error_message = "Image Export Unsuccessful";
        modelKey.InsertText(Point(0, 0, 0),
                            static_cast<char const*>(error_message));
        view.FitWorld();
        view.Update();
    } else {
        UTF8 success_message = "Image Export completed successfully.\n"
                               "You can find the JPEG image here:\n   " +
                               file_out;
        textKit.SetText(success_message);
        results_segment.InsertText(textKit);
        view.Update();
    }
}
