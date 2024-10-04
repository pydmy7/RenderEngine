#if defined __APPLE__
#  include "SamplesMacOS.h"
#else
#  include "sample.h"
#endif

#include "sample_common.h"
#include "sprk.h"

using namespace HPS;

void Sample::Hardcopy_Export(WindowKey wk, Canvas canvas, View view, Model model, SegmentKey modelKey)
{
    /* Sample: Hardcopy_Export
     * This sample will show you how to:
     * - Import an HSF file
     * - Retrieve the size of the window at runtime
     * - Export a 2D-PDF and PostScript file
     * - Check whether the Hardcopy Export completed successfully
     *
     * Read more about the topics covered in this sample in our Programming Guide
     * - section x.x:	 */

    SegmentKey description = modelKey.Subsegment();

    HPS::TextKit textKit = HPS::TextKit();
    textKit.SetSize(0.02f, Text::SizeUnits::SubscreenRelative);
    PointArray pointArray(3);
    pointArray[0] = HPS::Point(-0.95f, -0.70f, 10);
    pointArray[1] = HPS::Point(0.95f, -0.70f, 10);
    pointArray[2] = HPS::Point(0, -1, 10);
    textKit.SetRegion(pointArray, Text::RegionAlignment::Center, Text::RegionFitting::Left, false, false, true);
    textKit.SetBackground(true, "rounded box");
    textKit.SetAlignment(HPS::Text::Alignment::BottomLeft);
    HPS::MaterialMappingKit matMapKit = HPS::MaterialMappingKit();
    matMapKit.SetFaceColor(RGBAColor(1, 1, 1));
    matMapKit.SetEdgeColor(RGBAColor(0, 0, 0));

    /* This next block of code shows you how to load an HSF file
     * 1. Create an ImportOptionsKit and set the segment in which you want the file to be loaded
     * 2. Use the Stream::File::Import method to load the desired file, with the options you specified
     * 3. Since this operation happens on a separate thread, wait for it to complete before proceeding
     * 4. Use a try-catch block to capture any Stream IO exceptions */
    Stream::ImportNotifier notifier;
    SegmentKey geometry = modelKey.Subsegment();

    try {
        Stream::ImportOptionsKit importOptionsKit;
        importOptionsKit.SetSegment(geometry);
        UTF8 filename = INPUT_FOLDER + "/conrod.hsf";

        notifier = Stream::File::Import(static_cast<char const*>(filename), importOptionsKit);
        notifier.Wait();
    }
    catch (IOException e) {
        /* catch Stream I/O Errors here */
    }

    view.FitWorld();
    view.Update();

    /* This block of code shows you how to export a 2D-PDF and a Postscript file representing the scene
     * 1. Use a Hardcopy::File::ExportOptionKit object to set the options needed for the export
     * 2. To obtain the size of a window, get its WindowInfoControl object and use its ShowPhysicalSize method
     * 3. Setting WYSIWYG on the export kit matches the model on screen to the hardcopy output exactly
     * 4. The Hardcopy::File:Export method is where the format to export to is specified
     * 5. Hardcopy::File::Export returns a IOResult object which can be checked to see if the
     *		export operation completed successfully. */
    IOResult PDFResult = IOResult::Failure, postScriptResult = IOResult::Failure;
    UTF8 pdfFileOut = OUTPUT_FOLDER + "/hardcopy_export.pdf";
    UTF8 psFileOut = OUTPUT_FOLDER + "/hardcopy_export.ps";
    try {
        Hardcopy::File::ExportOptionsKit exportOptionsKit;
        float width, height;

        wk.GetWindowInfoControl().ShowPhysicalSize(width, height);
        exportOptionsKit.SetWYSIWYG(true);
        exportOptionsKit.SetSize(width, height, Hardcopy::SizeUnits::Centimeters);
        exportOptionsKit.SetResolution(100, Hardcopy::ResolutionUnits::DPCM);

        PDFResult = Hardcopy::File::Export(pdfFileOut, Hardcopy::File::Driver::PDF, wk, exportOptionsKit);
        postScriptResult = Hardcopy::File::Export(psFileOut, Hardcopy::File::Driver::Postscript, wk, exportOptionsKit);
    }
    catch (IOException) {
        // handle Hardcopy exception
    }

    /*Here the Material Kit defined previously is attached to the description subsegment
     *Edges are set to visible
     *The matMapKit is attached to the description subsegment to ensure it's style does not affect the model Key Material Mapping
     *Style*/

    description.GetVisibilityControl().SetEdges(true);
    description.SetMaterialMapping(matMapKit);

    /* This block of code checks the result of the two export operations completed above
     * and prints out a message on the screen informing the user whether they completed
     * successfully or not*/

    UTF8 resultDescription;
    if (PDFResult == IOResult::Success)
        resultDescription = "PDF Export completed successfully.";
    else
        resultDescription = "PDF Export FAILED.";

    if (postScriptResult == IOResult::Success)
        resultDescription += "\nPostScript Export completed successfully.";
    else
        resultDescription += "\nPostScript Export FAILED.";

    resultDescription += "\nYou can find the exported files in this folder:\n   " + OUTPUT_FOLDER;

    textKit.SetText(resultDescription);

    description.InsertText(textKit);
}
