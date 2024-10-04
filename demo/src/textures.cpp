#if defined __APPLE__
#include "SamplesMacOS.h"
#else
#include "sample.h"
#endif

#include "sample_common.h"
#include "sprk.h"

using namespace HPS;

void Sample::Textures(WindowKey wk, Canvas canvas, View view, Model model,
                      SegmentKey modelKey) {
    /* Sample: Textures
     * This sample will show you how to:
     * - Load an HSF file through Stream and check for Stream IO exceptions
     * - Load an image file through HPS::Image and check for Image IO exceptions
     * - Search the database for segments, and iterate through a list of search
     * results
     * - Create a portfolio
     * - Define textures with sphere, cylinder and UV parameterizations
     * - Apply textures to geometry
     *
     * Read more about the topics covered in this sample in our Programming
     * Guide
     * - section 4.1:	Portfolios
     * - section 4.2:	Materials and Material Palettes
     * - section 4.3:	Texture Mapping */

    /* This next block of code shows you how to load an HSF file
     * 1. Create an ImportOptionsKit and set the format of the image to be read
     * in
     * 2. Use the Image::File::Import method to load the desired image, with the
     * options you specified
     * 3. An ImageKit describing the inserted image is returned by
     * Image::File::Import
     * 4. Use a try-catch block to capture any Image IO exceptions */
    ImageKit imageKit;
    try {
        UTF8 file_in = INPUT_FOLDER + "/wood.jpg";
        Image::ImportOptionsKit importOptionsKit;
        importOptionsKit.SetFormat(Image::Format::Jpeg);
        imageKit = Image::File::Import(static_cast<char const*>(file_in),
                                       importOptionsKit);
    } catch (IOException e) {
        UTF8 error_message;
        error_message = "Image Import Unsuccessful";
        modelKey.InsertText(Point(0, 0, 0),
                            static_cast<char const*>(error_message));
        view.FitWorld();
        view.Update();
    }

    /* This next block of code shows you how to load an HSF file
     * 1. Create an ImportOptionsKit and set the segment in which you want the
     * file to be loaded
     * 2. Use the Stream::File::Import method to load the desired file, with the
     * options you specified
     * 3. Since this operation happens on a separate thread, wait for it to
     * complete before proceeding
     * 4. Use a try-catch block to capture any Stream IO exceptions */
    try {
        /* Loading an HSF file containing a sphere, cylinder and cube */
        UTF8 file_in = INPUT_FOLDER + "/geometry.hsf";
        Stream::ImportOptionsKit importOptionsKit;
        importOptionsKit.SetSegment(modelKey);
        Stream::ImportNotifier notifier = Stream::File::Import(
            static_cast<char const*>(file_in), importOptionsKit);
        notifier.Wait();
    } catch (IOException e) {
        UTF8 error_message;
        error_message = "HSF Import Unsuccessful";
        modelKey.InsertText(Point(0, 0, 0),
                            static_cast<char const*>(error_message));
        view.FitWorld();
        view.Update();
    }

    /* The next block shows how to search the database
     * An iterator is obtained, so that the geometry matching the search
     *criteria can be inspected
     * 1. Use the Find() method to decide where to search and what to search for
     * 2. Obtain a SearchResultIterator object from the SearchResult returned to
     *the user in Find()
     * 3. Set up a while loop to walk though the SearchResult object with an
     *iterator
     * 4. At the beginning of the loop check whether the iterator is valid with
     *a call to its IsValid() method
     * 5. Obtain the key for the geometry the iterator is pointing to with a
     *call to its GetItem() method
     * 6. You can complete different actions based on the type of the key
     *returned by GetItem(). To find out what kind of key was returned, compare
     *the key type with one of those included in the HPS::Type enum
     * 7. Increment the iterator with a call to its Next() method*/
    SearchResults searchResults;
    modelKey.Find(Search::Type::Geometry, Search::Space::SubsegmentsAndIncludes,
                  searchResults);
    SearchResultsIterator it = searchResults.GetIterator();

    SegmentKey sphere, cylinder, cube;

    while (it.IsValid()) {
        Key key = it.GetItem();
        if (key.Owner().Name() == "sphere")
            sphere = (SegmentKey)key.Owner();
        else if (key.Owner().Name() == "cylinder")
            cylinder = (SegmentKey)key.Owner();
        else if (key.Owner().Name() == "cube")
            cube = (SegmentKey)key.Owner();

        it.Next();
    }

    /* The next block of code shows how to define a texture and how to apply it
     *to geometry
     * 1. Obtain the key of a portfolio. The portfolio needs to be visible from
     *the segment which will be using it therefore the portfolio obtained here
     *is the one set on the Model
     * 2. Define an image, by giving using the PortfolioKey::DefineImage()
     *		the first argument of DefineImage is the name to be given to the
     *image the second argument of DefineImage is the imageKit associated with
     *the image from which the texture will be created.
     * 3. Choose a parameterization source for the texture which will be created
     *using the PortfolioKey::SetParameterizationSource
     * 4. Define a texture using the PortfolioKey::DefineTexture()
     *		the first argument of DefineTexture is the name to be given to
     *the texture the second argument of DefineTexture is the ImageDefinition
     *object returned by the call to DefineImage the third argument of
     *DefineTexture is a TextureOptionsKit containing texture options such as
     *parameterization source
     * 5. To apply a texture to the geometry contained in a segment, obtain its
     *MaterialMapping control and use the SetFaceTexture() method*/
    PortfolioKeyArray portfolios;
    PortfolioKey pKey;
    ImageDefinition imageDefinition;
    TextureOptionsKit textureOptionsKit;

    modelKey.GetPortfolioControl().Show(portfolios);
    pKey = portfolios[0];

    imageDefinition = pKey.DefineImage("wood", imageKit);

    // Defining sphere texture
    textureOptionsKit.SetParameterizationSource(
        Material::Texture::Parameterization::Sphere);
    pKey.DefineTexture("wood_sphere", imageDefinition, textureOptionsKit);

    // Defining cylinder texture
    textureOptionsKit.SetParameterizationSource(
        Material::Texture::Parameterization::Cylinder);
    pKey.DefineTexture("wood_cylinder", imageDefinition, textureOptionsKit);

    // Defining UV texture
    textureOptionsKit.SetParameterizationSource(
        Material::Texture::Parameterization::UV);
    pKey.DefineTexture("wood_uv", imageDefinition, textureOptionsKit);

    sphere.GetMaterialMappingControl().SetFaceTexture("wood_sphere");
    cylinder.GetMaterialMappingControl().SetFaceTexture("wood_cylinder");
    cube.GetMaterialMappingControl().SetFaceTexture("wood_uv");

    view.FitWorld();
    view.GetSegmentKey().GetModellingMatrixControl().Rotate(15, 10, 0);
    view.Update(Window::UpdateType::Complete);
}
