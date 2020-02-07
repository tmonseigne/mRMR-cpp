#include "gtest/gtest.h"
#include "CMRMR.hpp"

// ReSharper disable CppUnusedIncludeDirective

// ReSharper restore CppUnusedIncludeDirective
using namespace std;

int main(int argc, char** argv)
{
	try
	{
		CMRMR test;
		test.readCSV("res/test_lung_s3.csv");
		const std::vector<size_t> selection = test.process(0, 500, EMRMRMethod::MID);

		testing::InitGoogleTest(&argc, argv);
		return RUN_ALL_TESTS();
	}
	catch (std::exception&) { return 1; }
}
