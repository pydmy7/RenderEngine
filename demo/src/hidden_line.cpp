#if defined __APPLE__
#  include "SamplesMacOS.h"
#else
#  include "sample.h"
#endif

#include "sample_common.h"
#include "sprk.h"

using namespace HPS;

void Sample::Hidden_Line(WindowKey wk, Canvas canvas, View view, Model model, SegmentKey modelKey)
{

    /* Sample: Hidden_Line
     * This sample will show you how to:
     * - Load an HSF file through Stream and check for Stream IO exceptions
     * - Change visibility at the segment level
     * - Change the rendering algorithm to HiddenLine
     * - Tweak Hidden Line options
     *
     * Read more about the topics covered in this sample in our Programming Guide
     * - section 7.6:	Hidden Surface Removal
     * - section 7.7:	Hidden Line Removal */

    /* This next block of code shows you how to load an HSF file
     * 1. Create an ImportOptionsKit and set the segment in which you want the file to be loaded
     * 2. Use the Stream::File::Import method to load the desired file, with the options you specified
     * 3. Since this operation happens on a separate thread, wait for it to complete before proceeding
     * 4. Use a try-catch block to capture any Stream IO exceptions */
    Stream::ImportNotifier notifier;
    try {
        Stream::ImportOptionsKit importOptionsKit;
        importOptionsKit.SetSegment(modelKey);
        UTF8 filename = INPUT_FOLDER + "/suspension.hsf";

        notifier = Stream::File::Import(static_cast<char const*>(filename), importOptionsKit);
        notifier.Wait();
    }
    catch (IOException e) {
        // handle Stream IO errors here
        throw;
    }

    /* This block shows you how to to set the Hidden Line rendering algorithm
     * 1. Change the visibility for the segment as needed through a VisibilityControl
     * 2. Set the rendering algorithm to HiddenLine from the SubwindowControl
     * 3. Tweak Hidden Line attributes as needed from the HiddenLineAttributeControl object*/
    modelKey.GetVisibilityControl().SetFaces(true).SetEdges(true).SetLines(true);
    modelKey.GetSubwindowControl().SetRenderingAlgorithm(Subwindow::RenderingAlgorithm::HiddenLine);
    modelKey.GetHiddenLineAttributeControl()
        .SetAlgorithm(HiddenLine::Algorithm::ZBuffer)
        .SetVisibility(false)
        .SetSilhouetteCleanup(true)
        .SetRenderFaces(false)
        .SetRenderText(false)
        .SetLinePattern("-  -");

    view.FitWorld();
    modelKey.GetModellingMatrixControl().Rotate(0, 90, 0);

}
