///-------------------------------------------------------------------------------------------------
/// 
/// \file CMRMR.hpp
/// \brief mRMR (minimum Redundancy Maximum Relevance Feature Selection) Class.
/// \author Thibaut Monseigne (Inria).
/// \version 1.0.
/// \date 03/02/2020.
/// \copyright <a href="https://choosealicense.com/licenses/agpl-3.0/">GNU Affero General Public License v3.0</a>.
/// \remarks 
/// - This Algorithm is the same principle as <a href="http://home.penglab.com/proj/mRMR/">Hanchuan Peng et al.</a> (<a href="http://home.penglab.com/proj/mRMR/FAQ_mrmr.htm#Q1.8">License</a>). The difference is in the implementation in C++.
/// - Paper : Feature selection based on mutual information: criteria of max-dependency, max-relevance, and min-redundancy, \n
/// Hanchuan Peng, Fuhui Long, and Chris Ding, IEEE Transactions on Pattern Analysis and Machine Intelligence, Vol. 27, No. 8, pp.1226-1238, 2005.
/// 
///-------------------------------------------------------------------------------------------------

#pragma once

#include <vector>
#include <sstream>
#include <map>

enum class EMRMRMethod { MID, MIQ };

class CMRMR
{
public:	
	/// <summary> Initializes a new instance of the <see cref="CMRMR"/> class. </summary>
	CMRMR() = default;	

	/// <summary> Finalizes an instance of the <see cref="CMRMR"/> class. </summary>
	~CMRMR() { reset(); }
	
	/// <summary> Reset all member of this object. </summary>
	void reset();

	/// <summary> Prints some informations.</summary>
	//// <returns> the string with some informations. </returns>
	std::string print() const;
	
	/// <summary>Reads the CSV file (with example format in http://home.penglab.com/proj/mRMR/ website). </summary>
	/// <param name="filename">The filename.</param>
	/// <returns> True if Succes, False if Fail. </returns>
	bool readCSV(const std::string& filename);

	/// <summary> Reset previous datas and set this datas. </summary>
	/// <param name="datas">The datas.</param>
	/// <param name="classes">The classes of each sample.</param>
	/// <returns> True if success, False if fail (not same number of sample on two parameters or not same feature on each datas). </returns>
	bool setDatas(const std::vector<std::vector<double>>& datas, const std::vector<int>& classes);

	/// <summary> add this datas to actual. </summary>
	/// <param name="datas">The datas.</param>
	/// <param name="classes">The classes of each sample.</param>
	/// <returns> True if success, False if fail (not same number of sample on two parameters or not same number of feature on each datas). </returns>
	bool addDatas(const std::vector<std::vector<double>>& datas, const std::vector<int>& classes);

	/// <summary> add this sample to datas. </summary>
	/// <param name="sample">The sample.</param>
	/// <param name="classId">The classes of this sample.</param>
	/// <returns> True if success, False if fail (not same number feature than previous datas). </returns>
	bool addSample(const std::vector<double>& sample, const int classId);
	
	/// <summary> Apply the mRMR algorithm after compute zscore and discretisation. </summary>
	/// <param name="threshold">The threshold for discretization (if infinity we only use z-score).</param>
	/// <param name="nFeatures"> The number of features to keep. Default 500 is the original theorical max of the method (not needed but a wink to the origin). </param>
	/// <param name="method"> Method Used for mRMR. </param>
	/// <returns></returns>
	std::vector<size_t> process(const double threshold = std::numeric_limits<double>::infinity(), const size_t nFeatures = 500, const EMRMRMethod method = EMRMRMethod::MID);

	/// <summary>	Override the ostream operator. </summary>
	/// <param name="os">	The ostream. </param>
	/// <param name="obj">	The object. </param>
	/// <returns>	Return the modified ostream. </returns>
	friend std::ostream& operator <<(std::ostream& os, const CMRMR& obj) { return (os << obj.print()); }


private:
	size_t m_nFeatures = 0;							// Number of Features
	size_t m_nSamples  = 0;							// Number of Features
	std::map<int, std::vector<size_t>> m_classes;	// Datas in the format class -> vector id sample
	std::vector<std::vector<double>> m_datas;		// Datas in the format sample -> features
	std::vector<std::vector<double>> m_discretized;	// Datas in the format sample -> features discretized (z-score or z-score + discretization)


	std::vector<int> class2IdxVector(size_t& n) const;
	std::vector<int> discret2IdxVector(const size_t feature, size_t& n) const;
	
	/// <summary> Compute the z-score. </summary>
	/// z-score is compute for each feature separatly.\n
	/// 1 - Compute Mean of feature \f$ \mu = \frac{1}{n}\sum_{i=1}^{n}(x_{i}) \f$ \n
	/// 2 - Compute The Unbiased estimation of standard deviation \f$ \sigma = \sqrt{\frac{\sum_{i=1}^{n}(x_{i} - \mu)^{2}}{n - 1}} \f$\n
	/// 2 - Compute The z-score \f$ z_i = \frac{x_{i} - \mu}{\sigma} \f$
	void zScore();

	/// <summary> Discretizes the datas with the specified threshold (value became in set (-1, 0, 1). </summary>
	/// <param name="threshold">The threshold for discretization.</param>
	void discretize(const double threshold);

	/// <summary> Mutuals the information. </summary>
	/// <param name="feature1">The feature number 1 (default (-1) is for use the classification target instead of feature).</param>
	/// <param name="feature2">The feature number 2 (default (-1) is for use the classification target instead of feature).</param>
	/// <returns> the mutal information. </returns>
	double mutualInfo(const size_t feature1 = size_t(-1), const size_t feature2 = size_t(-1)) const;

	/// <summary> mRMR (minimum Redundancy Maximum Relevance Feature Selection) algorithm. </summary>
	/// <param name="nFeatures"> The number of features to keep. Default 500 is the original theorical max of the method (not needed but a wink to the origin). </param>
	/// <param name="method"> Method Used for mRMR. </param>
	/// <returns> The selected indexes. </returns>
	std::vector<size_t> mRMR(const size_t nFeatures = 500, const EMRMRMethod method = EMRMRMethod::MID) const;
};
