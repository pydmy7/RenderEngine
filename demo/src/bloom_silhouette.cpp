#if defined __APPLE__
#  include "SamplesMacOS.h"
#else
#  include "sample.h"
#endif

#include "sample_common.h"
#include "sprk.h"

using namespace HPS;

void Sample::Bloom_Silhouette(WindowKey wk, Canvas canvas, View view, Model model, SegmentKey modelKey)
{

    /* Sample: Bloom_Silhouette
     * This sample will show you how to:
     * - Load an HSF file through Stream and check for Stream IO exceptions
     * - Change post processing effects
     *
     * Read more about the topics covered in this sample in our Programming Guide
     * - section 7.4:	Bloom
     * - section 7.9:	Perimeter and Silhouette Edges*/

    /* This next block of code shows you how to load an HSF file
     * 1. Create an ImportOptionsKit and set the segment in which you want the file to be loaded
     * 2. Use the Stream::File::Import method to load the desired file, with the options you specified
     * 3. Since this operation happens on a separate thread, wait for it to complete before proceeding
     * 4. Use a try-catch block to capture any Stream IO exceptions */
    Stream::ImportNotifier notifier;
    try {
        Stream::ImportOptionsKit importOptionsKit;
        importOptionsKit.SetSegment(modelKey);
        UTF8 filename = INPUT_FOLDER + "/turbine.hsf";

        notifier = Stream::File::Import(static_cast<char const*>(filename), importOptionsKit);
        notifier.Wait();
    }
    catch (IOException e) {
        // handle Stream IO errors here
    }

    /* This block of code shows you how to change post process effects
     * 1. The PostProcessEffectsControl must be obtained from a WindowKey object
     * 2. Find the method which described the effect you are interested in and toggle it on or off as needed
     * 3. You can set multiple effects in one line using method chaining
     * NOTE - Many post processing effects are only available for shader drivers */
    canvas.GetWindowKey().GetPostProcessEffectsControl().SetBloom(true).SetSilhouetteEdges(true);
    modelKey.GetModellingMatrixControl().Rotate(0, 90, 0);

}
