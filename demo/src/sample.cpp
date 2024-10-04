#include "sample.h"
#include <algorithm>
#include <string>
#include <iostream>


#ifndef _MSC_VER
#	include <unistd.h>
#endif

using namespace HPS;

HPS::UTF8 Sample::GetExecutableDirectory()
{
#ifdef _MSC_VER
	char executableDirectory[MAX_PATH];
	GetModuleFileNameA(nullptr, executableDirectory, sizeof(executableDirectory));
	char * lastSlash = strrchr(executableDirectory, '\\');
	lastSlash[0] = '\0';
#else
	char executableDirectory[PATH_MAX];
	ssize_t bytes = readlink("/proc/self/exe", executableDirectory, sizeof(executableDirectory));
	char * lastSlash = strrchr(executableDirectory, '/');
	lastSlash[0] = '\0';
#endif
	return executableDirectory;
}

Sample::Sample(World* world)
{
	availableTests.push_back(Test(0,	"lightweight_subwindow"));
	availableTests.push_back(Test(1,	"bloom_silhouette"));
	availableTests.push_back(Test(2,	"database_search"));
	availableTests.push_back(Test(3,	"hidden_line"));
	availableTests.push_back(Test(4,	"image_background"));
	availableTests.push_back(Test(5,	"image_io"));
	availableTests.push_back(Test(6,	"image_output"));
	availableTests.push_back(Test(7,	"mask_transforms"));
	availableTests.push_back(Test(8,	"meshes"));
	availableTests.push_back(Test(9,	"quickmoves"));
	availableTests.push_back(Test(10,	"render_modes"));
	availableTests.push_back(Test(11,	"segment_attributes"));
	availableTests.push_back(Test(12,	"sub_shell_attributes"));
	availableTests.push_back(Test(13,	"spotlight"));
	availableTests.push_back(Test(14,	"textures"));
	availableTests.push_back(Test(15,	"exchange_publish"));
	availableTests.push_back(Test(16,	"hidden_line_advanced"));
	availableTests.push_back(Test(17,	"shell_attributes"));
	availableTests.push_back(Test(18,	"priority"));
	availableTests.push_back(Test(19,	"interpolation"));
	availableTests.push_back(Test(20,	"exchange_pmi_highlighting"));
	availableTests.push_back(Test(21,	"exchange_component_highlighting"));
	availableTests.push_back(Test(22,	"custom_event"));
	availableTests.push_back(Test(23,	"conditional_styles_and_includes"));
	availableTests.push_back(Test(24,	"hardcopy_export"));
	availableTests.push_back(Test(25,	"selection"));
	availableTests.push_back(Test(26,	"exchange_component_interaction"));
	availableTests.push_back(Test(27,	"insert_cube"));
	availableTests.push_back(Test(28,	"select_by_point"));
	availableTests.push_back(Test(29,   "highlighting"));
	availableTests.push_back(Test(30,   "publish_widgets"));
	availableTests.push_back(Test(31,   "complex_clip_regions"));
	availableTests.push_back(Test(32,   "reference_geometry"));

	driver = "opengl2";
	testToRun = new Test();
	runAllTests = false;
	doNotPause = false;

	UTF8 executableDirectory = GetExecutableDirectory();
	INPUT_FOLDER = executableDirectory + "/../../samples/data";
	OUTPUT_FOLDER = executableDirectory + "/../../samples/code/output";

	InitHPS(world);
}

Sample::~Sample()
{
	delete testToRun;
}

bool Sample::ProcessArguments(int argc, char** argv)
{
	if(argc < 2)
		return false;

	int argCount = 1;

	while(argCount < argc)
	{
		char flag = ' ';
		char* currentArg = argv[argCount];
		if(currentArg[0] != '-')
			return false;

		if(currentArg[1] != 't' && currentArg[1] != 'd' && currentArg[1] != 'a' && currentArg[1] != 'c')
			return false;
		else
		{
			flag = currentArg[1];
			currentArg++;
			if(flag == 't')
			{
				std::string testNameString = argv[++argCount];
				std::transform(testNameString.begin(), testNameString.end(), testNameString.begin(),
					[](unsigned char c) { return static_cast<unsigned char>(::tolower(c)); });
				testToRun->testName = UTF8(testNameString.c_str());
				Test item(-1, testToRun->testName);
				auto it = std::find(availableTests.cbegin(), availableTests.cend(), item); 

				if(it == availableTests.cend())
					return false;

				testToRun->testID = it->testID;
			}
			if(flag == 'd')
			{
				if (argCount < argc - 1)
				{
					std::string driverNameString = argv[++argCount];
					std::transform(driverNameString.begin(), driverNameString.end(), driverNameString.begin(),
						[](unsigned char c) { return static_cast<unsigned char>(::tolower(c)); });
					driver = UTF8(driverNameString.c_str());
				}
				else
					driver = "opengl2";
			}
			if(flag == 'a')
			{
				runAllTests = true;
			}
			if (flag == 'c')
			{
				doNotPause = true;
			}
			argCount++;
		}
	}
	return true;
}

void Sample::RunTest(Test* test)
{
	Window::Driver driverToUse;
	if (driver == "opengl")
		driverToUse = Window::Driver::OpenGL;
#ifdef _MSC_VER
	else if (driver == "directx11"  || driver == "dx11")
 		driverToUse = Window::Driver::DirectX11;
#endif
	else if (driver == "opengl2" || driver == "ogl2")
		driverToUse = Window::Driver::OpenGL2;
	else
	{
		std::cout << "Driver " << driver << " is unknown, defaulting to OpenGL2\n";
		driverToUse = Window::Driver::OpenGL2;
		driver = "opengl2";
	}

	StandAloneWindowOptionsKit wo;
	wo.SetMobility(Window::Mobility::Free);
	wo.SetSubscreen(HPS::Rectangle(-0.75f, 0.75f, -0.75f, 0.75f));
	wo.SetDriver(driverToUse);
	UTF8 windowName = "HPS Samples - " + testToRun->testName + UTF8(" - ") + driver;
	wo.SetTitle(windowName);

	StandAloneWindowKey wk = Database::CreateStandAloneWindow(wo);
	Canvas canvas = Factory::CreateCanvas(wk);
	View view = Factory::CreateView();
	canvas.AttachViewAsLayout(view);
	Model model = Factory::CreateModel();
	view.AttachModel(model);
	SegmentKey modelKey = model.GetSegmentKey();
	modelKey.InsertDistantLight(Vector(1, 1, 0));

	switch(test->testID)
	{
	case 0:
		Lightweight_Subwindow(wk, canvas, view, model, modelKey);
		break;
	case 1:
		Bloom_Silhouette(wk, canvas, view, model, modelKey);
		break;
	case 2:
		Database_Search(wk, canvas, view, model, modelKey);
		break;
	case 3:
		Hidden_Line(wk, canvas, view, model, modelKey);
		break;
	case 4:
		Image_Background(wk, canvas, view, model, modelKey);
		break;
	case 5:
		Image_Io(wk, canvas, view, model, modelKey);
		break;
	case 6:
		Image_Output(wk, canvas, view, model, modelKey);
		break;
	case 7:
		Mask_Transforms(wk, canvas, view, model, modelKey);
		break;
	case 8:
		Meshes(wk, canvas, view, model, modelKey);
		break;
	case 9:
		Quickmoves(wk, canvas, view, model, modelKey);
		break;
	case 10:
		Render_Modes(wk, canvas, view, model, modelKey);
		break;
	case 11:
		Segment_Attributes(wk, canvas, view, model, modelKey);
		break;
	case 12:
		Sub_Shell_Attributes(wk, canvas, view, model, modelKey);
		break;
	case 13:
		Spotlight(wk, canvas, view, model, modelKey);
		break;
	case 14:
		Textures(wk, canvas, view, model, modelKey);
		break;
	case 16:
		Hidden_Line_Advanced(wk, canvas, view, model, modelKey);
		break;
	case 17:
		Shell_Attributes(wk, canvas, view, model, modelKey);
		break;
	case 18:
		Priority(wk, canvas, view, model, modelKey);
		break;
	case 19:
		Interpolation(wk, canvas, view, model, modelKey);
		break;
	case 22:
		Custom_Event(wk, canvas, view, model, modelKey);
		break;
	case 23:
		Conditional_Styles_And_Includes(wk, canvas, view, model, modelKey);
		break;
	case 24:
		Hardcopy_Export(wk, canvas, view, model, modelKey);
		break;
	case 25:
		Selection(wk, canvas, view, model, modelKey);
		break;
	case 27:
		Insert_Cube(wk, canvas, view, model, modelKey);
		break;
	case 28:
		Select_By_Point(wk, canvas, view, model, modelKey);
		break;
	case 29:
		Highlighting(wk, canvas, view, model, modelKey);
		break;
	case 32:
		Reference_Geometry(wk, canvas, view, model, modelKey);
		break;

	}

	wk.UpdateWithNotifier().Wait();
	if (!doNotPause)
	{
		wk.Pause();
	}
	wk.Flush();
	wk.Delete();
}
