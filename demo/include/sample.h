#ifndef _SAMPLE_H_
#define _SAMPLE_H_

#if defined(_MSC_VER)
#include <windows.h>
#pragma warning(disable : 4996)
#endif

#include "hps.h"
#include "sprk.h"
#include "sprk_ops.h"

using namespace HPS;

class Test;

class Sample {
public:
    Sample(World* world);
    ~Sample();
    bool ProcessArguments(int argc, char** argv);
    void RunTest(Test* testToRun);

public:
    // HPS SAMPLES
    void Lightweight_Subwindow(WindowKey wk, Canvas canvas, View view,
                               Model model, SegmentKey modelKey);
    void Bloom_Silhouette(WindowKey wk, Canvas canvas, View view, Model model,
                          SegmentKey modelKey);
    void Database_Search(WindowKey wk, Canvas canvas, View view, Model model,
                         SegmentKey modelKey);
    void Exchange_Publish(WindowKey wk, Canvas canvas, View view, Model model,
                          SegmentKey modelKey);
    void Hidden_Line(WindowKey wk, Canvas canvas, View view, Model model,
                     SegmentKey modelKey);
    void Hidden_Line_Advanced(WindowKey wk, Canvas canvas, View view,
                              Model model, SegmentKey modelKey);
    void Image_Background(WindowKey wk, Canvas canvas, View view, Model model,
                          SegmentKey modelKey);
    void Image_Io(WindowKey wk, Canvas canvas, View view, Model model,
                  SegmentKey modelKey);
    void Image_Output(WindowKey wk, Canvas canvas, View view, Model model,
                      SegmentKey modelKey);
    void Insert_Cube(WindowKey wk, Canvas canvas, View view, Model model,
                     SegmentKey modelKey);
    void Mask_Transforms(WindowKey wk, Canvas canvas, View view, Model model,
                         SegmentKey modelKey);
    void Meshes(WindowKey wk, Canvas canvas, View view, Model model,
                SegmentKey modelKey);
    void Quickmoves(WindowKey wk, Canvas canvas, View view, Model model,
                    SegmentKey modelKey);
    void Render_Modes(WindowKey wk, Canvas canvas, View view, Model model,
                      SegmentKey modelKey);
    void Segment_Attributes(WindowKey wk, Canvas canvas, View view, Model model,
                            SegmentKey modelKey);
    void Sub_Shell_Attributes(WindowKey wk, Canvas canvas, View view,
                              Model model, SegmentKey modelKey);
    void Spotlight(WindowKey wk, Canvas canvas, View view, Model model,
                   SegmentKey modelKey);
    void Textures(WindowKey wk, Canvas canvas, View view, Model model,
                  SegmentKey modelKey);
    void Shell_Attributes(WindowKey wk, Canvas canvas, View view, Model model,
                          SegmentKey modelKey);
    void Priority(WindowKey wk, Canvas canvas, View view, Model model,
                  SegmentKey modelKey);
    void Interpolation(WindowKey wk, Canvas canvas, View view, Model model,
                       SegmentKey modelKey);
    void Exchange_PMI_Highlighting(WindowKey wk, Canvas canvas, View view,
                                   Model model, SegmentKey modelKey);
    void Exchange_Component_Highlighting(WindowKey wk, Canvas canvas, View view,
                                         Model model, SegmentKey modelKey);
    void Custom_Event(WindowKey wk, Canvas canvas, View view, Model model,
                      SegmentKey modelKey);
    void Conditional_Styles_And_Includes(WindowKey wk, Canvas canvas, View view,
                                         Model model, SegmentKey modelKey);
    void Hardcopy_Export(WindowKey wk, Canvas canvas, View view, Model model,
                         SegmentKey modelKey);
    void Selection(WindowKey wk, Canvas canvas, View view, Model model,
                   SegmentKey modelKey);
    void Select_By_Shell(WindowKey wk, Canvas canvas, View view, Model model,
                         SegmentKey modelKey);
    void Exchange_Component_Interaction(WindowKey wk, Canvas canvas, View view,
                                        Model model, SegmentKey modelKey);
    void Select_By_Point(WindowKey wk, Canvas canvas, View view, Model model,
                         SegmentKey modelKey);
    void Highlighting(WindowKey wk, Canvas canvas, View view, Model model,
                      SegmentKey modelKey);
    void Parasolid_Cube(WindowKey window, Canvas canvas, View view, Model model,
                        SegmentKey modelKey);
    void Publish_Widgets(WindowKey window, Canvas canvas, View view,
                         Model model, SegmentKey modelKey);
    void Complex_Clip_Regions(WindowKey window, Canvas canvas, View view,
                              Model model, SegmentKey modelKey);
    void Reference_Geometry(WindowKey window, Canvas canvas, View view,
                            Model model, SegmentKey modelKey);

public:
    std::vector<Test> availableTests;
    Test* testToRun;
    bool runAllTests;
    bool doNotPause;
    UTF8 driver;
    UTF8 INPUT_FOLDER;
    UTF8 OUTPUT_FOLDER;

private:
    UTF8 GetExecutableDirectory();

    inline void InitHPS(World* world) {
        world->SetMaterialLibraryDirectory(GetExecutableDirectory() +
                                           "/../../data/materials");
        UTF8 const HEXCHANGE_INSTALL_DIR(getenv("HEXCHANGE_INSTALL_DIR"));

        if (HEXCHANGE_INSTALL_DIR.IsValid()) {
            HPS::UTF8 bin_subdir;
#ifdef _MSC_VER
#ifdef WIN64
            bin_subdir = "win64_v142/";
#else
            bin_subdir = "win32_v142/";
#endif
#else
#ifdef _LP64
            bin_subdir = "linux64/";
#else
            bin_subdir = "linux32/";
#endif
#endif
            world->SetExchangeLibraryDirectory(HEXCHANGE_INSTALL_DIR + "/bin/" +
                                               bin_subdir);
            world->SetPublishResourceDirectory(HEXCHANGE_INSTALL_DIR +
                                               "/bin/resource");
        }
    }
};

class Test {
public:
    Test() : testID(-1), testName("") {}

    Test(int ID, UTF8 name) : testID(ID), testName(name) {}

    inline friend bool operator==(const Test& t1, const Test& t2);

    int testID;
    UTF8 testName;
};

bool operator==(const Test& t1, const Test& t2) {
    return (t1.testName == t2.testName);
}

#endif  // sample
