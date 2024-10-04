#if defined __APPLE__
#  include "SamplesMacOS.h"
#else
#  include "sample.h"
#endif

#include "sample_common.h"
#include "sprk.h"

using namespace HPS;

void Sample::Insert_Cube(WindowKey wk, Canvas canvas, View view, Model model, SegmentKey modelKey)
{

    /* Sample: Insert cube
     * This sample will show you how to:
     * - Create a basic cube
     *
     * Read more about the topics covered in this sample in our Programming Guide
     * - section 2.1:	Shells */

    float size = 1.0f; // size of the cube, in world units
    Point center = Point(0, 0, 0); // center point of the cube

    PointArray pointArray;
    pointArray.resize(8);

    float halflength = size / 2;

    pointArray[0] = Point(-halflength + center.x, -halflength + center.y, halflength + center.z);
    pointArray[1] = Point(halflength + center.x, -halflength + center.y, halflength + center.z);
    pointArray[2] = Point(halflength + center.x, halflength + center.y, halflength + center.z);
    pointArray[3] = Point(-halflength + center.x, halflength + center.y, halflength + center.z);

    pointArray[4] = Point(-halflength + center.x, -halflength + center.y, -halflength + center.z);
    pointArray[5] = Point(halflength + center.x, -halflength + center.y, -halflength + center.z);
    pointArray[6] = Point(halflength + center.x, halflength + center.y, -halflength + center.z);
    pointArray[7] = Point(-halflength + center.x, halflength + center.y, -halflength + center.z);

    IntArray faceList;
    faceList.resize(30);
    faceList[0] = 4;
    faceList[1] = 0;
    faceList[2] = 1;
    faceList[3] = 2;
    faceList[4] = 3;

    faceList[5] = 4;
    faceList[6] = 1;
    faceList[7] = 5;
    faceList[8] = 6;
    faceList[9] = 2;

    faceList[10] = 4;
    faceList[11] = 5;
    faceList[12] = 6;
    faceList[13] = 7;
    faceList[14] = 4;

    faceList[15] = 4;
    faceList[16] = 4;
    faceList[17] = 7;
    faceList[18] = 3;
    faceList[19] = 0;

    faceList[20] = 4;
    faceList[21] = 3;
    faceList[22] = 2;
    faceList[23] = 6;
    faceList[24] = 7;

    faceList[25] = 4;
    faceList[26] = 0;
    faceList[27] = 4;
    faceList[28] = 5;
    faceList[29] = 1;

    modelKey.InsertShell(pointArray, faceList);

    view.GetSegmentKey().GetCameraControl().SetPosition(Point(2, 1, -5)).SetProjection(HPS::Camera::Projection::Perspective);

    view.FitWorld();


}
