#include <iostream>
#include "Classifier.h"
#include "DebugTools.h"
using namespace std;

int main()
{
	Classifier classifier;
	classifier.CreatePatternSet("C:/Users/Winter Pu/Desktop/pattern");
	vector<vector<Point3D>> patterns = classifier.ReadPatternsFromFiles("C:/Users/Winter Pu/Desktop/sample");

	cout << "DTW_Result" << endl;
	for (int i = 0; i < patterns.size(); i++)
	{
		cout << (PatternType)classifier.RecognizeDTW(patterns[i]) << endl;
	}

	cout<< endl;
	cout << "SW_Result" << endl;
	for (int i = 0; i < patterns.size(); i++)
	{
		cout << (PatternType)classifier.RecognizeSW(patterns[i]) << endl;
	}
	return 0;
}