#if defined __APPLE__
#include "SamplesMacOS.h"
#else
#include "sample.h"
#endif

#include "sample_common.h"
#include "sprk.h"

using namespace HPS;

void Sample::Reference_Geometry(WindowKey wk, Canvas canvas, View view,
                                Model model, SegmentKey modelKey) {
    // We create a root segment that becomes the include library. It is often
    // useful but not necessary to organize your instanced geometry into a
    // library.
    SegmentKey rootSegment = Database::CreateRootSegment();
    SegmentKey shellSegment =
        rootSegment.Subsegment("include library").Subsegment("shell");

    // Here we are defining the shell that will be instanced. This will be a
    // pyramid.
    Point pointArray[] = {HPS::Point(0, 0, 0), HPS::Point(1, 0, 0),
                          HPS::Point(1, 0, 1), HPS::Point(0, 0, 1),
                          HPS::Point(0.5, 0.5, 0.75)};

    int faceList[] = {4, 0, 1, 2, 3, 3, 4, 0, 1, 3, 4,
                      1, 2, 3, 4, 2, 3, 3, 4, 3, 0};

    ShellKit shellKit;
    shellKit.SetPoints(5, pointArray);
    shellKit.SetFacelist(21, faceList);

    // Insert the shell that was defined above.
    ShellKey myShellKey = shellSegment.InsertShell(shellKit);

    // Reference the shell as geometry instances and translate each instance
    // accordingly. In this example we are building a 3D array of pyramids -
    // 5x5x5.
    int MAX_SHELLS_PER_ROW = 5;
    int MAX_SHELLS_PER_COLUMN = 5;
    int MAX_PAGE = 5;

    model.GetSegmentKey().GetVisibilityControl().SetGenericEdges(true);
    SegmentKey refSegment =
        model.GetSegmentKey().Subsegment("reference geometries");

    for (int page = 0; page < MAX_PAGE; page++) {
        for (int columns = 0; columns < MAX_SHELLS_PER_COLUMN; columns++) {
            for (int rows = 0; rows < MAX_SHELLS_PER_ROW; rows++) {
                HPS::MatrixKit myKit;

                // Apply translation
                myKit.Translate((float)rows, (float)columns, (float)page);
                ReferenceKey refKey = refSegment.ReferenceGeometry(myShellKey);
                refKey.SetModellingMatrix(myKit);
            }
        }
    }

    view.FitWorld();
    view.GetSegmentKey().GetCameraControl().Orbit(45, 30);
    canvas.UpdateWithNotifier().Wait();

    view.FitWorld().Update();
}
