#if defined __APPLE__
#include "SamplesMacOS.h"
#else
#include "sample.h"
#endif

#include "sample_common.h"
#include "sprk.h"

using namespace HPS;

void Sample::Image_Background(WindowKey wk, Canvas canvas, View view,
                              Model model, SegmentKey modelKey) {
    /* Sample: Image_Background
     * This sample will show you how to:
     * - Load an HSF file through Stream and check for Stream IO exceptions
     * - Search portfolios for image definitions
     * - Apply a portfolio to a segment
     * - Search a list of named images
     * - Apply an image as the window background
     *
     * Read more about the topics covered in this sample in our Programming
     * Guide
     * - section 1.2.4:	Windows
     * - section 3.3:	Subwindows
     * - section 4.1:	Portfolios */

    /* This next block of code shows you how to load an HSF file
     * 1. Create an ImportOptionsKit and set the segment in which you want the
     * file to be loaded
     * 2. Use the Stream::File::Import method to load the desired file, with the
     * options you specified
     * 3. Since this operation happens on a separate thread, wait for it to
     * complete before proceeding
     * 4. Use a try-catch block to capture any Stream IO exceptions */
    try {
        /* Inserting an HSF file which contains three named images */
        Stream::ImportOptionsKit importOptionsKit;
        importOptionsKit.SetSegment(modelKey);
        UTF8 filename = INPUT_FOLDER + "/named_images.hsf";

        Stream::ImportNotifier notifier = Stream::File::Import(
            static_cast<char const*>(filename), importOptionsKit);
        notifier.Wait();
    } catch (IOException e) {
        /* catch Stream I/O Errors here */
    }

    /* TODO:
     * A new method will be added to ImportOptionsKit which will allow the user
     * to specify in which portfolio the image will end up in, instead of having
     * to search all portfolios.
     *
     * We will need to rewrite this sample to take advantage of that feature */

    /* The next block of code shows how to search portfolios for a specific
     *image of which we know the name
     * 1. Obtain a list of all the portfolios in the database
     * 2. Check all portfolios for ImageDefinition objects
     * 3. When ImageDefinition objects are found, iterate through them checking
     *whether their name matches the name of the desired image, using the
     *ImageDefinition::Name() method */
    PortfolioKeyArray portfolioKeys = Database::GetPortfolios();
    PortfolioKey pKey;
    ImageDefinition image;

    for (unsigned int i = 0; i < portfolioKeys.size(); i++) {
        ImageDefinitionArray imageDefinitions;
        portfolioKeys.at(i).ShowAllImageDefinitions(imageDefinitions);

        for (unsigned int j = 0; j < imageDefinitions.size(); j++) {
            if (imageDefinitions.at(j).Name() == "dots") {
                pKey = portfolioKeys.at(i);
                image = imageDefinitions.at(j);
                break;
            }
        }
    }

    /* This block of code shows how to apply an ImageDefinition object to the
     *window background
     * 1. The portfolio containing the image definition for the image to be
     *applied to the window has to be visible at the window level. To make sure
     *this is true, apply the portfolio to the Window's Style
     * 2. Set the image as the background by obtaining the SubwindowControl from
     *a WindowKey, and using its SetBackground() method
     * 3. The first argument of SetBackground needs to be
     *Subwindow::Background::Image to set the image as a background
     * 4. The second argument is the name of the image to be applied to the
     *background */
    canvas.GetWindowKey().GetPortfolioControl().Push(pKey);
    canvas.GetWindowKey().GetSubwindowControl().SetBackground(
        Subwindow::Background::Image, image.Name());
}
