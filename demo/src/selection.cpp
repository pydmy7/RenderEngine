#if defined __APPLE__
#  include "SamplesMacOS.h"
#else
#  include "sample.h"
#endif

#include "sample_common.h"
#include "sprk.h"

using namespace HPS;

void Sample::Selection(WindowKey wk, Canvas canvas, View view, Model model, SegmentKey modelKey)
{
    /* Sample: Selection
     * This sample will show you how to:
     * - Load an HSF file and check for IO exceptions
     * - Define a glyph to be used as a marker symbol
     * - Define a named style
     * - Select a single instance of an entity
     * - Modify selection option and selectability
     * - Highlight selected geometry
     *
     * Read more about the topics covered in this sample in our Programming Guide
     * - Section 6.1:		Selection
     * - Section 6.2:		Highlighting
     * - Section 4.3:		Styles
     * - Section 4.5:		Glyphs */

    /* This next block of code shows you how to load an HSF file
     * 1. Create an ImportOptionsKit and set the segment in which you want the file to be loaded
     * 2. Use the Stream::File::Import method to load the desired file, with the options you specified
     * 3. Since this operation happens on a separate thread, wait for it to complete before proceeding
     * 4. Use a try-catch block to capture any Stream IO exceptions */
    SegmentKey geometry = modelKey.Subsegment();
    Stream::ImportNotifier notifier;
    try {
        Stream::ImportOptionsKit importOptionsKit;
        importOptionsKit.SetSegment(geometry);
        UTF8 filename = INPUT_FOLDER + "/suspension.hsf";

        notifier = Stream::File::Import(static_cast<char const*>(filename), importOptionsKit);
        notifier.Wait();
    }
    catch (IOException e) {
        // handle Stream IO errors here
    }

    CameraKit cameraKit;
    cameraKit.SetPosition(Point(10, 5, 30))
        .SetTarget(Point(0.5f, -0.4f, 0.8f))
        .SetUpVector(Vector(0, 1, 0))
        .SetField(9.21f, 9.21f)
        .SetProjection(Camera::Projection::Orthographic);
    modelKey.SetCamera(cameraKit);

    /* This block of code shows how to define a glyph and a named style
     * 1. For both of these definitions a portfolio is needed, to obtain a portfolio key,
     *		we can use the PortfolioControl, and call its ShowTop method to obtain the key
     *		of the portfolio on top of the portfolio stack.
     *		NOTE: Remember that this portfolio must be visible by the segment which will use
     *		the glyph and styles defined in it
     * 2. To define a glyph, provide a name for the glyph and choose its symbol from the many
     *		default symbols provided by Visualize
     * 3. To define a named style, first create a segment which contains the attribute for the
     *		style, and then call the PortfolioKey::DefineNamedStyle method */
    PortfolioKey pKey;
    modelKey.GetPortfolioControl().ShowTop(pKey);
    pKey.DefineGlyph("myGlyph", GlyphKit::GetDefault(Glyph::Default::Plus));

    SegmentKey highlightStyle = modelKey.Subsegment();
    highlightStyle.GetMaterialMappingControl().SetFaceColor(RGBAColor(1, 0.5f, 0, 1)).SetEdgeColor(RGBAColor(1, 0.5f, 0, 1));
    pKey.DefineNamedStyle("orangeHighlight", highlightStyle);

    /* The next block of code inserts the marker in the scene. The marker is as a metaphor for
     * a mouse pointer, to show the user where the selection will take place
     * 1. Using the MarkerAttributeControl, set the symbol to be used for this marker and its size.
     *		The symbol chosen here is the one defined in the portfolio above
     * 2. The marker is inserted here using a MarkerKit, which specifies the point in World
     *		coordinates where the marker will appear
     * 3. Since the marker needs to always be visible, on top of the scene, we set Depth Range
     *		on the segment containing it, through its DrawingAttributeControl
     * 4. Finally, since we do not want the marker itself to be selectable, we turn its
     *		selectability off using a SelectabilityControl */
    MarkerKit marker;
    Point markerPosition(0.2f, 0.1f, 0);
    marker.SetPoint(markerPosition);

    SegmentKey markerSegment = geometry.Subsegment();
    markerSegment.GetMarkerAttributeControl().SetSymbol("myGlyph").SetSize(3);
    markerSegment.InsertMarker(marker);
    markerSegment.GetDrawingAttributeControl().SetDepthRange(0.1f, 0.2f);
    markerSegment.GetVisibilityControl().SetMarkers(true);
    markerSegment.GetMaterialMappingControl().SetMarkerColor(RGBAColor(1, 0, 0, 1));
    markerSegment.GetSelectabilityControl().SetEverything(Selectability::Value::Off);

    view.Update();

    /* Before performing a selection on 'markerPosition' we need to convert its coordinates from
     * the World frame of reference (in which they were inserted) to the Window frame of reference, which
     * is used to perform selection.
     *
     * In order to correctly account for cameras and modeling matrices between the window and the segment
     * in which the geometry resides, we build a key path.
     * A key path starts from the leaf segment (in this case the segment where the marker is) and contains
     * a list of segment keys, all the way to the window. Visualize will try to fill in segments automatically
     * if possible.
     *
     * All include keys need to be included in the key path, in order to provide an unequivocal path
     *
     * Once the KeyPath is ready, we can use its ConvertCoordinate method, to obtain the point we will
     * use for selection */
    KeyArray keyArray(5);
    keyArray[0] = markerSegment;
    keyArray[1] = view.GetAttachedModelIncludeLink();
    keyArray[2] = canvas.GetAttachedLayout().GetAttachedViewIncludeLink(0);
    keyArray[3] = canvas.GetAttachedLayoutIncludeLink();
    keyArray[4] = wk;

    KeyPath keyPath;
    keyPath.SetKeys(keyArray);
    Point selectionPosition;
    keyPath.ConvertCoordinate(Coordinate::Space::World, markerPosition, Coordinate::Space::Window, selectionPosition);

    /* This block of code shows a few selection options as well as the selection procedure itself
     * 1. In this example we choose the selection level to 'Entity'. 'Segment' and 'Subentity' levels are
     *		also available
     * 2. Setting the related selection limit to zero makes it so that we will only get one item from the
     *		selection routine
     * 3. Turning selection sorting on, will make sure that the item closer to the camera will be on top of
     *		the selection list.
     *
     *	The SelectByPoint method selects under the selectionPosition point using the options we pass to it.
     *	It is also possible to select by Area, Ray, etc... */
    SelectionOptionsKit selectionOptions;
    SelectionResults selectionResults;
    size_t numberOfSelectedItems;
    selectionOptions.SetLevel(Selection::Level::Entity).SetRelatedLimit(0).SetSorting(Selection::Sorting::Default);
    numberOfSelectedItems = wk.GetSelectionControl().SelectByPoint(selectionPosition, selectionOptions, selectionResults);

    /* This block of code shows how to walk a list of selected items and how to highlight them:
     * 1. Obtain an iterator for the selection results
     * 2. Use a loop that checks whether SelectionResultsIterator::IsValid() is true to walk the list
     * 3. Use SelectionResultsIterator::Next() to advance to the next iterator
     * 4. It is possible to obtain the SelectionItem from the SelectionResultsIterator by using its GetItem() method
     * 5. Once we have an item, we can use its ShowPath() method to get a clear path to the instance of the geometry
     *		that was selected. For example, here we select a spring. There are two instances of the spring entity
     *		in this scene. Using a key path ensures that only the correct one is selected.
     * 6. To highlight the selected entity, we first create a HighlightOptionsKit object and set its style to that
     *		of the named style we defined above
     * 7. The WindowKey::Highlight method can be used to highlight the geometry pointed at by the key path we
     *		obtained, using the highlight options defined in the HighlightOptionsKit object
     * */
    SelectionResultsIterator it = selectionResults.GetIterator();
    HighlightOptionsKit hok;
    hok.SetStyleName("orangeHighlight");
    while (it.IsValid()) {
        KeyPath selectionPath;
        SelectionItem selection = it.GetItem();
        selection.ShowPath(selectionPath);

        wk.GetHighlightControl().Highlight(selectionPath, hok);
        it.Next();
    }

    /* This last piece of code is used to output a description of the scene to the window
     * 1. DepthRange is set to make sure that the text will appear on top of other geometry
     * 2. A local camera is set on this segment to allow us to easily position the text
     *		in relation to it
     * 3. Text attributes are set as desired and text is entered */
    SegmentKey description = modelKey.Subsegment();
    description.GetDrawingAttributeControl().SetDepthRange(0.0f, 0.1f);
    description.GetVisibilityControl().SetEverything(false).SetText(true);
    description.GetCameraControl()
        .SetPosition(Point(0, 0, -5))
        .SetTarget(Point(0.3f, 0, 0))
        .SetUpVector(Vector(0, 1, 0))
        .SetField(2, 2)
        .SetProjection(Camera::Projection::Stretched);
    description.GetTextAttributeControl()
        .SetSize(0.02f, Text::SizeUnits::SubscreenRelative)
        .SetAlignment(Text::Alignment::CenterRight, Text::ReferenceFrame::WorldAligned, Text::Justification::Center);
    description.InsertText(Point(0, -0.85f, 0), "The entity under the red cross");
    description.InsertText(Point(0, -0.90f, 0), "is selected and highlighted in orange");

    view.Update(Window::UpdateType::Complete);

}
