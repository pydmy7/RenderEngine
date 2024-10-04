#include "sample.h"
#include "sample_common.h"
#include <iostream>

#include "hoops_license.h"

using namespace HPS;

int main(int argc, char** argv) {
    World _world(HOOPS_LICENSE);
    Sample sample(&_world);

    if (!sample.ProcessArguments(argc, argv)) {
        std::cout << "Usage:\n"
                  << "hps_samples [-a|-t test_name] [-d driver_name] [-c]\n\n"
                  << "Options:\n"
                  << "-a\tRun all tests.\n"
                  << "-c\tContinue. Do not pause after test.\n"
                  << "-d\tRun tests with specified driver.\n"
                  << "-t\tRun the specified test.\n\n"
                  << "Valid choices for test_name are:\n";
        for (unsigned int i = 0; i < sample.availableTests.size(); i++) {
            std::cout << sample.availableTests[i].testName << "\n";
        }
        UTF8 valid_drivers = "opengl, opengl2";
#ifdef _MSC_VER
        valid_drivers += ", directx11";
#endif
        std::cout << "\nValid choices for driver_name are:\n"
                  << valid_drivers << std::endl;
        return 0;
    }

    if (sample.runAllTests) {
        for (auto it = sample.availableTests.cbegin();
             it != sample.availableTests.cend(); it++) {
            sample.testToRun->testName = it->testName;
            sample.testToRun->testID = it->testID;
            sample.RunTest(sample.testToRun);
        }
    } else {
        sample.RunTest(sample.testToRun);
    }

    return 0;
}
