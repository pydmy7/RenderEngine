#if defined __APPLE__
#include "SamplesMacOS.h"
#else
#include "sample.h"
#endif

#include "sample_common.h"
#include "sprk.h"

#include <iostream>
#include <sstream>

using namespace HPS;

void Sample::Database_Search(WindowKey wk, Canvas canvas, View view,
                             Model model, SegmentKey modelKey) {
    /* Sample: Database_Search
     * This sample will show you how to:
     * - Load an HSF file through Stream and check for Stream IO exceptions
     * - Search the database for segments, shells and vertices
     * - Obtain a SearchResultIterator object, and how to walk a search result
     * list with it
     * - Inserting text using UTF8/string objects
     * - Setting up a camera through a CameraKit
     *
     * Read more about the topics covered in this sample in our Programming
     * Guide
     * - section 1.2:	HPS Scene Graph and the Structure of the Database */

    /* This next block of code shows you how to load an HSF file
     * 1. Create an ImportOptionsKit and set the segment in which you want the
     * file to be loaded
     * 2. Use the Stream::File::Import method to load the desired file, with the
     * options you specified
     * 3. Since this operation happens on a separate thread, wait for it to
     * complete before proceeding
     * 4. Use a try-catch block to capture any Stream IO exceptions */
    Stream::ImportNotifier notifier;
    SegmentKey geometry = modelKey.Subsegment();
    geometry.GetTextAttributeControl().SetAlignment(Text::Alignment::Center);

    try {
        Stream::ImportOptionsKit importOptionsKit;
        importOptionsKit.SetSegment(geometry);
        UTF8 filename = INPUT_FOLDER + "/bnc.hsf";

        notifier = Stream::File::Import(static_cast<char const*>(filename),
                                        importOptionsKit);
        notifier.Wait();
    } catch (IOException e) {
        /* catch Stream I/O Errors here */
    }

    geometry.GetModellingMatrixControl().Rotate(-90, 0, 0);

    /* The next block shows you how to search the database
     * 1. Use the Find method to search the database
     * 2. The first parameter of Find specifies what to search for
     * 3. The second parameter of Find specifies where to search
     * 4. The third parameter of Find returns contains the results of the
     * search, and is returned to the user
     * 5. The return value of Find indicates how many geometries matched our
     * search criteria */
    SearchResults searchResults;
    UTF8 results;
    size_t numResults =
        modelKey.Find(Search::Type::Segment,
                      Search::Space::SubsegmentsAndIncludes, searchResults);

    /* The next block shows you how to insert text
     * 1. Make sure that the visibility of text is turned on. You can explicitly
     *turn it on with a call to GetVisibilityControl().SetText(true)
     * 2. Insert the text by specifying the position where you want the text to
     *appear, and the text itself */
    SegmentKey textKey = geometry.Subsegment();
    textKey.GetVisibilityControl().SetText(true);
#ifdef TARGET_OS_ANDROID
    char buf[1024] = {0};
    sprintf(buf, "Number of segments: %u", numResults);
    results = buf;
#else
    results = "Number of segments: " + UTF8(std::to_string(numResults).c_str());
#endif
    textKey.InsertText(Point(0, 0, -0.7f), static_cast<char const*>(results));

    /* The next block shows you a different search.
     * This time an iterator is obtained, so that the geometry matching the
     *search criteria can be inspected
     * 1. Set up a while loop to walk though the SearchResult object with an
     *iterator
     * 2. At the beginning of the loop check whether the iterator is valid with
     *a call to its IsValid() method
     * 3. Obtain the key for the geometry the iterator is pointing to with a
     *call to its GetItem() method
     * 4. You can complete different actions based on the type of the key
     *returned by GetItem(). To find out what kind of key was returned, compare
     *the key type with one of those included in the HPS::Type enum
     * 5. Increment the iterator with a call to its Next() method*/
    numResults =
        modelKey.Find(Search::Type::Shell,
                      Search::Space::SubsegmentsAndIncludes, searchResults);
#ifdef TARGET_OS_ANDROID
    sprintf(buf, "Number of segments: %u", numResults);
    results = buf;
#else
    results = "Number of shells: " + UTF8(std::to_string(numResults).c_str());
#endif
    textKey.InsertText(Point(0, 0, -0.8f), static_cast<char const*>(results));

    SearchResultsIterator it = searchResults.GetIterator();
    size_t vertexCount = 0;
    while (it.IsValid()) {
        Key key = it.GetItem();

        if (key.Type() == Type::ShellKey) {
            PointArray points;
            ShellKey sKey(key);
            sKey.ShowPoints(points);
            vertexCount += points.size();
        }
        it.Next();
    }
#ifdef TARGET_OS_ANDROID
    sprintf(buf, "Number of segments: %u", vertexCount);
    results = buf;
#else
    results =
        "Number of vertices: " + UTF8(std::to_string(vertexCount).c_str());
#endif
    textKey.InsertText(Point(0, 0, -0.9f), static_cast<char const*>(results));

    /* The next block of code shows you how to set up a new Camera from a
     * CameraKit object
     * 1. Set up the camera position, target, up vector, field and projection
     * attributes
     * 2. Set the camera on the legend segment */
    CameraKit cameraKit;
    cameraKit.SetPosition(Point(0, 0, -5))
        .SetTarget(Point(0, 0, 0))
        .SetUpVector(Vector(0, 1, 0))
        .SetField(2, 2)
        .SetProjection(Camera::Projection::Stretched);
    textKey.SetCamera(cameraKit);

    view.FitWorld();
}
