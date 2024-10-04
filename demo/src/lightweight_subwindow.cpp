#if defined __APPLE__
#  include "SamplesMacOS.h"
#else
#  include "sample.h"
#endif

#include "sample_common.h"
#include "sprk.h"

#include <iostream>
#include <sstream>

using namespace HPS;

void Sample::Lightweight_Subwindow(WindowKey wk, Canvas canvas, View view, Model model, SegmentKey modelKey)
{

    /* Sample: Lightweight_Subwindow
     * This sample will show you how to:
     * - Load an HSF file through Stream and check for Stream IO exceptions
     * - Create a legend with a dedicated camera
     * - Use a lightweight sub-window to keeps geometry on top of the scene
     *
     * Read more about the topics covered in this sample in our Programming Guide */

    SegmentKey driveShaft = modelKey.Subsegment();
    SegmentKey legendBar = modelKey.Subsegment();

    /* This next block of code shows you how to load an HSF file
     * 1. Create an ImportOptionsKit and set the segment in which you want the file to be loaded
     * 2. Use the Stream::File::Import method to load the desired file, with the options you specified
     * 3. Since this operation happens on a separate thread, wait for it to complete before proceeding
     * 4. Use a try-catch block to capture any Stream IO exceptions */
    Stream::ImportNotifier notifier;
    try {
        Stream::ImportOptionsKit importOptionsKit;
        importOptionsKit.SetSegment(driveShaft);
        UTF8 filename = INPUT_FOLDER + "/driveshaft.hsf";

        notifier = Stream::File::Import(static_cast<char const*>(filename), importOptionsKit);
        notifier.Wait();

        importOptionsKit.SetSegment(legendBar);
        filename = INPUT_FOLDER + "/legend.hsf";

        notifier = Stream::File::Import(static_cast<char const*>(filename), importOptionsKit);
        notifier.Wait();
    }
    catch (IOException e) {
        /* catch Stream I/O Errors here */
        throw;
    }

    /* The next block of code shows you how to set up a new Camera from a CameraKit object
     * 1. Set up the camera position, target, up vector, field and projection attributes
     * 2. Apply the camera kit to the segment where the camera needs to be inserted */
    CameraKit camera;
    camera.SetPosition(Point(0.35f, 0.48f, 0.23f))
        .SetTarget(Point(0, 0, 0))
        .SetUpVector(Vector(-0.63f, 0.64f, -0.43f))
        .SetField(0.18f, 0.18f)
        .SetProjection(Camera::Projection::Orthographic);
    driveShaft.SetCamera(camera);

    camera.SetPosition(Point(0, 0, 1))
        .SetTarget(Point(0.3f, 0.3f, 0))
        .SetUpVector(Vector(0, 1, 0))
        .SetField(0.4f, 0.4f)
        .SetProjection(Camera::Projection::Orthographic);
    legendBar.SetCamera(camera);

    /* To make sure that the legend will be displayed on top of the geometry, it is placed in a
     * lightweight window. */
    legendBar.GetSubwindowControl().SetSubwindow(HPS::Rectangle(0, 1, -1, 1), Subwindow::Type::Lightweight);
    legendBar.GetTextAttributeControl()
        .SetSize(0.015f, HPS::Text::SizeUnits::SubscreenRelative)
        .SetAlignment(Text::Alignment::BottomLeft);

}
