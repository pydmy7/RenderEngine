#if defined __APPLE__
#  include "SamplesMacOS.h"
#else
#  include "sample.h"
#endif

#include "sample_common.h"
#include "sprk.h"

#include <fstream>

using namespace HPS;

void Sample::Image_Io(WindowKey wk, Canvas canvas, View view, Model model, SegmentKey modelKey)
{

    /* Sample: Image_Io
     * This sample will show you how to:
     * - Load a JPG file through HPS::Image and check for Image IO exceptions
     * - Obtain an ImageKit
     * - Obtain a TextKit
     * - Convert an image between formats
     * - Output an image to a file
     * - Import an image from a file
     * - Insert text
     * - Insert and Image
     *
     * Read more about the topics covered in this sample in our Programming Guide
     * - section 8.3:	Loading 2D Images */

    HPS::SegmentKey results_segment = modelKey.Subsegment();

    HPS::TextKit textKit = HPS::TextKit();
    textKit.SetSize(0.02f, Text::SizeUnits::SubscreenRelative);
    PointArray pointArray(3);
    pointArray[0] = HPS::Point(-0.95f, -0.65f, 10);
    pointArray[1] = HPS::Point(0.95f, -0.65f, 10);
    pointArray[2] = HPS::Point(0, -1, 10);
    textKit.SetRegion(pointArray, Text::RegionAlignment::Center, Text::RegionFitting::Left, false, false, true);
    textKit.SetBackground(true, "rounded box");
    textKit.SetAlignment(HPS::Text::Alignment::BottomLeft);
    results_segment.GetMaterialMappingControl().SetFaceColor(RGBAColor(1, 1, 1));
    results_segment.GetMaterialMappingControl().SetEdgeColor(RGBAColor(0, 0, 0));
    results_segment.GetVisibilityControl().SetEdges(true);

    /* This next block of code shows you how to load an HSF file
     * 1. Create an ImportOptionsKit and set the format of the image to be read in
     * 2. Use the Image::File::Import method to load the desired image, with the options you specified
     * 3. An ImageKit describing the inserted image is returned by Image::File::Import
     * 4. Use a try-catch block to capture any Image IO exceptions */
    ImageKit jpgImageKit;
    unsigned int imageWidth, imageHeight;
    try {
        UTF8 file_in = INPUT_FOLDER + "/sprocket_image.jpg";
        Image::ImportOptionsKit importOptionsKit;
        importOptionsKit.SetFormat(Image::Format::Jpeg);
        jpgImageKit = Image::File::Import(static_cast<char const*>(file_in), importOptionsKit);
    }
    catch (IOException e) {
        UTF8 error_message;
        error_message = "Image Import Unsuccessful";
        modelKey.InsertText(Point(0, 0, 0), static_cast<char const*>(error_message));
        view.FitWorld();
        view.Update();
    }

    /* The next block of code shows how to convert between image formats and how to write an image to a file
     * 1. Create a new ImageKit object and use its Convert method to change formats
     * 2. The first argument of Convert is the source ImageKit
     * 3. The second argument of Convert is the desired output format
     * 4. Once the new ImageKit is obtained through Convert, obtain its data using the ImageKit::ShowData() method
     * 5. Write the data out to a file in binary mode */
    if (!jpgImageKit.Empty()) {
        bool exportSuccessful = true;
        ImageKit pngImageKit;
        ByteArray imageData;
        std::ofstream file;

        jpgImageKit.ShowSize(imageWidth, imageHeight);
        pngImageKit.SetFormat(Image::Format::Png).SetSize(imageWidth, imageHeight);
        pngImageKit.Convert(jpgImageKit, Image::Format::Png);
        pngImageKit.ShowData(imageData);

        UTF8 file_out = OUTPUT_FOLDER + "/image_io.png";

        file.open(static_cast<char const*>(file_out), std::ios::out | std::ios::binary);
        if (file.fail())
            exportSuccessful = false;

        else {
            size_t size = imageData.size();
            for (size_t i = 0; i < size; i++) {
                file << imageData.at(i);
            }
        }
        if (file.fail())
            exportSuccessful = false;

        file.close();
        if (file.fail())
            exportSuccessful = false;

        if (!exportSuccessful) {
            UTF8 error_message;
            error_message = "Image Export Unsuccessful";
            modelKey.InsertText(Point(0, 0, 0), static_cast<char const*>(error_message));
            view.FitWorld();
            view.Update();
        }
        else {
            /* The next block of code shows you how to delete geometry from a segment and how to insert text
             * 1. The Flush method can be used to delete different elements from a segment
             * 2. The first argument of Flush specifies what to delete
             * 3. The second argument specifies which part of the scene graph will be affected by this operation
             * 4. To insert text, specify the text position and the text you want displayed*/

            UTF8 success_message = "Import and Export completed successfully.\n"
                                   "You can find the PNG image here:\n   " +
                                   file_out;

            textKit.SetText(success_message);
            results_segment.InsertText(textKit);

            /* The next block of code shows how to import an image, convert it to a texture, and place it in the scene using a
             * Shell
             * 1. Define a ShellKit to serve as a display for the image
             * 2. Create a rectangle with the Visualize shell primitive
             * 2. Define a PortfolioKey to hold a texture
             * 3. Define an ImageKit to hold the imported image
             * 4. Define an import options kit to declare the file format(png, jpeg, gif etc.)
             * 5. Import the image into your ImageKit using the file's path as the first parameter and the optionskit as the
             * second parameter
             * 6. Use a try-catch block to capture any Stream IO exceptions */

            float heightRatio;
            float widthRatio;
            float heightMargin = 0.05;
            float widthMargin = 0.05;
            if (imageHeight >= imageWidth) {
                heightRatio = 1.0f;
                widthRatio = (float)imageWidth / imageHeight;
                widthMargin = widthMargin * widthRatio;
                heightRatio -= heightMargin;
                widthRatio -= widthMargin;
            }
            else {
                heightRatio = (float)imageHeight / imageWidth;
                widthRatio = 1.0f;
                heightMargin = heightMargin * heightRatio;
                heightRatio -= heightMargin;
                widthRatio -= widthMargin;
            }

            ShellKit myShellKit;
            Point points[] = {Point(widthRatio, -heightRatio, 0),
                              Point(-widthRatio, -heightRatio, 0),
                              Point(-widthRatio, heightRatio, 0),
                              Point(widthRatio, heightRatio, 0)};
            int faceList[] = {4, 0, 1, 2, 3};

            HPS::SizeTArray vertices_map;
            vertices_map.resize(4);
            vertices_map.resize(4);
            vertices_map[0] = 0;
            vertices_map[1] = 1;
            vertices_map[2] = 2;
            vertices_map[3] = 3;

            HPS::FloatArray vparams;
            vparams.resize(8); // for UV coordinates on a quad, see texture coordinates discussion
            vparams[0] = 0;
            vparams[1] = 0; // [0, 0]
            vparams[2] = 1;
            vparams[3] = 0; // [1, 0]
            vparams[4] = 1;
            vparams[5] = 1; // [1, 1]
            vparams[6] = 0;
            vparams[7] = 1; // [0, 1]

            myShellKit.SetPoints(4, points);
            myShellKit.SetFacelist(5, faceList);
            myShellKit.SetVertexParametersByList(vertices_map, vparams, 2);

            SegmentKey imageSegment = modelKey.Subsegment("Image");
            imageSegment.InsertShell(myShellKit);
            imageSegment.GetVisibilityControl().SetFaceLights(false);
            HPS::PortfolioKey portfolio_key = HPS::Database::CreatePortfolio();
            HPS::ImageKit image_kit;

            try {
                UTF8 filename = OUTPUT_FOLDER + "/image_io.png";

                HPS::Image::ImportOptionsKit importOptionsKit;
                importOptionsKit.SetFormat(HPS::Image::Format::Png);
                image_kit = HPS::Image::File::Import(filename, importOptionsKit);
            }
            catch (HPS::IOException) {
                exportSuccessful = false;
            }

            /* The next block of code will show how to convert an image to a texture in the Visualize context and place it in your
             * scene
             * 1. Create an ImageDefinition within the portfolio key and a new textureOptionsKit
             * 2. Set the ParameterizationSource as UV. See section 5.3 for more details
             * 3. Define the texture within the portfolio key
             * 4. Assign the portfolio to the segment
             * 5. Set the model key face texture the new texture within the portfolio using it's earlier defined name to find it
             */

            HPS::ImageDefinition imageDefinition = portfolio_key.DefineImage("imported_image", image_kit);
            HPS::TextureOptionsKit textureOptionsKit;

            textureOptionsKit.SetParameterizationSource(HPS::Material::Texture::Parameterization::UV);
            portfolio_key.DefineTexture("my_texture", imageDefinition, textureOptionsKit);
            modelKey.GetPortfolioControl().Push(portfolio_key);
            modelKey.GetMaterialMappingControl().SetFaceTexture("my_texture");

            view.Update();
        }
    }

	
}
