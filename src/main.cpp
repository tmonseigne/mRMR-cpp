#include "gtest/gtest.h"

// ReSharper disable CppUnusedIncludeDirective

// ReSharper restore CppUnusedIncludeDirective

int main(int argc, char** argv)
{
	try
	{
		testing::InitGoogleTest(&argc, argv);
		return RUN_ALL_TESTS();
	}
	catch (std::exception&) { return 1; }
}
