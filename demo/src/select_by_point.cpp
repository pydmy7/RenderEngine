#if defined __APPLE__
#  include "SamplesMacOS.h"
#else
#  include "sample.h"
#endif

#include "sample_common.h"
#include "sprk.h"

using namespace HPS;

void Sample::Select_By_Point(WindowKey wk, Canvas canvas, View view, Model model, SegmentKey modelKey)
{
	WindowKey myWindowKey(wk);

    Point selectionPoint(0, 0, 0);

    modelKey.InsertSphere(Point(0, 0, 1), 0.5f);
    modelKey.InsertSphere(Point(0, 0, 3), 0.5f);
    modelKey.InsertSphere(Point(0, 0, 5), 0.5f);

    myWindowKey.UpdateWithNotifier().Wait();

    // example 1: analytic selection
    myWindowKey.GetSelectionOptionsControl().SetLevel(HPS::Selection::Level::Entity); // this will be an entity-level selection
    myWindowKey.GetSelectionOptionsControl().SetAlgorithm(HPS::Selection::Algorithm::Analytic);

    HPS::SelectionResults selectionResults;
    size_t numSelectedItems = myWindowKey.GetSelectionControl().SelectByPoint(selectionPoint, selectionResults);

    // this is an analytic selection, all 3 spheres should be returned as selected
    if (numSelectedItems != 3) {
        // if we get here, something failed
    }

    // example 2: visual selection
    myWindowKey.GetSelectionOptionsControl().SetAlgorithm(HPS::Selection::Algorithm::Visual);
    numSelectedItems = myWindowKey.GetSelectionControl().SelectByPoint(selectionPoint, selectionResults);

    // this is an visual selection, only 1 sphere should be returned
    if (numSelectedItems == 1) {
        // if we get here, something failed
    }

    // here is where you collect the selection results
    HPS::SelectionResultsIterator srIterator = selectionResults.GetIterator();

    while (srIterator.IsValid()) {
        HPS::SelectionItem selectionItem = srIterator.GetItem();
        Key key;
        selectionItem.ShowSelectedItem(key);

        // do something with the selected item using its key

        srIterator.Next();
    }


}
