///-------------------------------------------------------------------------------------------------
/// 
/// \file test_mRMR.hpp
/// \brief Tests for mRMR
/// \author Thibaut Monseigne (Inria).
/// \version 1.0.
/// \date 09/02/2020.
/// \copyright <a href="https://choosealicense.com/licenses/agpl-3.0/">GNU Affero General Public License v3.0</a>.
/// \remarks 
/// - For this tests I compare the results with the <a href="http://home.penglab.com/proj/mRMR/">software</a> of Hanchuan Peng (<a href="http://home.penglab.com/proj/mRMR/FAQ_mrmr.htm#Q1.8">License</a>)
/// 
///-------------------------------------------------------------------------------------------------

#pragma once

#include "gtest/gtest.h"
#include "CMRMR.hpp"

#ifdef _WIN32
const std::string FILENAME = "res/test_lung_s3.csv";		// With SLN we are on root folder
#else
const std::string FILENAME = "../res/test_lung_s3.csv";
#endif

const std::string SEP = "\n====================\n";

inline std::ostream& operator<<(std::ostream& os, const std::vector<size_t>& input)
{
	std::string sep = "";
	os << "(";
	for (auto const& i : input)
	{
		os << sep << i;
		sep = ", ";
	}
	os << ")";
	return os;
}

/// <summary>	Error message for vector<size_t>. </summary>
/// <param name="name">	The name of the test. </param>
/// <param name="ref"> 	The reference value. </param>
/// <param name="calc">	The calculate value. </param>
/// <returns>	Error message. </returns>
inline std::stringstream ErrorMsg(const std::string& name, const std::vector<size_t>& ref, const std::vector<size_t>& calc)
{
	std::stringstream ss;
	ss << SEP << name << " : \n********** Reference **********\n" << ref << "\n********** Compute **********\n" << calc << SEP;
	return ss;
}

class Test_mRMRM : public testing::Test
{
protected:
	CMRMR m_data;
	void SetUp() override { m_data.readCSV(FILENAME); }
};


TEST_F(Test_mRMRM, process1)
{
	const std::vector<size_t> calc = m_data.process(0, 0, EMRMRMethod::MID);
	const std::vector<size_t> ref;
	EXPECT_TRUE(ref == calc) << ErrorMsg("Process 1, threshold = 0, nFeatures = 0, method = MID", ref, calc).str();
}

TEST_F(Test_mRMRM, process2)
{
	const std::vector<size_t> calc = m_data.process(0, 10, EMRMRMethod::MID);
	const std::vector<size_t> ref  = { 230, 98, 242, 22, 181, 171, 82, 6, 248, 10 };
	EXPECT_TRUE(ref == calc) << ErrorMsg("Process 2, threshold = 0, nFeatures = 10, method = MID", ref, calc).str();
}

TEST_F(Test_mRMRM, process3)
{
	const std::vector<size_t> calc = m_data.process(0, 10, EMRMRMethod::MIQ);
	const std::vector<size_t> ref  = { 230, 139, 140, 6, 304, 234, 276, 261, 145, 142 };
	EXPECT_TRUE(ref == calc) << ErrorMsg("Process 3, threshold = 0, nFeatures = 10, method = MIQ", ref, calc).str();
}

TEST_F(Test_mRMRM, process4)
{
	const std::vector<size_t> calc = m_data.process(std::numeric_limits<double>::infinity(), 10, EMRMRMethod::MID);
	const std::vector<size_t> ref  = { 22, 125, 243, 132, 242, 29, 150, 166, 18, 269 };
	EXPECT_TRUE(ref == calc) << ErrorMsg("Process 4, no discretization, nFeatures = 10, method = MID", ref, calc).str();
}

TEST_F(Test_mRMRM, process5)
{
	const std::vector<size_t> calc = m_data.process(std::numeric_limits<double>::infinity(), 10, EMRMRMethod::MIQ);
	const std::vector<size_t> ref  = { 22, 139, 274, 104, 234, 33, 145, 105, 261, 41 };
	EXPECT_TRUE(ref == calc) << ErrorMsg("Process 5, no discretization, nFeatures = 10, method = MIQ", ref, calc).str();
}

TEST_F(Test_mRMRM, process6)
{
	const std::vector<size_t> calc = m_data.process(1, 10, EMRMRMethod::MID);
	const std::vector<size_t> ref  = { 22, 125, 243, 132, 242, 29, 150, 166, 18, 269 };
	EXPECT_TRUE(ref == calc) << ErrorMsg("Process 6, threshold = 1, nFeatures = 10, method = MID", ref, calc).str();
}

TEST_F(Test_mRMRM, process7)
{
	const std::vector<size_t> calc = m_data.process(1, 10, EMRMRMethod::MIQ);
	const std::vector<size_t> ref  = { 22, 139, 274, 104, 234, 33, 145, 105, 261, 41 };
	EXPECT_TRUE(ref == calc) << ErrorMsg("Process 7, threshold = 1, nFeatures = 10, method = MIQ", ref, calc).str();
}

TEST_F(Test_mRMRM, process8)
{
	const std::vector<size_t> calc = m_data.process(0, 500, EMRMRMethod::MID);
	const std::vector<size_t> ref  = {
		230, 98, 242, 22, 181, 171, 82, 6, 248, 10, 35, 307, 210, 166, 29, 80, 267, 137, 104, 19, 273, 214, 123, 150, 23, 244, 138, 293, 41, 67, 168, 8, 159,
		146, 18, 26, 209, 217, 125, 243, 241, 310, 163, 259, 197, 114, 154, 240, 95, 124, 147, 271, 65, 173, 20, 189, 54, 225, 84, 14, 193, 24, 169, 212, 134,
		7, 324, 206, 38, 44, 151, 202, 108, 27, 148, 234, 161, 192, 196, 160, 132, 0, 309, 190, 145, 183, 3, 233, 152, 297, 314, 238, 286, 107, 112, 79, 13,
		186, 308, 40, 179, 304, 130, 97, 46, 16, 207, 92, 1, 175, 131, 68, 270, 174, 162, 165, 94, 275, 42, 229, 64, 156, 227, 58, 187, 250, 109, 318, 322, 268,
		252, 296, 78, 218, 299, 15, 177, 48, 75, 236, 9, 222, 194, 83, 180, 223, 266, 235, 320, 204, 11, 311, 176, 55, 316, 228, 246, 239, 158, 256, 62, 269,
		33, 28, 231, 105, 205, 226, 53, 224, 260, 195, 153, 133, 4, 203, 32, 49, 5, 56, 101, 201, 215, 188, 213, 276, 129, 247, 199, 219, 142, 136, 127, 261,
		232, 106, 39, 198, 143, 263, 60, 149, 253, 99, 17, 211, 89, 255, 59, 63, 57, 172, 191, 45, 155, 122, 279, 103, 300, 86, 2, 315, 43, 264, 91, 323, 88,
		126, 70, 306, 301, 12, 167, 282, 34, 281, 74, 25, 200, 249, 72, 208, 90, 113, 237, 120, 292, 285, 257, 221, 157, 317, 303, 118, 262, 73, 93, 77, 305,
		119, 128, 71, 291, 31, 61, 85, 298, 110, 265, 178, 272, 87, 283, 319, 294, 21, 295, 116, 170, 182, 254, 100, 37, 185, 284, 111, 66, 274, 50, 277, 251,
		289, 245, 258, 220, 121, 76, 52, 36, 117, 144, 312, 51, 30, 184, 102, 287, 140, 141, 216, 278, 115, 69, 302, 290, 164, 96, 313, 280, 47, 288, 321, 139,
		81, 135
	};
	EXPECT_TRUE(ref == calc) << ErrorMsg("Process 8, threshold = 0, nFeatures = 500, method = MID", ref, calc).str();
}

TEST_F(Test_mRMRM, process9)
{
	const std::vector<size_t> calc = m_data.process(0, 500, EMRMRMethod::MIQ);
	const std::vector<size_t> ref  = {
		230, 139, 140, 6, 304, 234, 276, 261, 145, 142, 309, 53, 253, 63, 123, 72, 67, 206, 18, 40, 189, 104, 243, 21, 80, 248, 314, 92, 20, 68, 132, 38, 176,
		103, 259, 209, 166, 44, 65, 322, 228, 22, 54, 107, 46, 242, 138, 32, 75, 267, 14, 150, 263, 5, 136, 114, 48, 316, 310, 160, 133, 131, 26, 162, 99, 3,
		108, 93, 125, 306, 320, 28, 29, 98, 297, 33, 244, 165, 91, 210, 25, 212, 70, 78, 163, 240, 273, 31, 315, 90, 42, 318, 82, 13, 112, 97, 130, 83, 231, 39,
		226, 73, 319, 171, 188, 225, 8, 250, 124, 153, 168, 301, 57, 218, 59, 12, 271, 45, 10, 197, 4, 311, 181, 282, 126, 95, 41, 192, 56, 62, 217, 293, 252,
		146, 156, 0, 110, 174, 1, 159, 284, 94, 298, 196, 55, 307, 305, 239, 246, 270, 190, 60, 9, 49, 155, 183, 58, 229, 87, 61, 79, 105, 15, 207, 290, 50, 23,
		24, 106, 211, 286, 84, 247, 154, 241, 35, 30, 27, 303, 89, 199, 202, 66, 194, 237, 147, 43, 2, 203, 205, 7, 117, 214, 221, 254, 137, 233, 96, 11, 161,
		69, 152, 16, 115, 235, 100, 266, 71, 223, 149, 74, 268, 19, 52, 238, 109, 193, 204, 127, 151, 184, 111, 201, 291, 299, 308, 51, 224, 47, 186, 269, 216,
		232, 296, 195, 300, 88, 198, 37, 180, 279, 249, 260, 118, 77, 158, 220, 258, 324, 134, 102, 172, 85, 219, 213, 294, 236, 86, 256, 255, 295, 283, 17,
		128, 215, 177, 275, 222, 191, 281, 101, 285, 121, 289, 274, 179, 144, 182, 175, 113, 36, 227, 148, 64, 116, 34, 169, 119, 257, 167, 262, 187, 292, 264,
		245, 122, 265, 141, 323, 143, 129, 313, 278, 277, 200, 251, 185, 173, 120, 317, 280, 312, 272, 178, 321, 76, 208, 157, 170, 287, 302, 164, 288, 81, 135
	};
	EXPECT_TRUE(ref == calc) << ErrorMsg("Process 9, threshold = 0, nFeatures = 500, method = MIQ", ref, calc).str();
}
