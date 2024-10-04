#if defined __APPLE__
#  include "SamplesMacOS.h"
#else
#  include "sample.h"
#endif

#include "sample_common.h"
#include "sprk.h"

using namespace HPS;

void Sample::Quickmoves(WindowKey wk, Canvas canvas, View view, Model model, SegmentKey modelKey)
{

    /* Sample: Quickmoves
     * This sample will show you how to:
     * - Load an HSF file through Stream and check for Stream IO exceptions
     * - Include a segment into other segments
     * - Set different overlay settings
     *
     * Read more about the topics covered in this sample in our Programming Guide
     * - section 6.3.5:	Overlays*/

    /* This next block of code shows you how to load an HSF file
     * 1. Create an ImportOptionsKit and set the segment in which you want the file to be loaded
     * 2. Use the Stream::File::Import method to load the desired file, with the options you specified
     * 3. Since this operation happens on a separate thread, wait for it to complete before proceeding
     * 4. Use a try-catch block to capture any Stream IO exceptions
     * 5. Root segments are special segments, which can only be created from the Database. Since this root segment
     *		is not associated with a window, its contents will not be displayed on the screen. This segment
     *		will be used to store geometry which can later be included in segments which are under a window */
    SegmentKey includeSegment = Database::CreateRootSegment();
    SegmentKey window1 = modelKey.Subsegment();
    SegmentKey window2 = modelKey.Subsegment();
    SegmentKey window3 = modelKey.Subsegment();

    modelKey.GetTextAttributeControl().SetAlignment(Text::Alignment::Center);

    try {
        UTF8 file_in = INPUT_FOLDER + "/turbine.hsf";
        Stream::ImportOptionsKit importOptionsKit;
        importOptionsKit.SetSegment(includeSegment);
        Stream::ImportNotifier notifier = Stream::File::Import(static_cast<char const*>(file_in), importOptionsKit);
        notifier.Wait();
    }
    catch (IOException e) {
        /* handle Stream IO errors here */
    }

    /* This block of code shows how to include a segment into other segments and how to create subwindows
     * 1. To include a segment into another segment use the following method:
     *		destinationSegmentKey.IncludeSegment(sourceSegmentKey)
     *		including segments is a good way to add the same geometry in multiple places without
     *		needlessly creating multiple copies of the geometry in question
     * 2. To create a subwindow use the following method:
     *		segmentKey.SetSubwindow(HPS::Rectangle(left, right, bottom, top)
     *		the values of the arguments in the Rectangle constructors should be between -1 and 1
     *		with the top left corner representing the point (-1, 1) */
    SubwindowKit subwindow;
    window1.IncludeSegment(includeSegment);
    subwindow.SetSubwindow(HPS::Rectangle(-1, -0.33f, -1, 1));
    window1.SetSubwindow(subwindow);
    window1.InsertText(Point(0, -0.8f, 0), "No Overlay");
    window1.GetTextAttributeControl().SetTransform(Text::Transform::NonTransformable);

    window2.IncludeSegment(includeSegment);
    subwindow.SetSubwindow(HPS::Rectangle(-0.33f, 0.33f, -1, 1));
    window2.SetSubwindow(subwindow);
    window2.InsertText(Point(0, -0.8f, 0), "Normal Overlay");
    window2.GetTextAttributeControl().SetTransform(Text::Transform::NonTransformable);

    window3.IncludeSegment(includeSegment);
    subwindow.SetSubwindow(HPS::Rectangle(0.33f, 1, -1, 1));
    window3.SetSubwindow(subwindow);
    window3.InsertText(Point(0, -0.8f, 0), "Overlay with Z values");
    window3.GetTextAttributeControl().SetTransform(Text::Transform::NonTransformable);

    view.FitWorld();
    view.GetSegmentKey().GetModellingMatrixControl().Scale(0.9f, 0.9f, 0.9f).Rotate(0, 35, 0);

    SegmentKeyArray locators;
    locators.push_back(InsertCross(window1));
    locators.push_back(InsertCross(window2));
    locators.push_back(InsertCross(window3));

    /* This block of code shows how to set different overlays settings on a segments
     * The code will then execute a series of update on a subwindow at the time, allowing you
     * to see the performance difference between segments with the same geometry but difference
     * overlay settings */
    locators[0].GetDrawingAttributeControl().SetOverlay(Drawing::Overlay::None);
    locators[1].GetDrawingAttributeControl().SetOverlay(Drawing::Overlay::Default);
    locators[2].GetDrawingAttributeControl().SetOverlay(Drawing::Overlay::WithZValues);

    for (int i = 0; i < 20; i++) {
        locators[0].GetModellingMatrixControl().Rotate(0, 1, 0);
        view.Update();
    }

    for (int i = 0; i < 20; i++) {
        locators[1].GetModellingMatrixControl().Rotate(0, 1, 0);
        view.Update();
    }

    for (int i = 0; i < 20; i++) {
        locators[2].GetModellingMatrixControl().Rotate(0, 1, 0);
        view.Update();
    }

}
