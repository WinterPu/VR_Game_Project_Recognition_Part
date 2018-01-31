#include <iostream>
#include "Classifier.h"
#include "DebugTools.h"
using namespace std;

int main()
{
	Classifier classifier;
	classifier.CreatePatternSet("C:/Users/Winter Pu/Desktop/pattern");
	
	vector<string> sample_names = GetAllFilesNamesWithinFolder("C:/Users/Winter Pu/Desktop/sample",false);
	vector<vector<Point3D>> patterns = classifier.ReadPatternsFromFiles("C:/Users/Winter Pu/Desktop","sample");


	cout << "Sample Name" << " Result " << endl;

	double dtw_failure_count = 0;
	cout << "DTW_Result" << endl;
	for (int i = 0; i < patterns.size(); i++)
	{
		string result = classifier.GetPatternName((int)classifier.RecognizeDTW(patterns[i]));
		if (sample_names[i].find(result) == string::npos)
			dtw_failure_count++;
		cout << sample_names[i]<<" ";
		cout << result << endl;
	}
	cout << " DTW Rate: "<<dtw_failure_count / sample_names.size() << endl;



	cout<< endl;
	double sw_failure_count = 0;
	cout << "SW_Result" << endl;
	for (int i = 0; i < patterns.size(); i++)
	{
		string result = classifier.GetPatternName((int)classifier.RecognizeSW(patterns[i]));
		if (sample_names[i].find(result) == string::npos)
			sw_failure_count++;
		cout << sample_names[i] << " ";
		cout << result << endl;
	}
	cout << " SW Rate: " << sw_failure_count / sample_names.size() << endl;
	return 0;
}